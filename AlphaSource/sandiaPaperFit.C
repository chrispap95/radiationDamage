#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TString.h>

#include "RootStyle.cc"

using namespace std;

void sandiaPaperFit()
{
    // Use global style for plots
    set_root_style();

    // Input section: put here all data from Sandia paper
    TString labels[] = {
        "22 C",
        "60 C",
        //"12.7 mm - 22 C",
        //"12.7 mm - 60 C",
        //"6.35 mm - 22 C",
        //"6.35mm - 60C",
        //"3.18mm - 22C",
        //"3.18mm - 60C",
        //"BP - 22C",
    };

    vector<vector<float>> dR = {
        {52, 55, 153, 163, 278, 55, 153, 163, 278, 163, 278}, // 22C
        {55, 163, 278, 163, 278, 278},                        // 60 C

        {18.2, 52, 55, 153, 163, 278}, // 12.7 mm - 22 C
        {18.2, 55, 163, 278},          // 12.7 mm - 60 C
        {55, 153, 163, 278},           // 6.35 mm - 22 C
        {163, 278},                    // 6.35 mm - 60 C
        {163, 278},                    // 3.18 mm - 22 C
        {278},                         // 3.18 mm - 60 C
        {52, 153},                     // BP - 22 C
    };

    vector<vector<float>> lox = {
        {3.5, 3.2, 1.7, 1.5, 1.2, 3.1, 1.55, 1.35, 1.0, 1.5, 1.15},
        {3.3, 2.0, 1.6, 1.85, 1.45, 1.6},

        {6.0, 3.5, 3.2, 1.7, 1.5, 1.2},
        {5.2, 3.3, 2.0, 1.6},
        {3.1, 1.55, 1.35, 1.0},
        {1.85, 1.45},
        {1.5, 1.15},
        {1.6},
        {2.5, 1.3},
    };

    // Initialize error arrays
    vector<vector<float>> edR;
    vector<vector<float>> elox;

    // Plots
    int cols[] = {1, 4, 8};
    size_t n = sizeof(labels) / sizeof(labels[0]);
    TCanvas *c1 = new TCanvas("c1", "c1", 1);
    TMultiGraph *mgr = new TMultiGraph();
    TGraphErrors *gr[n];
    for (size_t i = 0; i < n; ++i)
    {
        size_t n_i = dR[i].size();
        vector<float> edR_i;
        vector<float> elox_i;
        for (size_t j = 0; j < n_i; ++j)
        {
            edR_i.push_back(dR[i][j] * 0.1);
            elox_i.push_back(0.2);
        }
        edR.push_back(edR_i);
        elox.push_back(elox_i);
        gr[i] = new TGraphErrors(n_i, dR[i].data(), lox[i].data(), edR[i].data(), elox[i].data());
        gr[i]->SetTitle(labels[i]);
        gr[i]->SetLineColor(cols[i]);
        gr[i]->SetMarkerColor(cols[i]);
        mgr->Add(gr[i]);
    }
    mgr->Draw("AP0");
    mgr->GetXaxis()->SetTitle("Dose rate (Gy/h)");
    mgr->GetYaxis()->SetTitle("Depth (mm)");
    mgr->SetMinimum(0.0);
    mgr->SetMaximum(8.0);
    mgr->GetXaxis()->SetLimits(0, 350);

    // Fits
    TF1 *func[n];
    TString annotation[n];
    for (size_t i = 0; i < n; ++i)
    {
        func[i] = new TF1("func" + (TString)to_string(i), "[0]/sqrt(x)");
        func[i]->SetLineColor(cols[i]);
        // func[i]->SetParameters(1, 1);
        gr[i]->Fit("func" + (TString)to_string(i), "", "", 1, 350);
        float nchi2 = func[i]->GetChisquare() / func[i]->GetNDF();
        cout << "chi2: " << nchi2 << endl;
        float A = pow(func[i]->GetParameter(0), 2);
        float eA = 2 * func[i]->GetParameter(0) * func[i]->GetParError(0);
        char result[100];
        sprintf(result, "%.0f #pm %.0fmm^{2}Gy/h - #chi^{2}=%.2f", A, eA, nchi2);
        annotation[i] = "Eq 9 par: " + (TString)result;
        func[i]->Draw("same");
    }

    // Legend
    TLegend *lg = new TLegend(0.2, 0.65, 0.8, 0.85);
    lg->SetBorderSize(0);
    lg->SetFillColorAlpha(0, 0);
    lg->SetTextSize(0.04);
    for (size_t i = 0; i < n; ++i)
    {
        lg->AddEntry(gr[i], "#splitline{" + labels[i] + "}{" + annotation[i] + "}", "p");
    }
    lg->Draw();

    // Text
    TLatex lx;
    lx.DrawLatex(134, 4, "Fit with:    #frac{A}{sqrt(x)}");

    /*
    // Transform for fit section
    vector<vector<float>> x;
    vector<vector<float>> y;
    vector<vector<float>> ex;
    vector<vector<float>> ey;
    int cols[] = {1, 4, 8};
    TCanvas *c2 = new TCanvas("c2", "c2", 1);
    TMultiGraph *mgr_n = new TMultiGraph();
    TGraphErrors *gr[n];
    for (size_t i = 0; i < n; ++i)
    {
        size_t n_i = dR[i].size();
        vector<float> edR_i;
        vector<float> elox_i;
        for (size_t j = 0; j < n_i; ++j)
        {
            edR_i.push_back(dR[i][j] * 0.05);
            elox_i.push_back(0.1);
        }
        edR.push_back(edR_i);
        elox.push_back(elox_i);
        gr[i] = new TGraphErrors(n_i, dR[i].data(), lox[i].data(), edR[i].data(), elox[i].data());
        gr[i]->SetTitle(labels[i]);
        gr[i]->SetLineColor(cols[i]);
        gr[i]->SetMarkerColor(cols[i]);
        mgr->Add(gr[i]);
    }
    mgr->Draw("AP0");
    mgr->GetXaxis()->SetTitle("Dose rate (Gy/h)");
    mgr->GetYaxis()->SetTitle("Depth (mm)");
    mgr->SetMinimum(0.0);
    mgr->SetMaximum(8.0);
    mgr->GetXaxis()->SetLimits(0, 350);

    // Fits
    TF1 *func[n];
    for (size_t i = 0; i < n; ++i)
    {
        func[i] = new TF1("func" + (TString)to_string(i), "[0]/sqrt(x)");
        func[i]->SetLineColor(cols[i]);
        // func[i]->SetParameters(1, 1);
        gr[i]->Fit("func" + (TString)to_string(i), "", "", 1, 350);
        cout << "chi2: " << func[i]->GetChisquare() / func[i]->GetNDF() << endl;
        float A = pow(func[i]->GetParameter(0), 2);
        float eA = 2 * func[i]->GetParameter(0) * func[i]->GetParError(0);
        cout << "Eq 9 parameter: " << A << " +/- " << eA << "mm^2Gy/h" << endl;
        func[i]->Draw("same");
    }

    // Legend
    TLegend *lg = new TLegend(0.4, 0.65, 0.6, 0.85);
    lg->SetBorderSize(0);
    lg->SetFillColorAlpha(0, 0);
    lg->SetTextSize(0.04);
    for (size_t i = 0; i < n; ++i)
    {
        lg->AddEntry(gr[i], labels[i], "p");
    }
    lg->Draw();

    // Text
    TLatex lx;
    lx.DrawLatex(134, 4, "Fit with:    #frac{A}{sqrt(x)}");
    */
}