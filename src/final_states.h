/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_FINAL_STATES_H_
#define SRC_FINAL_STATES_H_

#include <utility>
#include <vector>

namespace hhom {
template <typename P>
class BLSystem {
private:
    P bjet_, lepton_;
    bool filled_;

public:
    BLSystem() : filled_(false) {}
    BLSystem(const P &bjet, const P &lepton)
        : bjet_(bjet), lepton_(lepton), filled_(true) {}

    void set_bjet(const P &bjet) { bjet_ = bjet; }
    void set_lepton(const P &lepton) { lepton_ = lepton; }

    bool is_filled() const { return filled_; }
    void set_filled(bool filled) { filled_ = filled; }
    P bjet() const { return bjet_; }
    P lepton() const { return lepton_; }
};

template <typename P>
using BLPairs = std::pair<BLSystem<P>, BLSystem<P>>;

/**
 *  Interface for calculating collider observables.
 */
template <typename P>
class FinalStates {
protected:
    std::vector<P> final_states_;

    /*
     * The b and lepton pair does not make sense since they have different
     * parents in the Higgs pair processes. This is to use for the top pair
     * process, which is the most dominant background.
     */
    BLPairs<P> bl_pairs_;

public:
    explicit FinalStates(const std::vector<P> &final_state)
        : final_states_(final_state) {}
    virtual ~FinalStates() {}

    virtual std::vector<P> bjets() const = 0;
    virtual std::vector<P> leptons() const = 0;
    virtual P missing() const = 0;
    virtual P utm() const = 0;

    bool has_bl_pairs() const {
        return bl_pairs_.first.is_filled() && bl_pairs_.second.is_filled();
    }
    BLPairs<P> bl_pairs() const { return bl_pairs_; }
};
}  // namespace hhom

#endif  // SRC_FINAL_STATES_H_
