/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_OPTIMASS_CALCULATOR_H_
#define SRC_OPTIMASS_CALCULATOR_H_

#include <ostream>
#include "parton_level.h"

namespace hhom {
const double ETAS = 0.1;

enum class Convergence : int { Converged = 1, NotConverged = -1 };

class OptiMassResult {
private:
    double mass_;
    double sum_constraints_;
    Convergence cvg_;

public:
    OptiMassResult() = delete;
    OptiMassResult(const double mass, const double sum, const Convergence cvg)
        : mass_(mass), sum_constraints_(sum), cvg_(cvg) {}

    double mass() const { return mass_; }
    double sum_constraints() const { return sum_constraints_; }
    Convergence cvg() const { return cvg_; }

    friend std::ostream &operator<<(std::ostream &os, const OptiMassResult &re);
};

OptiMassResult calcOptiMassHH(const PartonLevel &final_states);

OptiMassResult calcOptiMassTTbar(const PartonLevel &final_states);
}  // namespace hhom

#endif  // SRC_OPTIMASS_CALCULATOR_H_
