{
  #include <iostream>
  #include <vector>

  ifstream fin;
  fin.open("peak_data_sorted.dat");
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
  TH2F *frame = new TH2F("frame",";Date;Peak",n,0,n,10,4.78,5.1);
  frame->SetStats(0);
  for (Int_t i = 1; i <= n; ++i) {
    string label = date[i-1];
    char char_arr[label.length()];
    strcpy(char_arr, label.c_str());
    char char_arr2[label.length()-3];
    Bool_t temp = 1;
    for (Int_t j = 0; j < 5; ++j) {
      if (j == 2 && temp) {
        char_arr2[j] = '/';
        temp = 0;
        j = 1;
        continue;
      }
      if(!temp) {
        char_arr2[j+1] = char_arr[j+4];
        continue;
      }
      char_arr2[j] = char_arr[j+4];
    }
    frame->GetXaxis()->SetBinLabel(i,char_arr2);
  }

  frame->Draw();
  TGraphErrors *graph = new TGraphErrors(n);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.2);

  for (Int_t i = 0; i < n; ++i) {
    graph->SetPoint(i,i+0.5,data[i]);
    graph->SetPointError(i,0,err[i]);
  }
  graph->Draw("p");

  //line.DrawLine(4.84,0, 4.84,n);
  c1->RedrawAxis();
  c1->Print("Results/20181201/stdCheck.pdf");
  c1->Print("Results/20181201/stdCheck.eps");
  c1->Print("Results/20181201/stdCheck.png");
}
