/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "optimass_calculator.h"
#include <iomanip>
#include <ostream>
#include "TTbar_minmaxt1t2.h"
#include "alm_base/ALMController.h"
#include "alm_base/MassMinimizer.h"
#include "alm_base/ProcessTree.h"
#include "hh_minH.h"
#include "parton_level.h"
#ifdef DEBUG
#include <iostream>
#endif

namespace hhom {
std::ostream &operator<<(std::ostream &os, const OptiMassResult &re) {
    using namespace std;

    os << right << fixed << setw(10) << setprecision(3) << re.mass_ << setw(11)
       << setprecision(5) << re.sum_constraints_ << setw(5)
       << static_cast<int>(re.cvg_);
    return os;
}

void initOptiMass(const PartonLevel &final_states,
                  OptiMass::MassMinimizer *optm) {
    optm->InitContainers();

    const BLPairs bl_pairs = final_states.bl_pairs();
    const BLSystem bl1 = bl_pairs.first, bl2 = bl_pairs.second;
#ifdef DEBUG
    std::cout << "-- b-l pair:\n";
    std::cout << show(bl1) << '\n' << show(bl2) << '\n';
#endif

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

    const auto met = final_states.missing();
#ifdef DEBUG
    std::cout << "-- missing:\n" << show(met) << '\n';
#endif

    optm->SetInvisibleSubsystemMomenta(0, met.px(), met.py());
    optm->SetInitInvisibleMomentum("v1_m", 0.);
    optm->SetInitInvisibleMomentum("v2_m", 0.);
}

OptiMassResult optiMassResult(OptiMass::MassMinimizer *optm,
                              const OptiMass::ALMController &alm_controller,
                              const std::string &sys_name) {
    OptiMass::ProcessTree &process_tree = optm->GetProcessTree();
    const double om = process_tree.GetSubsystemMass(sys_name);
    const double cd = alm_controller.GetSumSquaredConstraints();
    Convergence cvg = Convergence::NotConverged;
    if (cd <= ETAS) { cvg = Convergence::Converged; }
    return OptiMassResult{om, cd, cvg};
}

OptiMassResult calcOptiMassHH(const PartonLevel &final_states) {
    OptiMass::MassMinimizer *optm = new OptiMass::hh_minH();
    initOptiMass(final_states, optm);
    OptiMass::ALMController &alm_controller = optm->GetALMController();
    alm_controller.UseConstraint(0, true);
    alm_controller.UseConstraint(1, true);
    alm_controller.UseConstraint(2, false);
    optm->Calc();

    return optiMassResult(optm, alm_controller, "H");
}

OptiMassResult calcOptiMassTTbar(const PartonLevel &final_states) {
    OptiMass::MassMinimizer *optm = new OptiMass::TTbar_minmaxt1t2();
    initOptiMass(final_states, optm);
    OptiMass::ALMController &alm_controller = optm->GetALMController();
    alm_controller.UseConstraint(0, true);
    alm_controller.UseConstraint(1, true);
    alm_controller.UseConstraint(2, false);
    alm_controller.UseConstraint(3, true);
    alm_controller.UseConstraint(4, true);
    alm_controller.UseConstraint(5, false);
    optm->Calc();

    return optiMassResult(optm, alm_controller, "s");
}
}  // namespace hhom
