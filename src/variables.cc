/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "variables.h"
#include <iomanip>
#include <ostream>
#include "lhef/lhef.h"
#include "parton_level.h"

namespace hhom {
void PartonLevelResult::calc_variables(const PartonLevel &ps) {
    lhef::Particles met{ps.missing()};
    mhh_ = lhef::invariantMass({ps.bjets(), ps.leptons(), met});
}

std::ostream &operator<<(std::ostream &os, const PartonLevelResult &re) {
    using namespace std;

    os << re.om_;
    os << right << fixed << setw(10) << setprecision(3) << re.mhh_;
    return os;
}
}  // namespace hhom
