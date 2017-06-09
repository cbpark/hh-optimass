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
#include <string>
#include "TTbar_minmaxt1t2.h"
#include "alm_base/ALMController.h"
#include "alm_base/MassMinimizer.h"
#include "alm_base/ProcessTree.h"
#include "final_states.h"
#include "hh_minH.h"
#include "lhef/lhef.h"

namespace hhom {
std::ostream &operator<<(std::ostream &os, const OptiMassResult &re) {
    using namespace std;

    os << right << fixed << setw(10) << setprecision(3) << re.mass_ << setw(11)
       << setprecision(5) << re.sum_constraints_ << setw(5)
       << static_cast<int>(re.cvg_);
    return os;
}

template <typename P>
void initOptiMass(const FinalStates<P> &final_states,
                  OptiMass::MassMinimizer *optm) {
    optm->InitContainers();

    const BLPairs<P> bl_pairs{final_states.bl_pairs()};
    const BLSystem<P> bl1{bl_pairs.first}, bl2{bl_pairs.second};
    optm->SetMomentumValue("b1_x", bl1.bjet().px());
    optm->SetMomentumValue("b1_y", bl1.bjet().py());
    optm->SetMomentumValue("b1_z", bl1.bjet().pz());
    optm->SetMomentumValue("b1_m", bl1.bjet().mass());
    optm->SetMomentumValue("e1_x", bl1.lepton().px());
    optm->SetMomentumValue("e1_y", bl1.lepton().py());
    optm->SetMomentumValue("e1_z", bl1.lepton().pz());
    optm->SetMomentumValue("e1_m", bl1.lepton().mass());
    optm->SetMomentumValue("b2_x", bl2.bjet().px());
    optm->SetMomentumValue("b2_y", bl2.bjet().py());
    optm->SetMomentumValue("b2_z", bl2.bjet().pz());
    optm->SetMomentumValue("b2_m", bl2.bjet().mass());
    optm->SetMomentumValue("e2_x", bl2.lepton().px());
    optm->SetMomentumValue("e2_y", bl2.lepton().py());
    optm->SetMomentumValue("e2_z", bl2.lepton().pz());
    optm->SetMomentumValue("e2_m", bl2.lepton().mass());

    const P met{final_states.missing()};
    optm->SetInvisibleSubsystemMomenta(0, met.px(), met.py());
    optm->SetInitInvisibleMomentum("v1_m", 0.);
    optm->SetInitInvisibleMomentum("v2_m", 0.);
}

OptiMassResult getOptiMassResult(OptiMass::MassMinimizer *optm,
                                 const OptiMass::ALMController &alm_controller,
                                 const std::string &sys_name) {
    OptiMass::ProcessTree &process_tree = optm->GetProcessTree();
    double om = process_tree.GetSubsystemMass(sys_name);
    double cd = alm_controller.GetSumSquaredConstraints();
    Convergence cvg = Convergence::NotConverged;
    if (cd <= ETAS) { cvg = Convergence::Converged; }
    return {om, cd, cvg};
}

template <typename P>
OptiMassResult calcOptiMassHH(const FinalStates<P> &final_states) {
    // return an empty result if the final states do not contain b-lepton pairs.
    if (!final_states.has_bl_pairs()) { return OptiMassResult(); }

    OptiMass::MassMinimizer *optm = new OptiMass::hh_minH();
    initOptiMass(final_states, optm);
    OptiMass::ALMController &alm_controller = optm->GetALMController();
    alm_controller.UseConstraint(0, true);
    alm_controller.UseConstraint(1, true);
    alm_controller.UseConstraint(2, false);
    optm->Calc();

    return getOptiMassResult(optm, alm_controller, "H");
}

template OptiMassResult calcOptiMassHH<lhef::Particle>(
    const FinalStates<lhef::Particle> &final_states);

template <typename P>
OptiMassResult calcOptiMassTTbar(const FinalStates<P> &final_states) {
    if (!final_states.has_bl_pairs()) { return OptiMassResult(); }

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

    return getOptiMassResult(optm, alm_controller, "s");
}

template OptiMassResult calcOptiMassTTbar<lhef::Particle>(
    const FinalStates<lhef::Particle> &final_states);
}  // namespace hhom
