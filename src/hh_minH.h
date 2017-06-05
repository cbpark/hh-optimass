#ifndef SRC_HH_MINH_H_
#define SRC_HH_MINH_H_

#include <vector>
#include "alm_base/MassMinimizer.h"

namespace OptiMass {
class hh_minH : public MassMinimizer {
public:
    hh_minH() : MassMinimizer(false) {}
    ~hh_minH() { delete mass_interface_; }

    void InitContainersProlog();
    void InitContainersEpilog();

    void CalcProlog();
    void CalcStrategy(ROOT::Minuit2::MnUserParameters &params);
    void CalcEpilog() {}

    void CalcConstraints(std::vector<double> &vec_constraints,
                         std::vector<bool> &vec_constraints_using);
};
}  // namespace OptiMass

#endif  // SRC_HH_MINH_H_
