/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_VARIABLES_H_
#define SRC_VARIABLES_H_

#include <ostream>
#include "lhef/lhef.h"
#include "optimass_calculator.h"
#include "parton_level.h"

namespace hhom {
class PartonLevelResult {
private:
    OptiMassResult om_;
    double mhh_ = 0;
    double mT2_bbll_ = 0;

public:
    PartonLevelResult() = delete;
    PartonLevelResult(const PartonLevel &ps)
        : om_{calcOptiMassHH<lhef::Particle>(ps)} {
        calc_variables(ps);
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const PartonLevelResult &re);

private:
    void calc_variables(const PartonLevel &ps);
};
}  // namespace hhom

#endif  // SRC_VARIABLES_H_
