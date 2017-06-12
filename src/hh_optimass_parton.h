/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_HH_OPTIMASS_PARTON_H_
#define SRC_HH_OPTIMASS_PARTON_H_

#include <iostream>
#include <string>
#include "parton_level.h"
#include "user_interface.h"

void printEvent(const std::string &appname, const hhom::PartonLevelData &ps,
                const int nev, std::ostream *out) {
    message(appname, "event (" + std::to_string(nev) + ")", out);
    *out << "-- b-l pairs:\n" << hhom::show(ps.bl_pairs()) << '\n';
    *out << "-- b-l wrong pairs:\n"
         << hhom::show(ps.bl_swapped_pairs()) << '\n';
    *out << "-- missing:\n" << lhef::show(ps.missing()) << '\n';
    *out << "-- utm:\n" << lhef::show(ps.utm()) << '\n';
}

std::string header() {
    std::string hd = "# ";
    hd += "mhh_om, cd, cvg, mhh, mT2_bbll";
    return hd;
}

#endif  // SRC_HH_OPTIMASS_PARTON_H_
