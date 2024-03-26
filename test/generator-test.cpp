#include "geomu/BartolPM14Generator.h"
#include <cstdio>

using namespace std;

int main()
{
  BartolPM14Generator generator;
  generator.SetTargetNevent(0, 100000);
  for(size_t i = 0; i < 10000000; ++i) {
    bool anti;
    double energy, cos_theta;
    double weight = generator.Generate(&energy, &cos_theta, &anti);
    printf("%.3le\t%.3le\t%.3le\t%d\n", energy, cos_theta, weight, (int)anti);
  }
  return 0;
}
