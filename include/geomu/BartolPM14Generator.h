#pragma once
#include <random>

class GridSampler2D;

class BartolPM14Generator {

public:
  BartolPM14Generator();
  ~BartolPM14Generator();
  BartolPM14Generator(const BartolPM14Generator &) = delete;

  // Generate neutrino beam.
  double Generate(double *energy, double *cos_theta, bool *anti) const;

private:
  GridSampler2D *sampler_;  // [0]: 14  [1]: -14
  mutable std::uniform_int_distribution<int> zero_one_distribution_;

  // Load data and initialize sampler.
  void LoadData();

  // Randomly generate 0 or 1.
  bool ZeroOrOne() const;

};
