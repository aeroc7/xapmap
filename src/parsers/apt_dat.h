/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef APT_DAT_H_
#define APT_DAT_H_

#include <string>
#include <vector>

namespace parsers {
class ParseAptDat {
public:
    ParseAptDat(const std::string &path);

private:
    std::vector<std::string> scenery_directories(const std::string &path) const;
};
}  // namespace parsers

#endif /* APT_DAT_H_ */