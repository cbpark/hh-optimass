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

const char appname[] = "mhh_compare";
const char title_x[] = "m_{hh} (GeV)";
const char title_y[] = "normalized";

int fillHist(unique_ptr<ifstream> fin, shared_ptr<TH1> hist1,
             shared_ptr<TH1> hist2);

int main(int argc, char *argv[]) {
    if (argc != 3) { return howToUse(appname, "<hh> <output>"); }
    auto fin = unique_ptr<ifstream>(new ifstream(argv[1]));
    if (fin->fail()) { return failedToRead(appname, argv[1]); }

    // Create the canvas.
    auto canvas = mkCanvas("canvas", 600);

    const auto bin = make_pair(150, 1000);
    const double nbin = 50;

    // Histogram.
    auto hist1 = make_shared<TH1D>("hh_true", "", nbin, bin.first, bin.second);
    setHist(hist1, title_x, title_y);
    hist1->SetLineColor(kBlack);

    auto hist2 = make_shared<TH1D>("hh_om", "", nbin, bin.first, bin.second);
    setHist(hist2, title_x, title_y);
    hist2->SetLineColor(kBlue);
    hist2->SetLineStyle(2);

    // Fill and draw histogram
    const int nev = fillHist(move(fin), hist1, hist2);
    hist1->DrawNormalized();
    hist2->DrawNormalized("SAME");
    cout << "-- Number of events = " << nev << "\n";

    auto legend = mkLegend(0.6, 0.75, 1.0, 0.85);
    legend->AddEntry("hh_true", " true");
    legend->AddEntry("hh_om", " OM reco.");
    legend->Draw();

    canvas->SaveAs(argv[2]);
}

int fillHist(unique_ptr<ifstream> fin, shared_ptr<TH1> hist1,
             shared_ptr<TH1> hist2) {
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
            hist1->Fill(mhh);
            hist2->Fill(mhh_om);
            ++nev;
        }
    }
    fin->close();
    return nev;
}
