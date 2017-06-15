/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include "TH1D.h"
#include "common.h"

using namespace std;

const char appname[] = "delta_mhh";

int main(int argc, char *argv[]) {
    if (argc != 3) { return howToUse(appname, "<hh> <output>"); }

    auto fin = unique_ptr<ifstream>(new ifstream(argv[1]));
    if (fin->fail()) { return failedToRead(appname, argv[1]); }

    // Create the canvas.
    auto canvas = mkCanvas("canvas", 600);

    const auto bin = make_pair(-0.5, 0.5);
    const double nbin = 100;

    // Histogram.
    auto hist = make_shared<TH1D>("hh", "", nbin, bin.first, bin.second);
    setHist(hist);
    hist->SetLineColor(kBlack);
    hist->SetXTitle("(m_{hh}^{OM} - m_{hh}) / m_{hh}");
    hist->SetYTitle("normalized");

    // Fill and draw histogram
    const int nev = fillHist(move(fin), hist);
    hist->DrawNormalized();
    cout << "-- Number of events = " << nev << "\n";

    canvas->SaveAs(argv[2]);
}

int fillHist(unique_ptr<ifstream> fin, shared_ptr<TH1> hist) {
    int nev = 0;
    string line;
    while (getline(*fin, line)) {
        if (line.front() == '#') { continue; }  // comment line

        istringstream iss(line);
        double mhh_om, cd, mhh, mT2_bbll, mT2_ll, dphi_ll, dR_ll, MET, pT_utm;
        int cvg;
        if (!(iss >> mhh_om >> cd >> cvg >> mhh >> mT2_bbll >> mT2_ll >>
              dphi_ll >> dR_ll >> MET >> pT_utm)) {
            break;
        }

        if (mhh > 0) {
            double var = (mhh_om - mhh) / mhh;
            hist->Fill(var);
            ++nev;
        }
    }
    fin->close();
    return nev;
}
