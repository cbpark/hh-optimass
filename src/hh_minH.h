#ifndef SRC_OPTIMASS_HH_MINH_
#define SRC_OPTIMASS_HH_MINH_

#include <vector>
#include "Minuit2/MnUserParameters.h"
#include "alm_base/MassMinimizer.h"

namespace OptiMass {

class hh_minH : public MassMinimizer {
public:
    hh_minH() {}
    ~hh_minH() { delete mass_interface_; }

    void InitContainersProlog();
    void InitContainersEpilog();
    void CalcProlog() {
        init_step_size_ = process_tree_.GetEffectiveScale();
    }
    void CalcStrategy(ROOT::Minuit2::MnUserParameters &params) {
        MinimizeCombined(params);
    }
    void CalcEpilog() {}
    void CalcConstraints(std::vector<double> &vec_constraints,
                         std::vector<bool> &vec_constraints_using);
};
}  // namespace optimass

#endif  // SRC_OPTIMASS_HH_MINH_
