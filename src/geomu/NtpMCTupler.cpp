#include "geomu/NtpMCTupler.h"
#include <TChain.h>
#include <TFile.h>
#include <Framework/Ntuple/NtpMCTreeHeader.h>
#include <Framework/Ntuple/NtpMCEventRecord.h>

using namespace genie;

#include "geomu/diagnostic.h"

NtpMCTupler::NtpMCTupler()
{
  chain_ = new TChain("gtree");
  file_ = nullptr;
  header_ = nullptr;
  event_ = nullptr;
  chain_->SetBranchAddress("gmcrec", &event_);

  ifile_ = -1;
  nfile_ = 0;
  ievent_ = -1;
}

NtpMCTupler::~NtpMCTupler()
{
  delete chain_;
}

void NtpMCTupler::AddFile(const char *path)
{
  chain_->AddFile(path);
  ++nfile_;
}

void NtpMCTupler::Process()
{
  BeginProcess();
  while(ReadNextEvent()) {
    ProcessOneEvent();
  }
  EndProcess();
}

size_t NtpMCTupler::GetEventNumber() const
{
  return chain_->GetEntries();
}

void NtpMCTupler::ProcessOneEvent()
{
  if((ievent_ + 1) % 1000 == 0) {
    difo << (ievent_ + 1) << " events processed" << dend;
  }
  ddbg << *event_ << dtrm;
}

void NtpMCTupler::BeginProcess()
{
  difo << "Begin processing" << dend;
}

void NtpMCTupler::EndProcess()
{
  difo << "End processing, " << GetProcessedEventNumber() << " events processed" << dend;
}

void NtpMCTupler::OnNewFile()
{
  difo << "Switching to new file: [" << GetCurrentFileIndex() << "] "
       << GetCurrentFile()->GetName() << "\n" << *header_ << dtrm;
}

bool NtpMCTupler::ReadNextEvent()
{
  int bytes = chain_->GetEntry(ievent_ + 1);
  if(!bytes) {
    file_ = nullptr;
    header_ = nullptr;
    return false;
  }

  ++ievent_;
  if(chain_->GetTree()->GetReadEntry() == 0) {
    file_ = chain_->GetFile();
    header_ = (NtpMCTreeHeader *)file_->Get("header");
    ++ifile_;
    OnNewFile();
  }
  return true;
}
