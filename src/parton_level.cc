/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "parton_level.h"
#include <string>
#include <utility>
#include "clhef/lhef.h"

namespace hhom {
bool valid(const lhef::Particles &ps) {
    if (ps.size() != 2) { return false; }
    // Since the total system is neutral, we must have a particle and an
    // anti-particle.
    if (ps[0].pid() * ps[1].pid() >= 0) { return false; }
    return true;
}

BLPairs<lhef::Particle> pairing(const lhef::Particles &bs,
                                const lhef::Particles &ls) {
    BLSystem<lhef::Particle> bl1, bl2;
    if (!valid(bs) || !valid(ls)) { return std::make_pair(bl1, bl2); }

    lhef::transformParticles(bs, [&bl1, &bl2](const lhef::Particle &b) {
        b.pid() > 0 ? bl1.set_bjet(b) : bl2.set_bjet(b);
    });
    lhef::transformParticles(ls, [&bl1, &bl2](const lhef::Particle &l) {
        l.pid() > 0 ? bl2.set_lepton(l) : bl1.set_lepton(l);
    });

    bl1.set_filled(true);
    bl2.set_filled(true);
    return std::make_pair(bl1, bl2);
}

lhef::Particles PartonLevel::bjets() const {
    return lhef::selectByID(lhef::Bottom, final_states_);
}

lhef::Particles PartonLevel::leptons() const {
    return lhef::selectByID(lhef::LeptonIso, final_states_);
}

lhef::Particle PartonLevel::missing() const {
    lhef::Particles neus = lhef::selectByID(lhef::Neutrino, final_states_);
    return lhef::sum(neus);
}

lhef::Particle PartonLevel::utm() const {
    lhef::ParticleID b_l_neu;
    b_l_neu.insert(b_l_neu.end(), lhef::Bottom.cbegin(), lhef::Bottom.cend());
    b_l_neu.insert(b_l_neu.end(), lhef::LeptonIso.cbegin(),
                   lhef::LeptonIso.cend());
    b_l_neu.insert(b_l_neu.end(), lhef::Neutrino.cbegin(),
                   lhef::Neutrino.cend());
    lhef::Particles extra = lhef::excludeByID(b_l_neu, final_states_);
    return lhef::sum(extra);
}

BLPairs<lhef::Particle> PartonLevel::bl_wrong_pairs() const {
    BLSystem<lhef::Particle> bl1{bl_pairs_.first.bjet(),
                                 bl_pairs_.second.lepton()};
    BLSystem<lhef::Particle> bl2{bl_pairs_.second.bjet(),
                                 bl_pairs_.first.lepton()};
    return std::make_pair(bl1, bl2);
}

std::string show(const BLSystem<lhef::Particle> &bl) {
    return "[" + lhef::show(bl.bjet()) + ", " + lhef::show(bl.lepton()) + "]";
}

std::string show(const BLPairs<lhef::Particle> &bl_pairs) {
    const BLSystem<lhef::Particle> bl1 = bl_pairs.first, bl2 = bl_pairs.second;
    return show(bl1) + "\n" + show(bl2);
}
}  // namespace hhom
