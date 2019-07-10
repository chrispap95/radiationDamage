{
  #include <iostream>
  #include <vector>

  ifstream fin;
  fin.open("peak_data.dat");
  std::vector<float> data;
  std::vector<float> err;
  std::vector<string> name;
  std::vector<string> date;

  while (!fin.eof()) {
    string temp_name, temp_date;
    float temp_data, temp_err;
    fin >> temp_name >> temp_date >> temp_data >> temp_err;
    name.push_back(temp_name);
    date.push_back(temp_date);
    data.push_back(temp_data);
    err.push_back(temp_err);
  }

  TCanvas *c1 = new TCanvas("c1");
  c1->SetLeftMargin(0.20);
  Int_t n = name.size();
  TH2F *frame = new TH2F("frame","",10,4.78,4.94,n,0,n);
  frame->SetStats(0);
  frame->GetXaxis()->SetTitle("Peak");
  frame->GetYaxis()->SetLabelSize(0.08);
  for (Int_t i = 1; i <= n; ++i) {
    string label = name[i-1]+" "+date[i-1];
    char char_arr[label.length()];
    strcpy(char_arr, label.c_str());
    frame->GetYaxis()->SetBinLabel(i,char_arr);
  }

  frame->Draw();
  TGraphErrors *graph = new TGraphErrors(n);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.2);

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
  c1->Print("Results/11092018/comparison.pdf");
  c1->Print("Results/11092018/comparison.eps");
  c1->Print("Results/11092018/comparison.png");
}
