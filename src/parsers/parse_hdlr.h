/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSE_HDLR_H_
#define PARSE_HDLR_H_

#include <memory>

#include "apt_dat.h"
#include "parse_task.h"

namespace parsers {
template <typename Tclass>
class ParseHdlrWrapper final {
public:
    std::optional<const Tclass *> get_task() const {
        ASSERT(parse_task.operator bool());
        if (!finished()) {
            return std::nullopt;
        }

        return parse_task.get()->get_data();
    }

    bool finished() const noexcept {
        ASSERT(parse_task.operator bool());
        return parse_task.get()->ready();
    }

private:
    friend class ParseHdlr;
    std::unique_ptr<ParseTask<Tclass>> parse_task;
};

class ParseHdlr final {
public:
    ParseHdlr();

    ParseHdlrWrapper<ParseAptDat> ap_database;
};
}  // namespace parsers

#endif /* PARSE_HDLR_H_ */