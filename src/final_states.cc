/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "final_states.h"
#include <string>
#include <utility>
#include "clhef/lhef.h"

namespace hhom {
std::string show(const BLSystem &bl) {
    return "[" + lhef::show(bl.bquark()) + ", " + lhef::show(bl.lepton()) + "]";
}

lhef::Particle PartonLevel::missing() const {
    lhef::Particles neus = lhef::selectByID(lhef::Neutrino, fstates_);
    return lhef::sum(neus);
}

lhef::Particles PartonLevel::bquarks() const {
    return lhef::selectByID(lhef::Bottom, fstates_);
}

lhef::Particles PartonLevel::leptons() const {
    return lhef::selectByID(lhef::LeptonIso, fstates_);
}

bool valid(const lhef::Particles &ps) {
    if (ps.size() != 2) { return false; }
    if (ps[0].pid() * ps[1].pid() >= 0) { return false; }
    return true;
}

BLPairs PartonLevel::pairing() {
    const lhef::Particles bs{bquarks()};
    const lhef::Particles ls{leptons()};
    BLSystem bl1, bl2;
    if (!valid(bs) || !valid(ls)) { return std::make_pair(bl1, bl2); }

    for (const auto &b : bs) {
        if (b.pid() > 0) {
            bl1.add_bquark(b);
        } else {
            bl2.add_bquark(b);
        }
    }
    for (const auto &l : ls) {
        if (l.pid() > 0) {
            bl2.add_lepton(l);
        } else {
            bl1.add_lepton(l);
        }
    }

    bl1.set_filled(true);
    bl2.set_filled(true);
    return std::make_pair(bl1, bl2);
}

BLPairs PartonLevel::bl_wrong_pairs() const {
    BLSystem bl1{bl_pairs_.first.bquark(), bl_pairs_.second.lepton()};
    BLSystem bl2{bl_pairs_.second.bquark(), bl_pairs_.first.lepton()};
    return std::make_pair(bl1, bl2);
}
}  // namespace hhom
