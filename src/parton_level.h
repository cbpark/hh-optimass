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
#include "final_states.h"
#include "lhef/lhef.h"

namespace hhom {
BLPairs<lhef::Particle> pairing(const lhef::Particles &bs,
                                const lhef::Particles &ls);

class PartonLevel : public FinalStates<lhef::Particle> {
public:
    PartonLevel() = delete;
    explicit PartonLevel(const lhef::Event &e)
        : FinalStates<lhef::Particle>{lhef::finalStates(e)} {
        bl_pairs_ = pairing(bjets(), leptons());
    }

    lhef::Particles bjets() const override;
    lhef::Particles leptons() const override;
    lhef::Particle missing() const override;
    lhef::Particle utm() const override;
};

std::string show(const BLSystem<lhef::Particle> &bl);
std::string show(const BLPairs<lhef::Particle> &bl_pairs);
}  // namespace hhom

#endif  // SRC_PARTON_LEVEL_H_
