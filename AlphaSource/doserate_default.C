void doserate_default()
{
//=========Macro generated from canvas: canv/D versus doserate
//=========  (Thu Oct 24 21:50:09 2019) by ROOT version 6.14/04
   TCanvas *canv = new TCanvas("canv", "D versus doserate",200,45,800,600);
   canv->Range(-2.662629,-0.1626288,3.963659,1.463659);
   canv->SetFillColor(0);
   canv->SetBorderMode(0);
   canv->SetBorderSize(2);
   canv->SetLogx();
   canv->SetLogy();
   canv->SetFrameBorderMode(0);
   canv->SetFrameBorderMode(0);
   
   TMultiGraph *multigraph = new TMultiGraph();
   multigraph->SetName("");
   multigraph->SetTitle("");
   
   Double_t Graph_fx1001[7] = {
   0.22,
   0.31,
   0.31,
   0.98,
   8.53,
   74.4,
   390};
   Double_t Graph_fy1001[7] = {
   3.31789,
   5.74305,
   4.87321,
   6.78443,
   12.31375,
   13.46707,
   15.89024};
   Double_t Graph_fex1001[7] = {
   0.022,
   0.031,
   0.031,
   0.098,
   0.06824,
   0.5952,
   3.12};
   Double_t Graph_fey1001[7] = {
   0.59431,
   0.80162,
   0.74794,
   0.83448,
   0.57133,
   0.62179,
   0.73247};
   TGraphErrors *gre = new TGraphErrors(7,Graph_fx1001,Graph_fy1001,Graph_fex1001,Graph_fey1001);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillColor(2);
   gre->SetFillStyle(1000);
   gre->SetLineColor(2);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(33);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_Graph1001 = new TH1F("Graph_Graph1001","",100,0.1782,432.4122);
   Graph_Graph1001->SetMinimum(1.333667);
   Graph_Graph1001->SetMaximum(18.01262);
   Graph_Graph1001->SetDirectory(0);
   Graph_Graph1001->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1001->SetLineColor(ci);
   Graph_Graph1001->GetXaxis()->SetLabelFont(42);
   Graph_Graph1001->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1001->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1001->GetXaxis()->SetTitleFont(42);
   Graph_Graph1001->GetYaxis()->SetLabelFont(42);
   Graph_Graph1001->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1001->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1001->GetYaxis()->SetTitleOffset(0);
   Graph_Graph1001->GetYaxis()->SetTitleFont(42);
   Graph_Graph1001->GetZaxis()->SetLabelFont(42);
   Graph_Graph1001->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1001->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1001);
   
   multigraph->Add(gre,"");
   
   Double_t Graph_fx1002[6] = {
   0.22,
   0.31,
   0.31,
   0.98,
   8.53,
   390};
   Double_t Graph_fy1002[6] = {
   3.67532,
   7.02963,
   5.71187,
   7.36742,
   12.77335,
   15.34901};
   Double_t Graph_fex1002[6] = {
   0.022,
   0.031,
   0.031,
   0.098,
   0.06824,
   3.12};
   Double_t Graph_fey1002[6] = {
   0.62754,
   0.9026,
   0.82169,
   0.8698,
   0.57146,
   0.6859};
   gre = new TGraphErrors(6,Graph_fx1002,Graph_fy1002,Graph_fex1002,Graph_fey1002);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillColor(4);
   gre->SetFillStyle(1000);
   gre->SetLineColor(4);
   gre->SetMarkerColor(4);
   gre->SetMarkerStyle(34);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_Graph1002 = new TH1F("Graph_Graph1002","",100,0.1782,432.4122);
   Graph_Graph1002->SetMinimum(1.749067);
   Graph_Graph1002->SetMaximum(17.33362);
   Graph_Graph1002->SetDirectory(0);
   Graph_Graph1002->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1002->SetLineColor(ci);
   Graph_Graph1002->GetXaxis()->SetLabelFont(42);
   Graph_Graph1002->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1002->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1002->GetXaxis()->SetTitleFont(42);
   Graph_Graph1002->GetYaxis()->SetLabelFont(42);
   Graph_Graph1002->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1002->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1002->GetYaxis()->SetTitleOffset(0);
   Graph_Graph1002->GetYaxis()->SetTitleFont(42);
   Graph_Graph1002->GetZaxis()->SetLabelFont(42);
   Graph_Graph1002->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1002->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1002);
   
   multigraph->Add(gre,"");
   multigraph->Draw("AP");
   multigraph->GetXaxis()->SetTitle("Dose rate (krad/hr)");
   multigraph->GetXaxis()->SetLabelFont(42);
   multigraph->GetXaxis()->SetLabelSize(0.035);
   multigraph->GetXaxis()->SetTitleSize(0.035);
   multigraph->GetXaxis()->SetTitleOffset(1.3);
   multigraph->GetXaxis()->SetTitleFont(42);
   multigraph->GetYaxis()->SetTitle("Dose Constant (Mrad)");
   multigraph->GetYaxis()->SetLabelFont(42);
   multigraph->GetYaxis()->SetLabelSize(0.035);
   multigraph->GetYaxis()->SetTitleSize(0.035);
   multigraph->GetYaxis()->SetTitleOffset(1.2);
   multigraph->GetYaxis()->SetTitleFont(42);
   
   TLegend *leg = new TLegend(0.57,0.22,0.89,0.42,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("Graph","EJ200PVT-1X1P","p");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(2);
   entry->SetMarkerStyle(33);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph","EJ260PVT-1X1P","p");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(4);
   entry->SetMarkerStyle(34);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   leg->Draw();
   canv->Modified();
   canv->cd();
   canv->SetSelected(canv);
}
