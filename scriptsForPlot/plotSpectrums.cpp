double beginPoint(TH1F* Hist)
{
  double y_prev, y_curr, y_next;
  double x_valley;
  
  Hist->Smooth(2);
  
  for (int i = 2; i < Hist->GetNbinsX() -1 ; i++)
  {
    double x = Hist->GetBinCenter(i);
    if (x < 400 || x > 1100) continue;

    y_prev = Hist->GetBinContent(i - 1);
    y_curr = Hist->GetBinContent(i);
    y_next = Hist->GetBinContent(i + 1);

    if (y_curr < y_prev && y_curr < y_next) {
      x_valley = x;
      std::cout << "!!found!!" << std::endl;
      std::cout << x_valley << ", "<< y_curr - y_prev << std::endl;
    } 
  }
  std::cout << "Valley at x = " << x_valley << ", y = " << y_curr << std::endl;
  return x_valley;
}


int plotSpectrums(){
  TFile *inputFile = new TFile("/home/li_s3/mythen/aare-submodule/build/Output/Spectrum_-40deg_testNoSquare.root");

  TCanvas* c1 = new TCanvas("c1","c1",600,500);
  gStyle->SetOptStat(0);

  TLegend* legend = new TLegend(0.6, 0.4, 0.8, 0.8);
  int step = 0;
  int color = 1;
  double valleyPoint;

  
  
  for (TObject* keyAsObj : *inputFile->GetListOfKeys()){
    auto key = dynamic_cast<TKey*>(keyAsObj);
    TString EnergyPath = key->GetName();
    TString Enpoint = EnergyPath(2, 4);
    //if (Enpoint.Atoi() == 1900) break;
    std::cout << Enpoint.Atoi() << std::endl;
 
    TH1F* h = (TH1F*)inputFile->Get(EnergyPath+"/h2");
    TH1F *hist = (TH1F*)h->Clone();
    
    valleyPoint = beginPoint(hist);
    if (Enpoint.Atoi() >= 1900) step = 1000;
    TF1 *fitgaus = new TF1("fitgaus", "gaus", 2000+step, 6000+step);
    auto result= h->Fit(fitgaus,"SRN");

    std::cout << result->Parameter(1) << std::endl;
    std::cout << result->Parameter(2)*3 + result->Parameter(1)<< std::endl;

    h->Scale(1/h->Integral(h->GetXaxis()->FindBin(valleyPoint), h->GetXaxis()->FindBin(result->Parameter(1)+result->Parameter(2)*3)));
    h->GetXaxis()->SetRangeUser(valleyPoint-10, 8000);
    h->Draw("same, hist");
    h->SetLineColor(color);
    //h->SetMaximum(0.2);
    legend->AddEntry(h, Enpoint+" eV", "l");
    color++;
    if (color == 10 or color == 5) color++;
    //h->Delete();
  }
  legend->SetBorderSize(0);
  legend->Draw("same");
  return 0;
}
