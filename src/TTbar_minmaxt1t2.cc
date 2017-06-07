/*
 *  Copyright (c) 2017 Won Sang Cho, Chan Beom Park
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "TTbar_minmaxt1t2.h"
#include <cmath>
#include <string>
#include <vector>
#include "alm_base/MassFunctionInterface.h"
#include "alm_base/MassMinimizer.h"
#include "constants.h"

using std::string;
using std::vector;

namespace OptiMass {
void TTbar_minmaxt1t2::InitContainersProlog() {
    process_tree_.AddProcess(string("s - t1 t2") +
                             string(" , ( t1 - b1 w1 , w1 - e1 v1 )") +
                             string(" , ( t2 - b2 w2 , w2 - e2 v2 )"));

    vector<string> vec_invisibles{"v1", "v2"};
    process_tree_.SetInvisibles(vec_invisibles);
    process_tree_.SetMass("b2", hhom::MB);
    process_tree_.SetMass("t2", hhom::MT);
    process_tree_.SetMass("v2", 0);
    process_tree_.SetMass("t1", hhom::MT);
    process_tree_.SetMass("v1", 0);
    process_tree_.SetMass("s", 0);
    process_tree_.SetMass("b1", hhom::MB);
    process_tree_.SetMass("w1", hhom::MW);
    process_tree_.SetMass("e2", 0);
    process_tree_.SetMass("e1", 0);
    process_tree_.SetMass("w2", hhom::MW);

    vector<MassFunctionInterface *> mass_interface = {
        new MassFunctionParticle(&process_tree_, "t1",
                                 &CalcMassSquareFromIndice),
        new MassFunctionParticle(&process_tree_, "t2",
                                 &CalcMassSquareFromIndice)};
    mass_interface_ =
        new MassFunctionGroup(&process_tree_, &CalcMean, mass_interface);

    vector<string> vec_optimize{"v1", "v2"};
    process_tree_.SetPtlOptimize(vec_optimize);

    vector<string> vec_buffer{"t1", "t2"};
    process_tree_.AddInvisibleSubsystem(vec_buffer);

    // Minuit Parameters
    maxfcn_ = 5000;
    init_step_size_ = 100.;

    // ALM Control Parameters (fixed)
    alm_controller_.SetALMControlParam("tau_mu", 0.5);
    alm_controller_.SetALMControlParam("b_eta0", 0.5);
    alm_controller_.SetALMControlParam("gamma", 0.2);
    alm_controller_.SetALMControlParam("nIterMax", 20);
    alm_controller_.SetALMControlParam("eta_s", 0.1);
    alm_controller_.SetALMControlParam("eta_ratio", 100.);
    alm_controller_.SetALMControlParam("b_eta", 0.3);

    const double MScale_Parent = 100.;
    const double MScale_Rel = 0.;
    const double Cmax[2] = {0.001 * MScale_Parent, 0.001 * MScale_Rel};
    alm_controller_.SetALMControlParam("eta_s", std::hypot(Cmax[0], Cmax[1]));

    // set number of constraints
    alm_controller_.SetNumberConstraints(6);
}

void TTbar_minmaxt1t2::InitContainersEpilog() {
    process_tree_.SetInvisibleSubsystemMomentaAutoCalc(0, false);
    // ALM lagrange multiplier and penalty initial values
    alm_controller_.SetInitialPenaltyParam(0.1);
    alm_controller_.SetInitialLagrangeMultiplier(0, 0);
    alm_controller_.SetInitialLagrangeMultiplier(1, 0);
    alm_controller_.SetInitialLagrangeMultiplier(2, 0);
    alm_controller_.SetInitialLagrangeMultiplier(3, 0);
    alm_controller_.SetInitialLagrangeMultiplier(4, 0);
    alm_controller_.SetInitialLagrangeMultiplier(5, 0);
    // Transverse Projection ( use this for calculate MT2 type variable
}

void TTbar_minmaxt1t2::CalcConstraints(vector<double> &vec_constraints,
                                       vector<bool> &vec_constraints_using) {
    const double t2_M = process_tree_.GetSubsystemMass("t2");
    const double w2_M = process_tree_.GetSubsystemMass("w2");
    const double t1_M = process_tree_.GetSubsystemMass("t1");
    const double w1_M = process_tree_.GetSubsystemMass("w1");

    // t1.M() - 173.
    if (vec_constraints_using.at(0)) {
        vec_constraints[0] = t1_M - hhom::MT;
    } else {
        vec_constraints[0] = 0;
    }
    // t2.M() - 173.
    if (vec_constraints_using.at(1)) {
        vec_constraints[1] = t2_M - hhom::MT;
    } else {
        vec_constraints[1] = 0;
    }
    // t1.M() - t2.M()
    if (vec_constraints_using.at(2)) {
        vec_constraints[2] = t1_M - t2_M;
    } else {
        vec_constraints[2] = 0;
    }
    // w1.M() - 80.4
    if (vec_constraints_using.at(3)) {
        vec_constraints[3] = w1_M - hhom::MW;
    } else {
        vec_constraints[3] = 0;
    }
    // w2.M() - 80.4
    if (vec_constraints_using.at(4)) {
        vec_constraints[4] = w2_M - hhom::MW;
    } else {
        vec_constraints[4] = 0;
    }
    // w1.M() - w2.M()
    if (vec_constraints_using.at(5)) {
        vec_constraints[5] = w1_M - w2_M;
    } else {
        vec_constraints[5] = 0;
    }
}
}  // namespace OptiMass
