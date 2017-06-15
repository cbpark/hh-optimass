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

const char appname[] = "mhh_true";

int main(int argc, char *argv[]) {
    if (argc != 4) { return howToUse(appname, "<hh> <ttbar> <output>"); }
    auto fin_hh = unique_ptr<ifstream>(new ifstream(argv[1]));
    if (fin_hh->fail()) { return failedToRead(appname, argv[1]); }
    auto fin_tt = unique_ptr<ifstream>(new ifstream(argv[2]));
    if (fin_tt->fail()) { return failedToRead(appname, argv[2]); }

    // Create the canvas.
    auto canvas = mkCanvas("canvas", 600);

    const auto bin = make_pair(150, 1000);
    const double nbin = 50;

    // Histogram.
    auto hist_hh = make_shared<TH1D>("hh", "", nbin, bin.first, bin.second);
    setHist(hist_hh);
    hist_hh->SetLineColor(kBlack);
    hist_hh->SetXTitle("m_{hh} (GeV)");
    hist_hh->SetYTitle("normalized");

    auto hist_tt = make_shared<TH1D>("ttbar", "", nbin, bin.first, bin.second);
    setHist(hist_tt);
    hist_tt->SetLineColor(kRed);
    hist_tt->SetLineStyle(2);

    // Fill and draw histogram
    const int nev_hh = fillHist(move(fin_hh), hist_hh);
    hist_hh->DrawNormalized();

    const int nev_tt = fillHist(move(fin_tt), hist_tt);
    hist_tt->DrawNormalized("SAME");
    cout << "-- Number of events = (" << nev_hh << ", " << nev_tt << ")\n";

    auto legend = mkLegend(0.7, 0.75, 1.0, 0.85);
    legend->AddEntry("hh", " hh");
    legend->AddEntry("ttbar", " t#bar{t}");
    legend->Draw();

    canvas->SaveAs(argv[3]);
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
            hist->Fill(mhh);
            ++nev;
        }
    }
    fin->close();
    return nev;
}
