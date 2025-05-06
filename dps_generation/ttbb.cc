#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
using namespace Pythia8;

int main() {
  Pythia pythia;

  // First hard process: bb̄ production
  pythia.readString("HardQCD:gg2bbbar = on");
  pythia.readString("HardQCD:qqbar2bbbar = on");

  // Second hard process: tt̄ production
  pythia.readString("SecondHard:generate = on");
  pythia.readString("SecondHard:TopPair = on");

  // Beam energy
  pythia.readString("Beams:eCM = 13000.");

  // Initialize
  if (!pythia.init()) return 1;

  // ROOT output file and tree
  TFile* outFile = new TFile("ttbb.root", "RECREATE");
  TTree* tree = new TTree("events", "ttbb events");

  // Variables to store per-particle info
  int id;
  float px, py, pz, e;
  tree->Branch("id", &id, "id/I");
  tree->Branch("px", &px, "px/F");
  tree->Branch("py", &py, "py/F");
  tree->Branch("pz", &pz, "pz/F");
  tree->Branch("e",  &e,  "e/F");

  // Event loop
  for (int i = 0; i < 10000; ++i) {
    if (!pythia.next()) continue;

    for (int j = 0; j < pythia.event.size(); ++j) {
      if (!pythia.event[j].isFinal()) continue;

      id = pythia.event[j].id();
      px = pythia.event[j].px();
      py = pythia.event[j].py();
      pz = pythia.event[j].pz();
      e  = pythia.event[j].e();

      tree->Fill();
    }
  }

  // Finalize
  tree->Write();
  outFile->Close();
  delete outFile;

  pythia.stat();
  return 0;
}


