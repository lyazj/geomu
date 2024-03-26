#pragma once
#include <vector>
#include <random>

class GridSampler2D {

public:
  GridSampler2D() = default;
  GridSampler2D(const GridSampler2D &) = delete;

  // Input grid: yield densities relevant to (x, y), an x-major matrix.
  void SetGrid(const std::vector<double> &grid_x, const std::vector<double> &grid_y,
      const std::vector<std::vector<double>> &grid);

  // Returns weight of current sample.
  double Sample(double *x, double *y) const;

  // Set random number seed.
  void Seed(double seed);

private:
  std::vector<double> grid_x_, grid_y_;
  std::vector<std::vector<double>> grid_;  // yield densities at grid points
  std::vector<std::vector<double>> center_;  // yield densities at lattice centers
  std::vector<double> frac_cumsum_;  // cumulative sum of yield fractions, flattened
  double total_yield_;
  mutable std::default_random_engine random_engine_;
  mutable std::uniform_real_distribution<double> uniform_distribution_;

  // Used by SetGrid() for pre-procession works.
  void ProcessGrid();

  // Generate a uniform distributed random number in [0.0, 1.0).
  double Random() const;
};
