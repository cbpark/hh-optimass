/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_FINAL_STATES_H_
#define SRC_FINAL_STATES_H_

#include <string>
#include <vector>

namespace hhom {
template <typename P>
class BLSystem {
private:
    P bjet_, lepton_;
    bool filled_ = false;

public:
    BLSystem() {}
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
public:
    virtual ~FinalStates() {}
    virtual std::vector<P> bjets() const = 0;
    virtual std::vector<P> leptons() const = 0;
    virtual bool has_bl_pairs() const = 0;
    virtual BLPairs<P> bl_pairs() const = 0;
    virtual P missing() const = 0;
    virtual P utm() const = 0;
};
}  // namespace hhom

#endif  // SRC_FINAL_STATES_H_
