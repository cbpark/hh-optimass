/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>

namespace hhom {
std::string fileName(const std::string &path) {
    return path.substr(path.find_last_of('/') + 1);
}

std::string baseName(const std::string &path, const std::string &ext) {
    std::string fname{fileName(path)};
    std::string tmp_ext = ext;
    tmp_ext.insert(0, 1, '.');
    return fname.substr(0, fname.find_last_of(tmp_ext) - ext.size());
}
}  // namespace hhom

#endif  // SRC_UTILS_H_
