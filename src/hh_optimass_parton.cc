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
#include "TError.h"  // for gErrorIgnoreLevel
#include "clhef/lhef.h"
#include "optimass_calculator.h"
#include "parton_level.h"
#include "user_interface.h"

using std::cout;

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

#if !defined(DEBUG)
    gErrorIgnoreLevel = 1001;  // to make Minuit2 quiet
#endif

    auto event{lhef::parseEvent(&fin)};
    for (int iev = 1; !event.done(); event = lhef::parseEvent(&fin), ++iev) {
        hhom::PartonLevel ps{event};
#if DEBUG
        message(appname, "event (" + std::to_string(iev) + ")", to_out);
        cout << "-- b-l pairs:\n" << hhom::show(ps.bl_pairs()) << '\n';
        cout << "-- missing:\n" << lhef::show(ps.missing()) << '\n';
#endif
        if (!ps.has_bl_pairs()) { continue; }

        hhom::OptiMassResult om = hhom::calcOptiMassHH<lhef::Particle>(ps);
        // hhom::OptiMassResult om{hhom::calcOptiMassTTbar<lhef::Particle>(ps)};
        cout << om << '\n';
    }
}
