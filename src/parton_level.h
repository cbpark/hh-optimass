/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_PARTON_LEVEL_H_
#define SRC_PARTON_LEVEL_H_

#include <ostream>
#include <string>
#include "final_states.h"
#include "lhef/lhef.h"
#include "optimass_calculator.h"

namespace hhom {
BLPairs<lhef::Particle> pairing(const lhef::Particles &bs,
                                const lhef::Particles &ls);

class PartonLevelData : public FinalStates<lhef::Particle> {
public:
    PartonLevelData() = delete;
    explicit PartonLevelData(const lhef::Event &e)
        : FinalStates<lhef::Particle>{lhef::finalStates(e)} {
        bl_pairs_ = pairing(bjets(), leptons());
    }

    lhef::Particles bjets() const override {
        return lhef::selectByID(lhef::Bottom, final_states_);
    }

    lhef::Particles leptons() const override {
        return lhef::selectByID(lhef::LeptonIso, final_states_);
    }

    lhef::Particle missing() const override {
        lhef::Particles neus = lhef::selectByID(lhef::Neutrino, final_states_);
        return lhef::sum(neus);
    }

    lhef::Particle utm() const override;
};

inline std::string show(const BLSystem<lhef::Particle> &bl) {
    return "[" + lhef::show(bl.bjet()) + ", " + lhef::show(bl.lepton()) + "]";
}

inline std::string show(const BLPairs<lhef::Particle> &bl_pairs) {
    BLSystem<lhef::Particle> bl1 = bl_pairs.first, bl2 = bl_pairs.second;
    return show(bl1) + "\n" + show(bl2);
}

class PartonLevelAnalysis {
private:
    OptiMassResult om_;
    double utm_;
    double mhh_;
    double mT2_bbll_, mT2_ll_;
    double dphi_ll_, dR_ll_;
    double met_;

public:
    PartonLevelAnalysis() = delete;
    PartonLevelAnalysis(const PartonLevelData &ps)
        : om_{calcOptiMassHH<lhef::Particle>(ps)}, utm_(ps.utm().pt()) {
        const lhef::Particle missing{ps.missing()};
        mT2_bbll_ = mT2_bbll(ps, missing);
        calc_variables(ps.bjets(), ps.leptons(), missing);
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const PartonLevelAnalysis &re);

private:
    double mT2_bbll(const PartonLevelData &ps, const lhef::Particle &missing);
    void calc_variables(const lhef::Particles &bjets,
                        const lhef::Particles &leptons,
                        const lhef::Particle &missing);
};
}  // namespace hhom

#endif  // SRC_PARTON_LEVEL_H_
