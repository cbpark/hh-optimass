/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_FINAL_STATES_H_
#define SRC_FINAL_STATES_H_

#include "clhef/lhef.h"

namespace hhom {
class PartonLevel {
private:
    lhef::Particles fstates_;

public:
    PartonLevel() = delete;
    explicit PartonLevel(const lhef::Event &e)
        : fstates_(lhef::finalStates(e)) {}

    lhef::Particle missing() const;
    lhef::Particles bquarks() const;
    lhef::Particles leptons() const;
};
}  // namespace hhom

#endif  // SRC_FINAL_STATES_H_
