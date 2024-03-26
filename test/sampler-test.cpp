#include "geomu/GridSampler2D.h"
#include <cstdio>

using namespace std;

int main()
{
  vector<double> grid_x = { 0.0, 1.0, 2.0};
  vector<double> grid_y = {-2.0, 0.0, 2.0};
  vector<vector<double>> grid = {
    {0.0, 1.0, 2.0},
    {3.0, 4.0, 5.0},
    {6.0, 7.0, 8.0},
  };

  GridSampler2D sampler;
  sampler.SetGrid(grid_x, grid_y, grid);
  for(size_t i = 0; i < 1000000; ++i) {
    double x, y;
    double weight = sampler.Sample(&x, &y);
    printf("%.3lf\t%.3lf\t%.3lf\n", x, y, weight);
  }
  return 0;
}
