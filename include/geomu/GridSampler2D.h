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
  double Sample(double *x, double *y);
  double EvenlySample(double *x, double *y);  // even probability for each grid

  // Event number assurance.
  // Should be called after SetGrid().
  void SetMinNevent(size_t n);
  void SetGridMinNevent(size_t n);

  // Sample at least 'MinNevent' events, where each grid contains at least GridMinNevent events.
  // Returns false if all above requirements are satisfied.
  bool SmartSample(double *x, double *y, double *w);

private:
  std::vector<double> grid_x_, grid_y_;
  std::vector<std::vector<double>> grid_;  // yield densities at grid points
  std::vector<std::vector<double>> center_;  // yield densities at lattice centers
  std::vector<double> frac_;  // yield fractions, flattened
  std::vector<double> frac_cumsum_;  // cumulative sum of yield fractions, flattened
  std::vector<size_t> min_nevent_;  // grid minimum event number, flattened
  double total_yield_;
  size_t nevent_remain_, ngrid_to_refine_;
  std::uniform_real_distribution<double> uniform_distribution_;

  // Used by SetGrid() for pre-procession works.
  void ProcessGrid();

  // Generate a uniform distributed random number in [0.0, 1.0).
  double Random();

  // Sample in specified grid.
  double SampleGrid(double *x, double *y, size_t id);

  // Compute ngrid_to_refine_.
  void UpdateNgridToRefine();
};
