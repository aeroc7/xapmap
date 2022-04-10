/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "apt_dat.h"

#include <utils/from_chars_full.h>
#include <utils/str_utils.h>

#include <exception>
#include <fstream>
#include <typeinfo>

#include "fast_fliner.h"
#include "parse_except.h"

using namespace utils;

namespace parsers {
static inline void only_raise_info(const ParseError &e) {
    Log(Log::ERROR) << "Parse error: " << e.what();
}

template <typename T>
static T num_from_str(std::string_view str, const LrCbParam &p) {
    T out_val;

    // Unlike C's `atof` (and some other implementations), a leading '+' is not allowed.
    // As from_chars implementations do not ignore leading whitespace, if there
    // is a '+', it will be the first character. Filter it out so it doesn't return
    // as a parse error.

    if (str[0] == '+') {
        str.remove_prefix(1);
    }

    const auto [ptr, ec] = utils::from_chars<T>(str.begin(), str.end(), out_val);

    switch (ec) {
        case std::errc::invalid_argument:
            only_raise_info(
                ParseError("Failed to convert '" + std::string(str) + "' to numerical type.",
                    p.filename, p.line_num));
            break;
        case std::errc::result_out_of_range:
            only_raise_info(ParseError("The number " + std::string(str) +
                                           " is out of range for the type " + typeid(T).name(),
                p.filename, p.line_num));
            break;
        default:
            break;
    }

    return out_val;
}

static bool sv_check_good(const str::ss_return &s, const LrCbParam &p) {
    using cu_str_err = str::ss_error;
    switch (s.err) {
        case cu_str_err::out_of_range:
            only_raise_info(
                ParseError("Index-string-length mismatch (out of range)", p.filename, p.line_num));
            return false;
        case cu_str_err::str_empty:
            only_raise_info(ParseError("String was empty", p.filename, p.line_num));
            return false;
        case cu_str_err::success:
        default:
            return true;
    }
}

static inline std::string_view set_default_str_ne(
    const str::ss_return &str_val, const LrCbParam &p) {
    if (sv_check_good(str_val, p)) {
        return str_val.str;
    }

    return "";
}

static std::string_view split_after_safe(const LrCbParam &pline, std::size_t index) {
    const auto split_val = str::split_string_tend_sv(pline.line, index);
    return set_default_str_ne(split_val, pline);
}

template <typename Lambda>
class FileLineReader {
public:
    FileLineReader(const std::string &path, Lambda cb_on_line, const std::atomic_bool &stop)
        : on_line(cb_on_line), file_basename(filename_split(path)), stop_parsing(stop) {
        read_file(path);
    }

private:
    void read_file(const std::string &path) const {
        parsers::FastFliner flr(
            path,
            [this](const parsers::LineData &data) {
                this->user_cb(data);
            },
            stop_parsing);
    }

    void user_cb(const parsers::LineData &data) const {
        on_line(LrCbParam{data.line_data, data.line_number, "ASDASD"});
    }

    Lambda on_line;
    static constexpr std::size_t LINE_BUF_SIZE_INIT = 1024;
    std::string file_basename;
    const std::atomic_bool &stop_parsing;
};

ParseAptDat::ParseAptDat(const std::string &path, const std::atomic_bool &stop)
    : stop_parsing{stop} {
    const auto sd = scenery_directories(path);

    for (const auto &e : sd) {
        parse_apt_dat_file(path + e);
    }
}

void ParseAptDat::verify_node_zerod(const LrCbParam &p) const {
    if (cur_node != NodeOpenType::NONE) {
        throw ParseError("Node is supposed to be zerod but has a value of " +
                             std::to_string(static_cast<int>(cur_node)),
            p.filename, p.line_num);
    }
}

std::vector<std::string> ParseAptDat::scenery_directories(const std::string &path) const {
    std::vector<std::string> file_paths;

    FileLineReader(
        path + "Custom Scenery/scenery_packs.ini",
        [&](const LrCbParam &p) {
            const auto row_item = str::split_string_sv(p.line, 0);

            if (row_item.str == "SCENERY_PACK") {
                const auto file_loc = str::split_string_tend_sv(p.line, 1);
                switch (file_loc.err) {
                    case str::ss_error::out_of_range:
                        only_raise_info(ParseError(
                            "Attempted string split-to-end is out of range (line contents: '" +
                                std::string(p.line) + "')",
                            p.filename, p.line_num));
                        return;
                    case str::ss_error::str_empty:
                        only_raise_info(ParseError(
                            "Tried to extract pathname but it was empty", p.filename, p.line_num));
                        return;
                    case str::ss_error::success:
                        break;
                }

                file_paths.push_back(std::string{file_loc.str});
            }
        },
        stop_parsing);

    return file_paths;
}

void ParseAptDat::cur_apt_start(const LrCbParam &p) {
    const auto icao_str = str::split_string_sv(p.line, 4);
    const auto ap_name_str = str::split_string_tend_sv(p.line, 5);

    cur_apt.icao = set_default_str_ne(icao_str, p);
    cur_apt.name = set_default_str_ne(ap_name_str, p);
}

void ParseAptDat::cur_apt_end(const LrCbParam &) {
    if (str::cmp_equal_sv(cur_apt.icao.c_str(), "") == false) {
        airport_db[cur_apt.icao] = cur_apt;
    }

    cur_apt = {};
}

void ParseAptDat::cur_apt_1302(const LrCbParam &p) {
    const auto data_category = str::split_string_sv(p.line, 1);

    if (str::cmp_equal_sv(data_category.str, "city")) {
        cur_apt.city = split_after_safe(p, 2);
    } else if (str::cmp_equal_sv(data_category.str, "state")) {
        cur_apt.state = split_after_safe(p, 2);
    } else if (str::cmp_equal_sv(data_category.str, "country")) {
        cur_apt.country = split_after_safe(p, 2);
    } else if (str::cmp_equal_sv(data_category.str, "datum_lat")) {
        cur_apt.coords.lat = num_from_str<double>(split_after_safe(p, 2), p);
    } else if (str::cmp_equal_sv(data_category.str, "datum_lon")) {
        cur_apt.coords.lon = num_from_str<double>(split_after_safe(p, 2), p);
    }
}

void ParseAptDat::cur_apt_100(const LrCbParam &p) {
    AirportRunwayData cur_rwy;

    cur_rwy.width = num_from_str<double>(split_after_safe(p, 1), p);

    for (std::size_t i = 0; i < 2; ++i) {
        const auto rwy_str_offset = (i * 9);

        // Copy over runway name
        const auto rwy_name = split_after_safe(p, 8 + rwy_str_offset);
        std::memcpy(cur_rwy.name[i].begin(), rwy_name.begin(),
            std::min(rwy_name.size(), cur_rwy.name[i].size()));

        // Runway lat & lon
        cur_rwy.latitude[i].lat = num_from_str<double>(split_after_safe(p, 9 + rwy_str_offset), p);
        cur_rwy.latitude[i].lon = num_from_str<double>(split_after_safe(p, 10 + rwy_str_offset), p);
    }

    cur_apt.runways.push_back(std::move(cur_rwy));
}

// Node
void ParseAptDat::cur_apt_111(const LrCbParam &p) {
    switch (cur_node) {
        case NodeOpenType::AIRPORT_BOUNDARY:
            cur_apt.bounds.push_back(extract_node(p));
            break;
        default:
            break;
    }
}

// Node with ending
void ParseAptDat::cur_apt_113(const LrCbParam &p) {
    cur_apt_111(p);
    clear_cur_node();
}

// Bezier curve node with ending
void ParseAptDat::cur_apt_114(const LrCbParam &) {
    clear_cur_node();
}

// Airport boundary start
void ParseAptDat::cur_apt_130(const LrCbParam &p) {
    verify_node_zerod(p);
    set_cur_node(NodeOpenType::AIRPORT_BOUNDARY);
}

CoordPair ParseAptDat::extract_node(const LrCbParam &p) {
    CoordPair ret;
    // Ignores row code--that should already be figured out by the caller.
    // Part 1 should contain the latitude, and part 2 should contain the
    // longitude.

    ret.lat = num_from_str<double>(split_after_safe(p, 1), p);
    ret.lon = num_from_str<double>(split_after_safe(p, 2), p);

    return ret;
}

void ParseAptDat::parse_apt_dat_file(const std::string &file) {
    FileLineReader(
        file + "Earth nav data/apt.dat",
        [this](const LrCbParam &p) {
            const auto row_code_str = str::split_string_sv(p.line, 0);

            if (str::cmp_equal_sv(row_code_str.str, "1")) {
                // Land airport
                cur_apt_end(p);
                cur_apt_start(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "100")) {
                // Runway
                cur_apt_100(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "1302")) {
                // Airport metadata
                cur_apt_1302(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "111")) {
                // Node
                cur_apt_111(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "113")) {
                // Node with implicit close of loop
                cur_apt_113(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "114")) {
                // Node with implicit close of loop (with bezier control point)
                cur_apt_114(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "130")) {
                // Airport boundary header
                cur_apt_130(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "99")) {
                // End of file, finish up last airport
                cur_apt_end(p);
            }
        },
        stop_parsing);
}

std::optional<const AirportData *> ParseAptDat::get_icao_info(const std::string &icao) const {
    if (!airport_db.contains(icao)) {
        return std::nullopt;
    }

    return {&airport_db.at(icao)};
}
}  // namespace parsers