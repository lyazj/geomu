#include "geomu/NtpMCTupler.h"
#include <iostream>

using namespace std;
using namespace genie;

int main(int argc, char *argv[])
{
  if(argc == 1) {
    cerr << "usage: " << program_invocation_short_name << " <rootfile> [ <rootfile> ... ]" << endl;
    return 1;
  }

  NtpMCTupler tupler;
  for(int i = 1; i < argc; ++i) {
    tupler.AddFile(argv[i]);
  }
  tupler.Process();

  return 0;
}
