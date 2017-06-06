/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "optimass_calculator.h"
#include "TError.h"  // for gErrorIgnoreLevel
#include "alm_base/ALMController.h"
#include "alm_base/MassMinimizer.h"
#include "alm_base/ProcessTree.h"
#include "hh_minH.h"
#include "parton_level.h"

namespace hhom {
double calcOptiMassHH(const PartonLevel &fstates) {
    OptiMass::MassMinimizer *optm = new OptiMass::hh_minH;

    optm->InitContainers();
    const BLPairs bl_pairs = fstates.bl_pairs();
    const BLSystem bl1 = bl_pairs.first, bl2 = bl_pairs.second;
    optm->SetMomentumValue("b1_x", bl1.bquark().px());
    optm->SetMomentumValue("b1_y", bl1.bquark().py());
    optm->SetMomentumValue("b1_z", bl1.bquark().pz());
    optm->SetMomentumValue("b1_m", bl1.bquark().mass());
    optm->SetMomentumValue("e1_x", bl1.lepton().px());
    optm->SetMomentumValue("e1_y", bl1.lepton().py());
    optm->SetMomentumValue("e1_z", bl1.lepton().pz());
    optm->SetMomentumValue("e1_m", bl1.lepton().mass());
    optm->SetMomentumValue("b2_x", bl2.bquark().px());
    optm->SetMomentumValue("b2_y", bl2.bquark().py());
    optm->SetMomentumValue("b2_z", bl2.bquark().pz());
    optm->SetMomentumValue("b2_m", bl2.bquark().mass());
    optm->SetMomentumValue("e2_x", bl2.lepton().px());
    optm->SetMomentumValue("e2_y", bl2.lepton().py());
    optm->SetMomentumValue("e2_z", bl2.lepton().pz());
    optm->SetMomentumValue("e2_m", bl2.lepton().mass());

    const auto met = fstates.missing();
    optm->SetInvisibleSubsystemMomenta(0, met.px(), met.py());
    optm->SetInitInvisibleMomentum("v1_m", 0.);
    optm->SetInitInvisibleMomentum("v2_m", 0.);

    auto alm_controller = optm->GetALMController();
    alm_controller.UseConstraint(0, true);
    alm_controller.UseConstraint(1, true);
    alm_controller.UseConstraint(2, false);

    gErrorIgnoreLevel = 1001;
    optm->Calc();

    auto process_tree = optm->GetProcessTree();
    return process_tree.GetSubsystemMass("H");
}
}  // namespace hhom
