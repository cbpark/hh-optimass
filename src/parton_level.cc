/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "parton_level.h"
#include <iomanip>
#include <ostream>
#include <string>
#include <utility>
#include "colevent_constants.h"
#include "lhef/lhef.h"

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

lhef::Particle PartonLevelData::utm() const {
    lhef::ParticleID b_l_neu;
    b_l_neu.insert(b_l_neu.end(), lhef::Bottom.cbegin(), lhef::Bottom.cend());
    b_l_neu.insert(b_l_neu.end(), lhef::LeptonIso.cbegin(),
                   lhef::LeptonIso.cend());
    b_l_neu.insert(b_l_neu.end(), lhef::Neutrino.cbegin(),
                   lhef::Neutrino.cend());
    lhef::Particles extra = lhef::excludeByID(b_l_neu, final_states_);
    return lhef::sum(extra);
}

double PartonLevelAnalysis::mT2_bbll(const PartonLevelData &ps,
                                     const lhef::Particle &missing) {
    if (!ps.has_bl_pairs()) { return 0; }

    const double m_invisible = 0;
    auto mT2_ = [&missing,
                 m_invisible](const BLPairs<lhef::Particle> &bl_pairs) {
        BLSystem<lhef::Particle> bl1 = bl_pairs.first, bl2 = bl_pairs.second;
        lhef::Particle sum_bl1 = lhef::sum({bl1.bjet(), bl1.lepton()}),
                       sum_bl2 = lhef::sum({bl2.bjet(), bl2.lepton()});
        return lhef::mT2(sum_bl1, sum_bl2, missing.px(), missing.py(),
                         m_invisible, m_invisible, false);
    };

    double mT2_pair[2] = {mT2_(ps.bl_pairs()), mT2_(ps.bl_swapped_pairs())};
    return std::min(mT2_pair[0], mT2_pair[1]);
}

double mT2_ll(const lhef::Particles &leptons, const lhef::Particle &missing) {
    if (leptons.size() != 2) { return 0; }
    const double m_invisible = 0;
    return lhef::mT2(leptons.front(), leptons.back(), missing.px(),
                     missing.py(), m_invisible, m_invisible, false);
}

double dphi_ll(const lhef::Particles &leptons) {
    if (leptons.size() != 2) { return colevent::TWOPI; }
    return lhef::deltaPhi(leptons.front(), leptons.back());
}

double dR_ll(const lhef::Particles &leptons) {
    if (leptons.size() != 2) { return 0; }
    return lhef::deltaR(leptons.front(), leptons.back());
}

void PartonLevelAnalysis::calc_variables(const lhef::Particles &bjets,
                                         const lhef::Particles &leptons,
                                         const lhef::Particle &missing) {
    mhh_ = lhef::invariantMass({bjets, leptons, {missing}});
    mT2_ll_ = mT2_ll(leptons, missing);
    dphi_ll_ = dphi_ll(leptons);
    dR_ll_ = dR_ll(leptons);
    met_ = missing.pt();
}

std::ostream &operator<<(std::ostream &os, const PartonLevelAnalysis &r) {
    using namespace std;

    os << r.om_;
    os << right << fixed << setw(11) << setprecision(3) << r.mhh_ << setw(10)
       << setprecision(3) << r.mT2_bbll_ << setw(9) << r.mT2_ll_ << setw(11)
       << setprecision(6) << r.dphi_ll_ << setw(10) << r.dR_ll_ << setw(10)
       << setprecision(3) << r.met_ << setw(9) << r.utm_;
    return os;
}
}  // namespace hhom
