#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TVector3.h"

#include "RAT/DSReader.hh"
#include "RAT/DS/MC.hh"

//Compile with:
// g++ -o analyzeRUT analyzeRUT.cc `root-config --cflags --glibs` -I/home/jrlowery/jsns2/ratpac-kpipe/include
//

int main( int nargs, char** argv ) {

  if ( nargs < 4 ) {
    std::cout << "usage: analyze_data <input RAT root file> <output root file> <verbose (0 or 1)>" << std::endl;
    return 0;
  }

  std::string inputfile = argv[1];
  std::string outfile = argv[2];
  std::string verboseString = argv[3];
  bool verbose = verboseString != "0";

  RAT::DSReader* ds = new RAT::DSReader( inputfile.c_str() );
  TFile* out = new TFile(outfile.c_str(), "RECREATE" );

  TH1D* wavelengthHist = new TH1D("wavelengthHist", "", 100, 200, 1000);
  TH1D* xPosHist = new TH1D("xPos", "", 200, -55, 55);
  TH1D* yPosHist = new TH1D("yPos", "", 200, -55, 55);
  TH1D* zPosHist = new TH1D("zPos", "", 200, -1100, 1100);

  int numEvents = ds->GetTotal();
  std::cout << "Number of events: " << numEvents << std::endl;

  for (int i = 0; i < numEvents; ++i) {

    RAT::DS::Root* root = ds->NextEvent();

    RAT::DS::MC* mc = root->GetMC();
    if ( mc==NULL ) {
      std::cout << "mc==NULL" << std::endl;
      break;
    }

    if ( mc->GetMCParticleCount()==0 ) {
      std::cout << "ParticleCount==0" << std::endl;
      continue;
    }

    int numPMT = mc->GetMCPMTCount();
    int numPE = mc->GetNumPE();

    //Skip events that do not have hits
    if (numPMT == 0) continue;

    //Otherwise, get the PMT and number of hits
    RAT::DS::MCPMT* PMT = mc->GetMCPMT(0);
    int numHits = PMT->GetMCPhotonCount();
    
    if (verbose) {
      std::cout << "========================================" << std::endl;
      std::cout << "Event: " << i << std::endl;
      std::cout << "Number of Hits: " << numHits << std::endl;
    }

    for (int iHit = 0; iHit < numHits; iHit++) {
      RAT::DS::MCPhoton* hit = PMT->GetMCPhoton( iHit );
      double tHit = hit->GetHitTime();
      double wavelen = hit->GetLambda() * 1.0e6;  //Convert from mm to nm
      wavelengthHist->Fill(wavelen);
      TVector3 hitPosition = hit->GetPosition();
      xPosHist->Fill(hitPosition.X()); 
      yPosHist->Fill(hitPosition.Y()); 
      zPosHist->Fill(hitPosition.Z());
      TVector3 hitMomentum = hit->GetMomentum();

      if (verbose) {
	std::cout << "\nHit Time: " << tHit << "\tWavelength: " << wavelen << std::endl;
	std::cout << "Hit Position: (" << hitPosition.X() << ", " << hitPosition.Y() << ", " << hitPosition.Z() << ")" << " in mm" << std::endl;
	std::cout << "Hit Momentum: (" << hitMomentum.X() << ", " << hitMomentum.Y() << ", " << hitMomentum.Z() << ")" << " in MeV/c" << std::endl;
      }
    }
    if (verbose) std::cout << "========================================\n" << std::endl;
  } 

  wavelengthHist->GetXaxis()->SetTitle("Wavelength (nm)");
  xPosHist->GetXaxis()->SetTitle("x position (mm)");
  yPosHist->GetXaxis()->SetTitle("y position (mm)");
  zPosHist->GetXaxis()->SetTitle("z position (mm)");
  wavelengthHist->Write();
  xPosHist->Write();
  yPosHist->Write();
  zPosHist->Write();
  out->Close();

  std::cout << "Finished." << std::endl;

  return 0;
}
