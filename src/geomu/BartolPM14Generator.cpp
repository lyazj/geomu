#include "geomu/BartolPM14Generator.h"
#include "geomu/GridSampler2D.h"
#include "geomu/random.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifndef BARTOLPM14_DATAPATH
#define BARTOLPM14_DATAPATH  PROJECT_BASEDIR "/data/Bartol_pm14.tsv"
#endif  /* BARTOLPM14_DATAPATH */

using namespace std;

BartolPM14Generator::BartolPM14Generator() : zero_one_distribution_(0, 1)
{
  sampler_ = new GridSampler2D[2];
  LoadData();
}

BartolPM14Generator::~BartolPM14Generator()
{
  delete[] sampler_;
}

double BartolPM14Generator::Generate(double *energy, double *cos_theta, bool *anti)
{
  bool anti_ = ZeroOrOne();
  double log_energy_, cos_theta_;
  double weight;
  if(!sampler_[anti_].SmartSample(&log_energy_, &cos_theta_, &weight)) {
    weight = sampler_[anti_].EvenlySample(&log_energy_, &cos_theta_);  // more than expected
  }
  *energy = exp(log_energy_), *cos_theta = cos_theta_, *anti = anti_;
  return weight;
}

void BartolPM14Generator::LoadData()
{
  vector<double> log_energy, cos_theta;
  vector<vector<double>> yields[2];
  ifstream datafile(BARTOLPM14_DATAPATH);
  if(!datafile) throw runtime_error("error opening datafile: "s + BARTOLPM14_DATAPATH);
  string line;

  // Fetch cos_theta values.
  if(getline(datafile, line)) {
    istringstream stream(line);
    double value;
    while(stream >> value) cos_theta.push_back(value);
  }

  while(getline(datafile, line)) {
    // Fetch energy value.
    istringstream stream(line);
    double value;
    if(stream >> value) log_energy.push_back(log(value));

    // Fetch yield densities.
    for(size_t i = 0; i < 2; ++i) yields[i].emplace_back();
    for(;;) {
      for(size_t i = 0; i < 2; ++i) {
        if(!(stream >> value)) goto END;
        yields[i].back().push_back(value * (2 * M_PI));
      }
    }
    END: ;
  }

  // cos_theta must increase.
  reverse(cos_theta.begin(), cos_theta.end());
  for(size_t i = 0; i < 2; ++i) {
    for(size_t j = 0; j < yields[i].size(); ++j) {
      reverse(yields[i][j].begin(), yields[i][j].end());
    }
  }

  for(size_t i = 0; i < 2; ++i) sampler_[i].SetGrid(log_energy, cos_theta, yields[i]);
}

bool BartolPM14Generator::ZeroOrOne()
{
  return zero_one_distribution_(thread_random_engine);
}

void BartolPM14Generator::SetTargetNevent(size_t n, size_t n_grid)
{
  for(size_t i = 0; i < 2; ++i) {
    sampler_[i].SetMinNevent(n);
    sampler_[i].SetGridMinNevent(n_grid);
  }
}
