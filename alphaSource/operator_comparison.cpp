{
  TCanvas *c1 = new TCanvas("c1");
  c1->SetLeftMargin(0.20);
  Int_t n = 14;
  TH2F *frame = new TH2F("frame","",10,4.78,4.94,n,0,n);
  frame->SetStats(0);
  frame->GetXaxis()->SetTitle("Peak");
  frame->GetYaxis()->SetLabelSize(0.08);
  frame->GetYaxis()->SetBinLabel(1,"Ruhi 1");
  frame->GetYaxis()->SetBinLabel(2,"Ruhi 2");
  frame->GetYaxis()->SetBinLabel(3,"Claire 1");
  frame->GetYaxis()->SetBinLabel(4,"Chris 1");
  frame->GetYaxis()->SetBinLabel(5,"Yingyue 1");
  frame->GetYaxis()->SetBinLabel(6,"Ruhi 3");
  frame->GetYaxis()->SetBinLabel(7,"Duan 1");
  frame->GetYaxis()->SetBinLabel(8,"Chris 2");
  frame->GetYaxis()->SetBinLabel(9,"Chris 3");
  frame->GetYaxis()->SetBinLabel(10,"Claire 2");
  frame->GetYaxis()->SetBinLabel(11,"Chris 4");
  frame->GetYaxis()->SetBinLabel(12,"Yingyue 2");
  frame->GetYaxis()->SetBinLabel(13,"Yingyue 3");
  frame->GetYaxis()->SetBinLabel(14,"Chris 5");
  frame->Draw();
  TGraphErrors *graph = new TGraphErrors(n);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.2);

  Double_t data[14] = {4.838740825653076, 4.839351177215576, 4.881603717803955, 4.821920394897461,
     4.887848854064941, 4.862770080566406, 4.8589935302734375, 4.855875492095947,
     4.896286964416504, 4.866230487823486, 4.827983379364014, 4.828577041625977,
     4.881960391998291, 4.83397102355957};
  Double_t err[14] = {0.007249431684613228, 0.005922745447605848, 0.007823324762284756, 0.006953560281544924,
     0.008789995685219765, 0.005842468235641718, 0.006121148820966482, 0.008886133320629597,
     0.006398038938641548, 0.005619598086923361, 0.008749286644160748, 0.0061925677582621574,
     0.01247414667159319, 0.006209648214280605};

  Double_t temp = 0;
  for (Int_t i = 0; i < n; ++i) {
    temp += data[i];
  }
  Double_t temp2 = 0;
  for (Int_t i = 0; i < n; ++i) {
    temp2 += pow(data[i]-temp/n,2);
  }
  Double_t stdev = pow(temp2/((n-1)),0.5);
  TBox *box = new TBox(temp/n-stdev,0,temp/n+stdev,n);
  box->SetFillColor(16);
  box->Draw();

  for (Int_t i = 0; i < n; ++i) {
    graph->SetPoint(i,data[i],i+0.5);
    graph->SetPointError(i,err[i],0);
  }
  graph->Draw("p");
  TLine line;
  line.SetLineStyle(2);
  line.DrawLine(temp/n,0,temp/n,n);

  //line.DrawLine(4.84,0, 4.84,n);
  c1->RedrawAxis();
}
