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
    /*measurements.push_back(
        {
            true,
            {0.22,0.22,0.22},
            {4.33751,4.27079,4.34031},
            {0.02*0.1,0.02*0.1,0.02*0.1},
            {0.14129,0.13446,0.14025},
            4,34,1.5,"GIF++",1,NULL
        }
    );
    measurements.push_back(
        {
            true,
            {8.53,8.53,8.34,74.4,74.4,390,390,390},
            {11.80330,11.85346,11.31170,10.36126,11.76172,9.78101,11.44537,7.89202},
            {8.53*0.1,8.53*0.1,8.34*0.1,74.4*0.1,74.4*0.1,390*0.1,390*0.1,390*0.1},
            {11.80330*0.01126,11.85346*0.01118,11.31170*0.0114,10.36126*0.01117,
                11.76172*0.01125,9.78101*0.0113,11.44537*0.01123,7.89202*0.0118
            },2,34,1.5,"Co-60 (NIST)",1,NULL
        }
    );
    measurements.push_back(
        {
            true,
            {0.98,0.98,0.31,0.31},
            {7.91476,7.63909,5.61088,5.67994},
            {0.98*0.1,0.98*0.1,0.31*0.1,0.31*0.1},
            {7.91476*0.01119,7.63909*0.01116,5.61088*0.01121,5.67994*0.01114},
            3,34,1.5,"Gamma(GSFC REF)",1,NULL
        }
    );*/

    measurements.push_back(
        {
            true,
            {8.53,74.4,390},
            {11.80330 ,10.36126 ,9.78101},
            {8.53*0.1,74.4*0.1,390*0.1},
            {11.80330*0.01126,10.36126*0.01117,9.78101*0.0113},
            2,34,1.5,"Co-60 (NIST)",1,NULL
        }
    );
    measurements.push_back(
        {
            true,
            {0.98,0.31},
            {7.91476 ,5.61088},
            {0.98*0.1,0.31*0.1},
            {7.91476*0.01119,5.61088*0.01121},
            3,34,1.5,"Gamma(GSFC REF)",1,NULL
        }
    );
    measurements.push_back(
        {
            true,
            {0.22},
            {4.33751},
            {0.02*0.1},
            {0.14129},
            4,34,1.5,"GIF++",1,NULL
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
    allgraphs->GetYaxis()->SetTitleOffset(1.52);

    // NEED TO TUNE THESE:
    allgraphs->SetMinimum(1e-2);
    allgraphs->SetMaximum(50.0);
    double xmin = 1.e-2;
    double xmax = 2.e3;
    allgraphs->GetXaxis()->SetLimits(xmin,xmax);

    // Prepare the legend
    TLegend *legendA = new TLegend(0.57,0.22,0.92,0.42);
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

    canv->SaveAs(filename+".png");
    canv->SaveAs(filename+".pdf");
    canv->SaveAs(filename+".root");
    canv->SaveAs(filename+".C");

}
