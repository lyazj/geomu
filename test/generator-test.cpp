#include "geomu/BartolPM14Generator.h"
#include <cstdio>

using namespace std;

int main()
{
  BartolPM14Generator generator;
  for(size_t i = 0; i < 1000000; ++i) {
    bool anti;
    double energy, cos_theta;
    double weight = generator.Generate(&energy, &cos_theta, &anti);
    printf("%.3lf\t%.3lf\t%.3lf\t%d\n", energy, cos_theta, weight, (int)anti);
  }
  return 0;
}
