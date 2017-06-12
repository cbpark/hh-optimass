/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "hh_optimass_parton.h"
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include "TError.h"  // for gErrorIgnoreLevel
#include "lhef/lhef.h"
#include "parton_level.h"
#include "user_interface.h"
#include "variables.h"

using std::cout;

const char appname[] = "hh_optimass_parton";

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) { return howToUse(appname, "<input> [output]"); }
    auto to_out = &cout;  // information will be displayed in screen.

    std::ifstream fin(argv[1]);
    if (fin.fail()) {
        return failToRead(appname, argv[1]);
    } else {
        message(appname, "reading `" + std::string(argv[1]) + "' ...", to_out);
    }

    // prepare output
    std::streambuf *buf;
    std::ofstream of;
    bool has_output = argc == 3;
    if (has_output) {
        of.open(argv[2]);
        buf = of.rdbuf();
    } else {
        buf = cout.rdbuf();
    }
    std::ostream out(buf);
    out << header() << '\n';

#if !defined(DEBUG)
    gErrorIgnoreLevel = 1001;  // to make Minuit2 quiet
#endif

    auto event{lhef::parseEvent(&fin)};
    int nev = 0;
    for (; !event.done(); event = lhef::parseEvent(&fin)) {
        printProgress(appname, 1000, ++nev, to_out);
        hhom::PartonLevel ps{event};
#if DEBUG
        printEvent(appname, ps, nev, to_out);
#endif
        if (!ps.has_bl_pairs()) { continue; }

        auto result = hhom::PartonLevelResult(ps);
        out << result << '\n';
    }

    message(appname, "... done.", to_out);
    message(appname,
            "total " + std::to_string(nev) + " events have been processed.",
            to_out);
    if (has_output) {
        message(appname,
                "the output has been saved to `" + std::string(argv[2]) + "'.",
                to_out);
    }
}
