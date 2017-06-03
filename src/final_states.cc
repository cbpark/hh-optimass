/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "final_states.h"
#include "clhef/lhef.h"

namespace hhom {
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
}  // namespace hhom
