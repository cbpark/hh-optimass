/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_OPTIMASS_CALCULATOR_H_
#define SRC_OPTIMASS_CALCULATOR_H_

#include "parton_level.h"

namespace hhom {
double calcOptiMassHH(const PartonLevel &final_states);
}  // namespace hhom

#endif  // SRC_OPTIMASS_CALCULATOR_H_
