/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "variables.h"
#include <algorithm>  // for std::min
#include <iomanip>
#include <ostream>
#include "lhef/lhef.h"
#include "parton_level.h"

namespace hhom {
double mT2_bbll(const PartonLevel &final_states) {
    if (!final_states.has_bl_pairs()) { return 0; }

    const lhef::Particle met{final_states.missing()};
    double mT2_pair[2] = {mT2(final_states.bl_pairs(), met, 0),
                          mT2(final_states.bl_swapped_pairs(), met, 0)};

    return std::min(mT2_pair[0], mT2_pair[1]);
}

void PartonLevelResult::calc_variables(const PartonLevel &ps) {
    lhef::Particles met{ps.missing()};
    mhh_ = lhef::invariantMass({ps.bjets(), ps.leptons(), met});
    mT2_bbll_ = mT2_bbll(ps);
}

std::ostream &operator<<(std::ostream &os, const PartonLevelResult &re) {
    using namespace std;

    os << re.om_;
    os << right << fixed << setw(10) << setprecision(3) << re.mhh_ << setw(10)
       << setprecision(3) << re.mT2_bbll_;
    return os;
}
}  // namespace hhom
