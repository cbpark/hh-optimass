/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include <fstream>
#include <iostream>
#include <string>
#include "clhef/lhef.h"
#include "final_states.h"
#include "user_interface.h"

using std::cout;
using std::to_string;

const char appname[] = "hh_optimass_parton";

int main(int argc, char *argv[]) {
    if (argc != 2) { return howToUse(appname, "<input>"); }
    const auto to_out = &cout;  // information will be displayed in screen.

    std::ifstream fin(argv[1]);
    if (fin.fail()) {
        return failToRead(appname, argv[1]);
    } else {
        message(appname, "reading `" + std::string(argv[1]) + "' ...", to_out);
    }

    auto event{lhef::parseEvent(&fin)};
    for (; !event.done(); event = lhef::parseEvent(&fin)) {
        hhom::PartonLevel ps{event};
        if (!ps.have_bl_pairs()) { continue; }
        const auto bl_pairs{ps.bl_pairs()};
        cout << "b-l pairs:\n"
             << show(bl_pairs.first) << '\n'
             << show(bl_pairs.second) << '\n';
        const auto bl_wrong_pairs{ps.bl_wrong_pairs()};
        cout << "b-l pairs (wrong):\n"
             << show(bl_wrong_pairs.first) << '\n'
             << show(bl_wrong_pairs.second) << '\n';
        cout << "missing:\n" << lhef::show(ps.missing()) << '\n';
    }
}