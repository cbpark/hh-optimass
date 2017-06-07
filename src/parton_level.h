/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_PARTON_LEVEL_H_
#define SRC_PARTON_LEVEL_H_

#include <string>
#include "clhef/lhef.h"
#include "final_states.h"

namespace hhom {
class PartonLevel : public FinalStates<lhef::Particle> {
private:
    lhef::Particles final_states_;

    /*
     * The b and lepton pair does not make sense since they have different
     * parents in the Higgs pair processes. This is to use for the top pair
     * process, which is the most dominant background.
     */
    BLPairs<lhef::Particle> bl_pairs_;

public:
    PartonLevel() = delete;
    explicit PartonLevel(const lhef::Event &e)
        : final_states_(lhef::finalStates(e)), bl_pairs_(pairing()) {}

    lhef::Particles bjets() const override;
    lhef::Particles leptons() const override;
    bool has_bl_pairs() const override {
        return bl_pairs_.first.is_filled() && bl_pairs_.second.is_filled();
    }
    BLPairs<lhef::Particle> bl_pairs() const override { return bl_pairs_; }
    BLPairs<lhef::Particle> bl_wrong_pairs() const;

    lhef::Particle missing() const override;
    lhef::Particle utm() const override;

private:
    std::pair<BLSystem<lhef::Particle>, BLSystem<lhef::Particle>> pairing();
};

std::string show(const BLSystem<lhef::Particle> &bl);
std::string show(const BLPairs<lhef::Particle> &bl_pairs);
}  // namespace hhom

#endif  // SRC_PARTON_LEVEL_H_
