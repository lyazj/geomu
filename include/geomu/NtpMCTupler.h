#pragma once
#include <stddef.h>

class TChain;
class TFile;
namespace genie { class NtpMCTreeHeader; class NtpMCEventRecord; }

class NtpMCTupler {

public:
  NtpMCTupler();
  virtual ~NtpMCTupler();
  NtpMCTupler(const NtpMCTupler &) = delete;

  void AddFile(const char *path);
  void Process();

  size_t GetEventNumber() const;
  size_t GetProcessedEventNumber() const { return ievent_ + 1; }

protected:
  virtual void ProcessOneEvent();
  virtual void BeginProcess();
  virtual void EndProcess();
  virtual void OnNewFile();

  TChain *GetChain() const { return chain_; }
  TFile *GetCurrentFile() const { return file_; }
  genie::NtpMCTreeHeader *GetCurrentHeader() const { return header_; }
  genie::NtpMCEventRecord *GetCurrentEvent() const { return event_; }
  size_t GetCurrentFileIndex() const { return ifile_; }
  size_t GetFileNumber() const { return nfile_; }
  size_t GetCurrentEventIndex() const { return ievent_; }

private:
  TChain *chain_;  // owned
  TFile *file_;  // owned by chain_, maintained by ReadNextEvent()
  genie::NtpMCTreeHeader *header_;  // owned by chain_, maintained by ReadNextEvent()
  genie::NtpMCEventRecord *event_;  // owned and maintained by chain_

  size_t ifile_;  // maintained by ReadNextEvent()
  size_t nfile_;  // maintained by AddFile()
  size_t ievent_;  // maintained by ReadNextEvent()

  bool ReadNextEvent();

};
