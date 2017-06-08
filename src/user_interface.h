/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_USER_INTERFACE_H_
#define SRC_USER_INTERFACE_H_

#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

int howToUse(const std::string &appname, const std::string &usage) {
    std::cerr << "Usage: " << appname << " " + usage << '\n';
    return 1;
}

int failToRead(const std::string &appname, const std::string &fname) {
    std::cerr << appname << ": failed to read `" << fname << "'\n";
    return 1;
}

void message(const std::string &appname, const std::string &msg,
             std::ostream *out) {
    *out << appname << ": " << msg << '\n';
}

void printProgress(const std::string &appname, const int how_many, const int n,
                   std::ostream *out) {
    if (n % how_many == 0) {
        message(appname, std::to_string(n) + " events parsed.", out);
    }
}

#endif  // SRC_USER_INTERFACE_H_
