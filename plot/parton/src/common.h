/*
 *  Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of hh-optimass, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef PLOT_SRC_COMMON_H_
#define PLOT_SRC_COMMON_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"

const int FONT = 132;

int howToUse(const std::string &appname, const std::string &usage) {
    std::cerr << "Usage: " << appname << " " + usage << '\n';
    return 1;
}

int failedToRead(const std::string &appname, const std::string &fname) {
    std::cerr << appname << ": failed to read `" << fname << "'.\n";
    return 1;
}

int fillHist(std::unique_ptr<std::ifstream> fin, std::shared_ptr<TH1> hist);

std::unique_ptr<TCanvas> mkCanvas(const char *name, const int dim) {
    auto canvas =
        std::unique_ptr<TCanvas>(new TCanvas(name, "", 0, 0, dim, dim));
    canvas->SetTicks();
    gStyle->SetOptStat(0);  // no stat panel
    return canvas;
}

void setHist(std::shared_ptr<TH1> hist, const char *title_x = "",
             const char *title_y = "") {
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->CenterTitle();
    hist->SetTitle("");
    hist->SetTitleOffset(0.9, "x");
    hist->SetLineWidth(2);
    hist->SetTitleSize(1.4 * hist->GetLabelSize(), "xy");
    hist->SetTitleFont(FONT, "xy");
    hist->SetLabelFont(FONT, "xy");
    hist->SetXTitle(title_x);
    hist->SetYTitle(title_y);
}

std::shared_ptr<TLatex> mkText() {
    auto text = std::make_shared<TLatex>();
    text->SetTextFont(FONT);
    text->SetTextSize(0.9 * text->GetTextSize());
    text->SetNDC();
    return text;
}

std::shared_ptr<TLegend> mkLegend(const double x1, const double y1,
                                  const double x2, const double y2) {
    auto legend = std::make_shared<TLegend>(x1, y1, x2, y2);
    legend->SetTextFont(FONT);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);
    return legend;
}

#endif  // PLOT_SRC_COMMON_H_
