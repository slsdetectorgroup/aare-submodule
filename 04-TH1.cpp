#include <fmt/color.h>
#include <fmt/format.h>
#include <numeric>

#include <filesystem>

#include "aare/ClusterFile.hpp"
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TFile.h>
#include <THStack.h>
#include <iostream>
#include <fstream>
#include <TROOT.h>
#include <TSystem.h>
#include "tiffIO.h"

namespace fs = std::filesystem;


bool endsWith(const std::string& filename, const std::string& suffix) {
    if (suffix.size() > filename.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), filename.rbegin());
}

std::vector<fs::path> getFilesWithSuffix(const fs::path& folder, const std::string& suffix) {
    std::vector<fs::path> result;

    if (fs::exists(folder) && fs::is_directory(folder)) {
        for (const auto& entry : fs::directory_iterator(folder)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (endsWith(filename, suffix)) {
                    result.push_back(entry.path());
                }
            }
        }
    } else {
        std::cerr << "Error: Folder does not exist or is not a directory." << std::endl;
    }

    return result;
}

std::vector<std::string> readFileToVector(const std::string& filename)
{
    std::ifstream source;
    source.open(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(source, line))
    {
        lines.push_back(line);
    }
    return lines;
}

TH2F *readTiff(const char *fname) {
  float *img;
  uint32_t nr = 400, nc = 400;
  img=ReadFromTiff(fname, nr, nc);
  TH2F *h=NULL;
  if (img) {
    h = new TH2F("h", fname, nc, 0, nc, nr, 0, nr);
    for (int ir = 0; ir < nr; ir++) {
      for (int ic = 0; ic < nc; ic++) {
	h->SetBinContent(ic+1, ir+1, sqrt(img[ic+nc*ir]));
	//if (img[ic+nc*ir] < 100) std::cout << img[ic+nc*ir] << std::endl;
         //h2->SetBinContent(ic+1, ir+1, img[ic+nc*ir]);
      }
    }
    delete [] img;   
  }
  return h;
}

int main(int argc, char const *argv[]) {
  
    if (argc < 2) {
        fmt::print(fg(fmt::terminal_color::magenta), "Usage: {} <filename>\n",  argv[0]);
        return 1;
    }

    std::string InputFile(argv[1]);
    std::vector<std::string> dataFile = readFileToVector(InputFile);
   
    size_t Tempos = dataFile[0].find("deg");
    std::string degPoint = dataFile[0].substr(Tempos - 3, 3);
    
    TFile* OutFile = new TFile("Output/Spectrum_"+TString(degPoint)+"deg_testNoSquare.root", "RECREATE");
    OutFile->cd();

    int n_bins = 1000;
    double xmin = 0;
    double xmax = 15000;
    size_t chunk_size = 1000;

    int32_t t2;
    int32_t t3;
    char quad;
    double eta2x;
    double eta2y;
    double eta3x;
    double eta3y;

    for (int j = 0; j < dataFile.size(); j++)
    {
      size_t Enpos = dataFile[j].find("En");
      std::string EnergyPoint = dataFile[j].substr(Enpos+2, 6);
      std::cout << "Energy point: " << EnergyPoint << std::endl;
      TDirectory *energyDir = OutFile->mkdir("En"+TString(EnergyPoint));
      energyDir->cd();

      
      std::string folderPath = dataFile[j];
      auto clustFile = getFilesWithSuffix(folderPath, ".clust");
      auto noiseFile = getFilesWithSuffix(folderPath, "_var.tiff");
      char *noiseFilePath = std::string(noiseFile[0]).data();
      
      std::cout << "Reading info from: " << clustFile[0] << std::endl << noiseFile[0] << std::endl;

      aare::ClusterFile f(clustFile[0]);
    
      /*
      int32_t iframe2 = 7512528;
      auto clusters2 = f.read_frame(iframe2);
      std::cout << "clusters2.size() = " << clusters2.size() << std::endl; 
      */
      
      //TH2F *hNoise = new TH2F("hNoise", "", 400, -0.5, 399.5, 400, -0.5, 399.5); //Noise
      TH2F*hNoise;
      hNoise= readTiff(std::string(noiseFile[0]).data());
      hNoise->SetName("hNoise");
       
      TH2F *hImage = new TH2F("hImage", "", 400, 0, 400, 400, 0, 400); //Image     
      TH1D *h1 = new TH1D("h1", "", n_bins, xmin, xmax); //1x1 clusters
      TH1D *h2 = new TH1D("h2", "", n_bins, xmin, xmax); //2x2 clusters     
      TH1D *h3 = new TH1D("h3", "", n_bins, xmin, xmax); //3x3 clusters
      TH2F *hChannel = new TH2F("hChannel", "", 1000, 0, 15000, 400*400, 0, 400*400);
      
      // Loop over all clusters in the file and fill the histogram with the sum of
      // all pixel values in the cluster
      int numm = 0;
      for (auto clusters = f.read_clusters(chunk_size); !clusters.empty(); clusters = f.read_clusters(chunk_size)){
      //for (auto clusters = f.read_clusters2(); !clusters.empty(); clusters = f.read_clusters2()){ 
	numm++;
	if (numm%100000 == 0) std::cout << "numm = " << numm << std::endl;
	if (numm > 10*1E5) break;
	for (const auto &cluster : clusters) {
	  f.analyze_cluster(cluster, &t2, &t3, &quad, &eta2x, &eta2y, &eta3x, &eta3y);
          if (cluster.x >= 0 && cluster.x < 400 && cluster.y >= 1 && cluster.y < 400){
            if (t2 > 2*5*hNoise->GetBinContent(cluster.x+1, cluster.y+1) && t2 > 0.9*cluster.data[4]){
	  
	      hImage->Fill(cluster.x, cluster.y);	  
              h1->Fill(cluster.data[4]);
	      h2->Fill(t2);
	      h3->Fill(t3);
	      hChannel->Fill(t2, cluster.x + 400*cluster.y);	  
	    }
	  }
	  /*
	  std::cout << "t2 = " << t2 << ", t3 = " << t3 << std::endl;
	  std::cout << "eta2x = " << eta2x << ", eta3y = " << eta3y << std::endl;
	  std::cout << "quad = " << quad << std::endl;
          */	  
         }
      }
      
      hNoise->Write();
      hImage->Write();
      h1->Write();
      h2->Write();
      h3->Write();
      hChannel->Write();   
    }
    //OutFile->Write();
    OutFile->Close();

    //TString EnergyVar[] = ["1100", "1300", "1500", "1500", "1600", "1700", "1800", "1900", "2000", "2100"];
    //TFile *f1 = TFile::Open("hsimple.root");
    //OutFile->Open();
    /*
    TIter keyList(OutFile->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)keyList())) {
      TClass *cl = gROOT->GetClass(key->GetClassName())
      std::cout << "aaa" << std::endl;
    }
 */
    return 0;
}
