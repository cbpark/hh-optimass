#include "hh_minH.h"
#include <cmath>
#include <string>
#include <vector>
#include "alm_base/MassFunctionInterface.h"
#include "alm_base/MassMinimizer.h"
#include "alm_base/MathUtils.h"
#include "alm_base/ProcessTree.h"
#include "constants.h"

using std::string;
using std::vector;

namespace OptiMass {
void hh_minH::InitContainersProlog() {
    process_tree_.AddProcess(
        string("H - h1 h2 , ( h1 - b1 b2 )") +
        string(" , ( h2 - w1 w2 , ( w1 - e1 v1 ) , ( w2 - e2 v2 ) )"));

    vector<string> vec_invisibles{"v1", "v2"};
    process_tree_.SetInvisibles(vec_invisibles);
    process_tree_.SetMass("w1", hhom::MW);
    process_tree_.SetMass("h2", 0);
    process_tree_.SetMass("H", 0);
    process_tree_.SetMass("h1", 0);
    process_tree_.SetMass("v1", 0);
    process_tree_.SetMass("v2", 0);
    process_tree_.SetMass("b1", hhom::MB);
    process_tree_.SetMass("b2", hhom::MB);
    process_tree_.SetMass("e2", 0);
    process_tree_.SetMass("e1", 0);
    process_tree_.SetMass("w2", hhom::MW);

    MassFunctionInterface *mass_interface = new MassFunctionParticle(
        &process_tree_, "H", &CalcMassSquareFromIndice);
    mass_interface_ =
        new MassFunctionGroup(&process_tree_, &CalcMean, {mass_interface});

    vector<string> vec_optimize{"v1", "v2"};
    process_tree_.SetPtlOptimize(vec_optimize);

    vector<string> vec_buffer{"H"};
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
    const double Cmax[2]{0.001 * MScale_Parent, 0.001 * MScale_Rel};
    alm_controller_.SetALMControlParam("eta_s", std::hypot(Cmax[0], Cmax[1]));

    // set number of constraints
    alm_controller_.SetNumberConstraints(3);
}

void hh_minH::InitContainersEpilog() {
    process_tree_.SetInvisibleSubsystemMomentaAutoCalc(0, false);
    // ALM lagrange multiplier and penalty initial values
    alm_controller_.SetInitialPenaltyParam(0.1);
    alm_controller_.SetInitialLagrangeMultiplier(0, 0);
    alm_controller_.SetInitialLagrangeMultiplier(1, 0);
    alm_controller_.SetInitialLagrangeMultiplier(2, 0);
    // Transverse Projection ( use this for calculate MT2 type variable
}

void hh_minH::CalcConstraints(vector<double> &vec_constraints,
                              vector<bool> &vec_constraints_using) {
    const double h2_M = process_tree_.GetSubsystemMass("h2");
    const double h1_M = process_tree_.GetSubsystemMass("h1");

    // h1.M() - 125.
    if (vec_constraints_using.at(0)) {
        vec_constraints[0] = h1_M - hhom::MH;
    } else {
        vec_constraints[0] = 0;
    }
    // h2.M() - 125.
    if (vec_constraints_using.at(1)) {
        vec_constraints[1] = h2_M - hhom::MH;
    } else {
        vec_constraints[1] = 0;
    }
    // h1.M() - h2.M()
    if (vec_constraints_using.at(2)) {
        vec_constraints[2] = h1_M - h2_M;
    } else {
        vec_constraints[2] = 0;
    }
}
}  // namespace OptiMass
