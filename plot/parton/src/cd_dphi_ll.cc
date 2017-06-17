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
#include "TH2D.h"
#include "TMath.h"
#include "common.h"

using namespace std;

const char appname[] = "cd_dphi_ll";
const char title_x[] = "compatibility distance (GeV)";
const char title_y[] = "|#Delta#phi_{ll}|";

int fillHist2D(unique_ptr<ifstream> fin, shared_ptr<TH2> hist);

int main(int argc, char *argv[]) {
    if (argc != 3) { return howToUse(appname, "<input> <output>"); }
    auto fin = unique_ptr<ifstream>(new ifstream(argv[1]));
    if (fin->fail()) { return failedToRead(appname, argv[1]); }

    // Create the canvas.
    auto canvas = mkCanvas("canvas", 600);

    // Histogram.
    auto hist = make_shared<TH2D>("hist", "", 50, 0, 250, 50, 0, TMath::Pi());
    setHist(hist, title_x, title_y);

    // Fill and draw histogram
    int nev = fillHist2D(move(fin), hist);
    hist->Draw("colz");
    cout << "-- Number of events = " << nev << "\n";

    canvas->SaveAs(argv[2]);
}

int fillHist2D(unique_ptr<ifstream> fin, shared_ptr<TH2> hist) {
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

        if (mhh_om > 0 && dphi_ll < 6) {
            hist->Fill(cd, TMath::Abs(dphi_ll));
            ++nev;
        }
    }
    fin->close();
    return nev;
}
