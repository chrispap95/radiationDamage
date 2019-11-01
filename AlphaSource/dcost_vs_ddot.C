#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <vector>

using namespace std;

struct measurement {
    bool include;
    vector<float> x;
    vector<float> y;
    vector<float> ex;
    vector<float> ey;
    short markerColor; // line color and marker fill color (if it applies) will use the same value
    short markerStyle;
    float markerSize;
    string legendLabel;
    short legendId; // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
    TGraphErrors* graph; // this pointer gets filled in drawing section; it is initially null
};

void dcost_vs_ddot(TString tagTime="default") {


    // // Preambles
    // gROOT->SetStyle("UMD");

    // // overwrite style para
    // gStyle->SetEndErrorSize(2.5);
    // gStyle->SetPadTickX(0);
    // gStyle->SetPadRightMargin(0.02);
    // gStyle->SetPadLeftMargin(0.12);
    // gStyle->SetTitleYOffset(1.2);

    cout << "File date tag = " << tagTime << endl;

    // ======================================================================================================
    // Begin of results section
    // ======================================================================================================

    // This is where people decides which measurements shall be included in the plot
    // and add new measurements to the list of plottable results.
    // Please make sure that there is a sensible comment identifying each new result.
    // Ideally, this is the ONLY section that needs to be updated whenever a new measurement is available.

    vector<measurement> measurements;
    measurements.push_back(
        {
            false,
            {0.22*0.92,0.31*0.92,0.31*0.92,0.98*0.92,8.53*0.92,74.4*0.92,390*0.92},
            {3.31789,5.74305,4.87321,6.78443,12.31375,13.46707,15.89024},
            {0.22*0.1,0.31*0.1,0.31*0.1,0.98*0.1,8.53*0.008,74.4*0.008,390*0.008},
            {0.59431,0.80162,0.74794,0.83448,0.57133,0.62179,0.73247},
            2,33,1.2,"EJ200PVT-1X1P",1,NULL
        }
    );

    measurements.push_back(
        {
            false,
            {0.22*1.0,0.31*1.0,0.31*1.0,0.98*1.0,8.53*1.0,390*1.0},
            {3.67532,7.02963,5.71187,7.36742,12.77335,15.34901},
            {0.22*0.1,0.31*0.1,0.31*0.1,0.98*0.1,8.53*0.008,390*0.008},
            {0.62754,0.9026,0.82169,0.8698,0.57146,0.6859},
            2,34,1.2,"EJ260PVT-1X1P",1,NULL
        }
    );

    measurements.push_back(
        {
            false,
            {0.22*0.96,8.53*0.96,74.4*0.96,390*0.96,0.31*0.96,0.98*0.96,0.31*0.96},
            {4.40333,12.88039,11.11065,10.61664,7.3069,7.88074,5.56525},
            {0.22*0.1,8.53*0.008,74.4*0.008,390*0.008,0.31*0.1,0.98*0.1,0.31*0.1},
            {0.69842,0.59055,0.50843,0.4885,0.92941,0.90251,0.81116},
            4,33,1.2,"EJ200PS-1X1P",1,NULL
        }
    );

    measurements.push_back(
        {
            false,
            {0.22*1.04,8.34*1.04,0.31*1.04,390*1.04,0.98*1.04,0.31*1.04},
            {4.4927,15.12669,4.68655,13.10698,9.09121,6.43075},
            {0.22*0.1,8.34*0.008,0.31*0.1,390*0.008,0.98*0.1,0.31*0.1},
            {0.70351,0.66963,0.71287,0.57875,0.97303,0.88434},
            4,34,1.2,"EJ260PS-1X1P",1,NULL
        }
    );

    measurements.push_back(
        {
            true,
            {0.22*0.95,8.34*0.97,390*0.97,0.31*0.95},
            {4.40622,12.07512,8.40505,6.39462},
            {0.22*0.1,8.34*0.056,390*0.056,0.31*0.1},
            {0.69878,0.55584,0.3948,0.85466},
            4,22,1.2,"EJ200PS-2X1P",1,NULL
        }
    );

    measurements.push_back(
        {
            true,
            {0.22,0.31},
            {4.3876,8.18087},
            {0.22*0.1,0.31*0.1},
            {0.69642,0.99314},
            2,23,1.2,"EJ260PVT-2X1P",1,NULL
        }
    );

    measurements.push_back(
        {
            true,
            {0.22*1.05,8.34*1.03,390*1.03,0.31*1.05},
            {3.53379,13.18657,17.29626,6.42334},
            {0.22*0.1,8.34*0.056,390*0.056,0.31*0.1},
            {0.61619,0.6076,0.79456,0.85839},
            2,22,1.2,"EJ200PVT-2X1P",1,NULL
        }
    );

    // ======================================================================================================
    // End of results section
    // ======================================================================================================

    // ======================================================================================================
    // Begin of graph preparation section
    // ======================================================================================================

    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i) {
        // skip measurements not to be included
        if (!i->include) continue;

        // check sanity of input
        unsigned int n = i->x.size();
        if (i->y.size() !=n ||
        i->ex.size()!=n ||
        i->ey.size()!=n ) {
            cout << "Check inputs in sample: " << i->legendLabel.c_str() << endl;
            continue;
        }

        TGraphErrors *graph = new TGraphErrors(n,i->x.data(),i->y.data(),i->ex.data(),i->ey.data());
        graph->SetTitle();
        graph->GetXaxis()->SetTitle();
        graph->SetMarkerColor(i->markerColor);
        graph->SetFillColor(i->markerColor);
        graph->SetLineColor(i->markerColor);
        graph->SetMarkerSize(i->markerSize);
        graph->SetMarkerStyle(i->markerStyle);

        // create working pointer only when measurement is to be plotted
        i->graph = graph;

    }

    // ======================================================================================================
    // End of results section
    // ======================================================================================================

    // ======================================================================================================
    // Drawing
    // ======================================================================================================

    // Canvas
    TCanvas *canv = new TCanvas("canv","D versus doserate",200,10,800,600);
    canv->SetFillColor(0);
    canv->SetLogy();
    canv->SetLogx();

    // MultiGraph with all data points
    TMultiGraph *allgraphs = new TMultiGraph();
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i)
    if (i->include)
    allgraphs->Add(i->graph);

    allgraphs->Draw("AP");
    allgraphs->GetXaxis()->SetTitle("Dose rate (krad/hr)");
    allgraphs->GetYaxis()->SetTitle("Dose Constant (Mrad)");
    allgraphs->GetYaxis()->SetTitleOffset(1.2);
    allgraphs->GetXaxis()->SetTitleOffset(1.3);

    // NEED TO TUNE THESE:
    allgraphs->SetMinimum(2.0);
    allgraphs->SetMaximum(20.0);
    double xmin = 5.e-2;
    double xmax = 2.e3;
    allgraphs->GetXaxis()->SetLimits(xmin,xmax);

    // Prepare the legend
    TLegend *legendA = new TLegend(0.57,0.22,0.89,0.42);
    legendA->SetBorderSize(0);
    legendA->SetFillColor(0);
    for (vector<measurement>::iterator i = measurements.begin();i!=measurements.end();++i)
    if (i->include && i->legendId==1)
    legendA->AddEntry(i->graph,i->legendLabel.c_str(),"p");
    legendA->Draw();

    TLegend *legendB = new TLegend(0.20,0.7,0.63,0.92);
    legendB->SetBorderSize(0);
    legendB->SetFillColor(0);
    for (vector<measurement>::iterator i = measurements.begin();i!=measurements.end();++i)
    if (i->include && i->legendId==2)
    legendB->AddEntry(i->graph,i->legendLabel.c_str(),"p");
    //legendB->Draw();

    // use TPad to get border and fill color, TLatex does not have them
    // TLatex *material = new TLatex(0.8,0.94);
    // material->SetBorderSize(1);
    // material->SetFillColor(0);
    // material->SetTextAlign(11);
    // material->DrawLatex("#color[2]{#Box} PS based\\#color[4]{#Box} PVT based");

    canv->Update();

    TString filename = TString("doserate_");
    filename+=tagTime;

    //canv->SaveAs(filename+".png");
    canv->SaveAs(filename+".eps");
    //canv->SaveAs(filename+".root");
    //canv->SaveAs(filename+".C");

}
