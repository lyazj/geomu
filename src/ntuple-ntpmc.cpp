#include "geomu/NtpMCTupler.h"
#include <Framework/Ntuple/NtpMCEventRecord.h>
#include <Framework/EventGen/EventRecord.h>
#include <Framework/GHEP/GHepParticle.h>
#include <Framework/ParticleData/PDGCodes.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>

using namespace std;
using namespace genie;

#include "geomu/diagnostic.h"

class MyNtpMCTupler : public NtpMCTupler {

public:
  MyNtpMCTupler(const char *outpath);
  ~MyNtpMCTupler();

private:
  TFile *file_;
  TTree *tree_;

  double weight_;
  int neutrino_pid_;
  TLorentzVector *neutrino_p4_ = nullptr;
  TLorentzVector *neutrino_x4_ = nullptr;
  int lepton_pid_;
  TLorentzVector *lepton_p4_ = nullptr;
  TLorentzVector *lepton_x4_ = nullptr;

  virtual void ProcessOneEvent() override;
  void TranscriptOneEvent();

};

int main(int argc, char *argv[])
{
  if(argc < 3) {
    cerr << "usage: " << program_invocation_short_name << " <outfile> <infile> [ <infile> ... ]" << endl;
    return 1;
  }

  MyNtpMCTupler tupler(argv[1]);
  for(int i = 2; i < argc; ++i) {
    tupler.AddFile(argv[i]);
  }
  tupler.Process();

  return 0;
}

MyNtpMCTupler::MyNtpMCTupler(const char *outpath)
{
  file_ = new TFile(outpath, "NEW");
  if(!file_->IsOpen()) throw runtime_error(string("error opening output file: ") + outpath);
  tree_ = new TTree("tree", "tree");

  tree_->Branch("weight", &weight_, "weight/D");
  tree_->Branch("neutrino_pid", &neutrino_pid_, "neutrino_pid/I");
  tree_->Branch("neutrino_p4", &neutrino_p4_);
  tree_->Branch("neutrino_x4", &neutrino_x4_);
  tree_->Branch("lepton_pid", &lepton_pid_, "lepton_pid/I");
  tree_->Branch("lepton_p4", &lepton_p4_);
  tree_->Branch("lepton_x4", &lepton_x4_);
}

MyNtpMCTupler::~MyNtpMCTupler()
{
  tree_->Write(nullptr, tree_->kOverwrite);
  delete tree_;
  delete file_;
}

void MyNtpMCTupler::ProcessOneEvent()
{
  TranscriptOneEvent();
  NtpMCTupler::ProcessOneEvent();
}

void MyNtpMCTupler::TranscriptOneEvent()
{
  int abs_pdg;

  // Fetch current event.
  EventRecord *event = GetCurrentEvent()->event;
  Int_t nparticle = event->GetEntries();
  if(!nparticle) {
    dwrn << "Skipping empty event: " << GetCurrentEventIndex() << dend;
    return;
  }

  // Fetch incident neutrino.
  GHepParticle *neutrino = (GHepParticle *)event->UncheckedAt(0);
  abs_pdg = abs(neutrino->Pdg());
  if(abs_pdg != kPdgNuE && abs_pdg != kPdgNuMu) {
    derr << "Unexpected incident particle: " << neutrino->Pdg() << dend;
    return;
  }

  // Fetch produced charged lepton.
  int fd = neutrino->FirstDaughter(), ld = neutrino->LastDaughter();
  if(fd >= 0) for(int d = fd; d <= ld; ++d) {
    GHepParticle *lepton = (GHepParticle *)event->At(d);
    abs_pdg = abs(lepton->Pdg());
    if(abs_pdg != kPdgElectron && abs_pdg != kPdgMuon) continue;

    // Apply filters on this event.
    if(lepton->Pz() < 0.0) {
      ddbg << "Skipping event with negative charged lepton pz (" << lepton->Pz() << "):\n" << *event << dtrm;
      return;
    }

    // Transcript this event.
    weight_ = event->Weight();
    neutrino_pid_ = neutrino->Pdg();
    neutrino_p4_ = neutrino->P4();
    neutrino_x4_ = neutrino->X4();
    lepton_pid_ = lepton->Pdg();
    lepton_p4_ = lepton->P4();
    lepton_x4_ = lepton->X4();
    tree_->Fill();
    return;
  }
  ddbg << "Skipping event with no charged lepton produced:\n" << *event << dtrm;
}
