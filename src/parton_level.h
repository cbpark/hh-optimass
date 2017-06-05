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
#include <utility>
#include "clhef/lhef.h"

namespace hhom {
class BLSystem {
private:
    lhef::Particle bquark_, lepton_;
    bool filled_ = false;

public:
    BLSystem() {}
    BLSystem(const lhef::Particle &bquark, const lhef::Particle &lepton)
        : bquark_(bquark), lepton_(lepton), filled_(true) {}

    void add_bquark(const lhef::Particle &bquark) { bquark_ = bquark; }
    void add_lepton(const lhef::Particle &lepton) { lepton_ = lepton; }

    bool is_filled() const { return filled_; }
    void set_filled(bool filled) { filled_ = filled; }
    lhef::Particle bquark() const { return bquark_; }
    lhef::Particle lepton() const { return lepton_; }
};

std::string show(const BLSystem &bl);

using BLPairs = std::pair<BLSystem, BLSystem>;

class PartonLevel {
private:
    lhef::Particles fstates_;

    /*
     * The b and lepton pair does not make sense since they have different
     * parents in the Higgs pair processes. This is to use for the top pair
     * process, which is the most dominant background.
     */
    BLPairs bl_pairs_;
    lhef::Particle utm_;

public:
    PartonLevel() = delete;
    explicit PartonLevel(const lhef::Event &e)
        : fstates_{lhef::finalStates(e)}, bl_pairs_{pairing()} {}

    bool have_bl_pairs() const {
        return bl_pairs_.first.is_filled() && bl_pairs_.second.is_filled();
    }
    BLPairs bl_pairs() const { return bl_pairs_; }
    BLPairs bl_wrong_pairs() const;

    lhef::Particle missing() const;
    lhef::Particle utm() const;

private:
    std::pair<BLSystem, BLSystem> pairing();
    lhef::Particles bquarks() const;
    lhef::Particles leptons() const;
};
}  // namespace hhom

#endif  // SRC_PARTON_LEVEL_H_
