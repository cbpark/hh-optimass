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
#include "optimass_calculator.h"
#include "parton_level.h"
#include "user_interface.h"

using std::to_string;

const char appname[] = "hh_optimass_parton";

int main(int argc, char *argv[]) {
    if (argc != 2) { return howToUse(appname, "<input>"); }
    const auto to_out = &std::cout;  // information will be displayed in screen.

    std::ifstream fin(argv[1]);
    if (fin.fail()) {
        return failToRead(appname, argv[1]);
    } else {
        message(appname, "reading `" + std::string(argv[1]) + "' ...", to_out);
    }

    auto event{lhef::parseEvent(&fin)};
    for (; !event.done(); event = lhef::parseEvent(&fin)) {
        hhom::PartonLevel ps{event};
        if (!ps.has_bl_pairs()) { continue; }

        hhom::OptiMassResult om{hhom::calcOptiMassHH(ps)};
        std::cout << om << '\n';
    }
}
