#include "geomu/GridSampler2D.h"
#include "geomu/random.h"
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <cmath>

using namespace std;

void GridSampler2D::SetGrid(const vector<double> &grid_x, const vector<double> &grid_y,
    const vector<vector<double>> &grid)
{
  grid_x_ = grid_x;
  grid_y_ = grid_y;
  grid_ = grid;
  ProcessGrid();
}

void GridSampler2D::ProcessGrid()
{
  size_t nrow = grid_.size();
  if(nrow < 2) throw invalid_argument("too few rows");
  size_t ncol = grid_[0].size();
  if(ncol < 2) throw invalid_argument("too few columns");
  if(nrow != grid_x_.size() || ncol != grid_y_.size()) throw invalid_argument("inconsistent grid shape");

  // Initialize auxiliary fields.
  center_.assign(nrow - 1, vector<double>(ncol - 1));
  frac_cumsum_ = { };
  frac_cumsum_.reserve((nrow - 1) * (ncol - 1));

  // Compute center yield densities and their sum based on grid points.
  double cumsum = 0.0;
  for(size_t i = 1; i < nrow; ++i) {
    if(grid_[i].size() != ncol) throw invalid_argument("inconsistent row lengths");
    for(size_t j = 1; j < ncol; ++j) {
      center_[i - 1][j - 1] = (grid_[i - 1][j - 1] + grid_[i - 1][j] + grid_[i][j - 1] + grid_[i][j]) / 4.0;
      double w = center_[i - 1][j - 1] * (grid_x_[i] - grid_x_[i - 1]) * (grid_y_[j] - grid_y_[j - 1]);
      if(!(w >= 0.0)) throw logic_error("invalid yield detected");
      cumsum += w;
      frac_cumsum_.push_back(cumsum);
    }
  }
  total_yield_ = cumsum;

  // Normalize frac_cumsum_.
  if(!isfinite(cumsum) || cumsum == 0.0) throw logic_error("invalid total yield detected");
  for(double &value : frac_cumsum_) value /= cumsum;
}

double GridSampler2D::Random() const
{
  return uniform_distribution_(thread_random_engine);
}

double GridSampler2D::Sample(double *x, double *y) const
{
  double r = Random();
  size_t id = upper_bound(frac_cumsum_.begin(), frac_cumsum_.end(), r) - frac_cumsum_.begin();
  size_t ncol = grid_y_.size();
  size_t i = id / (ncol - 1), j = id % (ncol - 1);
  double rx = Random(), ry = Random();
  *x = grid_x_[i] + (grid_x_[i + 1] - grid_x_[i]) * rx;
  *y = grid_y_[j] + (grid_y_[j + 1] - grid_y_[j]) * ry;
  double w0 = grid_[i][j] + (grid_[i][j + 1] - grid_[i][j]) * ry;
  double w1 = grid_[i + 1][j] + (grid_[i + 1][j + 1] - grid_[i + 1][j]) * ry;
  double w = w0 + (w1 - w0) * rx;
  return total_yield_ * (w / center_[i][j]);
}
