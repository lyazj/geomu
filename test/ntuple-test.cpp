#include "geomu/NtpMCTupler.h"
#include <Framework/Ntuple/NtpMCEventRecord.h>

using namespace std;
using namespace genie;

#include "geomu/diagnostic.h"

class MyNtpMCTupler : public NtpMCTupler {

private:
  virtual void ProcessOneEvent() override;

};

int main(int argc, char *argv[])
{
  if(argc < 2) {
    cerr << "usage: " << program_invocation_short_name << " <infile> [ <infile> ... ]" << endl;
    return 1;
  }

  MyNtpMCTupler tupler;
  for(int i = 1; i < argc; ++i) {
    tupler.AddFile(argv[i]);
  }
  tupler.Process();

  return 0;
}

void MyNtpMCTupler::ProcessOneEvent()
{
  EventRecord *event = GetCurrentEvent()->event;
  difo << *event << dend;
  NtpMCTupler::ProcessOneEvent();
}
