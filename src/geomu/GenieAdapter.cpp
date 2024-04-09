#include "geomu/GenieAdapter.h"
#include <stdlib.h>
#include <string>

using namespace std;

GenieAdapter::GenieAdapter()
{
  [[maybe_unused]] static int adapted = (Adapt(), 1);
}

void GenieAdapter::Adapt()
{
  // $GENIE
  string genie = getenv("GENIE") ? : "";
  if(genie.empty()) genie = GENIE;
  setenv("GENIE", genie.c_str(), 1);

  // $CPLUS_INCLUDE_PATH
  string incdir = getenv("CPLUS_INCLUDE_PATH") ? : "";
  if(!incdir.empty()) incdir += ":";
  incdir += genie;
  incdir += "/src";
  setenv("CPLUS_INCLUDE_PATH", incdir.c_str(), 1);
}
