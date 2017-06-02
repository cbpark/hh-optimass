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
#include "user_interface.h"

using std::cout;
using std::to_string;

const char appname[] = "hh_optimass_parton";

int main(int argc, char *argv[]) {
    if (argc != 2) { return howToUse(appname, "<input>"); }
    const auto to_out = &cout;  // information will be displayed in screen.

    std::ifstream fin{argv[1]};
    if (fin.fail()) {
        return failToRead(appname, argv[1]);
    } else {
        message(appname, "reading `" + std::string(argv[1]) + "' ...", to_out);
    }

    lhef::Event lhe = lhef::parseEvent(&fin);
    unsigned long neve = 0;
    for (unsigned long ieve = 0; !lhe.empty();
         lhe = lhef::parseEvent(&fin), ++ieve, neve = ieve) {
        lhef::Particles finalStates = lhef::finalStates(lhe);
        message(appname, "final states in event (" + to_string(ieve + 1) + ")",
                to_out);
        cout << lhef::show(finalStates) << '\n';
    }
    message(appname, to_string(neve) + " events parsed.", to_out);
}
