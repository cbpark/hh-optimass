/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "clhef/lhef.h"
#include "user_interface.h"

using std::cout;
using std::to_string;

const char appname[] = "hh_optimass_parton";

int main(int argc, char *argv[]) {
    if (argc != 2) { return howToUse(appname, "<input>"); }
    const auto to_out = &cout;  // information will be displayed in screen.

    auto fin = std::make_shared<std::ifstream>(argv[1]);
    if (fin->fail()) {
        return failToRead(appname, argv[1]);
    } else {
        message(appname, "reading `" + std::string(argv[1]) + "' ...", to_out);
    }

    auto event = lhef::parseOrFail(fin);
    int neve = 0;
    while (event.first) {
        const lhef::Particles final_states = lhef::finalStates(event.second);
        message(appname, "final states in event (" + to_string(neve + 1) + ")",
                to_out);
        cout << lhef::show(final_states) << '\n';

        ++neve;
        event = lhef::parseOrFail(fin);
    }
    message(appname, to_string(neve) + " events parsed.", to_out);
}
