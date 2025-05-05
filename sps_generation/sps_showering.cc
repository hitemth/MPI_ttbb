// sps_showering.cc
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
using namespace Pythia8;

int main() {
  // --- 1) Initialize Pythia to read LHEF from MG5_aMC@NLO ---
  Pythia pythia;
  pythia.readString("Beams:frameType = 4");        // external LHE file
  pythia.readString("Beams:LHEF = events.lhe");
  pythia.init();

  // --- 2) Prepare ROOT output ---
  TFile  fOut("sps_ttbb_showered.root", "RECREATE");
  TTree* tree = new TTree("Events", "Showered SPS ttbb");

  std::vector<float>  px, py, pz, E;
  std::vector<int>    id;

  tree->Branch("px", &px);
  tree->Branch("py", &py);
  tree->Branch("pz", &pz);
  tree->Branch("E",  &E);
  tree->Branch("id", &id);

  // --- 3) Loop over events and fill the tree ---
  const int nEvents = 10000;
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next()) continue;

    px.clear();  py.clear();
    pz.clear();  E.clear();
    id.clear();

    // collect all final-state particles
    for (int i = 0; i < pythia.event.size(); ++i) {
      const Particle& p = pythia.event[i];
      if (!p.isFinal()) continue;
      px.push_back(p.px());
      py.push_back(p.py());
      pz.push_back(p.pz());
      E .push_back(p.e());
      id.push_back(p.id());
    }

    tree->Fill();
  }

  // --- 4) Finalize ---
  tree->Write();
  fOut.Close();
  pythia.stat();

  return 0;
}
