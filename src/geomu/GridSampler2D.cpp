#include "geomu/GridSampler2D.h"
#include "geomu/random.h"
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

//#define DIAG_LVL  DIAG_DBG
#include "geomu/diagnostic.h"

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
  frac_ = { };
  frac_.reserve((nrow - 1) * (ncol - 1));
  frac_cumsum_ = { };
  frac_cumsum_.reserve((nrow - 1) * (ncol - 1));
  min_nevent_.assign((nrow - 1) * (ncol - 1), 0);

  // Compute center yield densities and their sum based on grid points.
  double cumsum = 0.0;
  for(size_t i = 1; i < nrow; ++i) {
    if(grid_[i].size() != ncol) throw invalid_argument("inconsistent row lengths");
    for(size_t j = 1; j < ncol; ++j) {
      center_[i - 1][j - 1] = (grid_[i - 1][j - 1] + grid_[i - 1][j] + grid_[i][j - 1] + grid_[i][j]) / 4.0;
      double w = center_[i - 1][j - 1] * (grid_x_[i] - grid_x_[i - 1]) * (grid_y_[j] - grid_y_[j - 1]);
      if(!(w >= 0.0)) throw logic_error("invalid yield detected");
      cumsum += w;
      frac_.push_back(w);
      frac_cumsum_.push_back(cumsum);
    }
  }
  total_yield_ = cumsum;
  difo << "total yield: " << total_yield_ << dend;
  nevent_remain_ = 0;
  ngrid_to_refine_ = 0;

  // Normalize frac_ and frac_cumsum_.
  if(!isfinite(cumsum) || cumsum == 0.0) throw logic_error("invalid total yield detected");
  for(double &value : frac_) value /= cumsum;
  for(double &value : frac_cumsum_) value /= cumsum;

  difo << "lattice weights:";
  difo << scientific << setprecision(2);
  for(size_t i = 0; i < nrow - 1; ++i) {
    difo << '\n';
    for(size_t j = 0; j < ncol - 1; ++j) {
       difo << '\t' << frac_[i * (ncol - 1) + j];
    }
  }
  difo << defaultfloat << dend;
}

double GridSampler2D::Random()
{
  return uniform_distribution_(thread_random_engine);
}

double GridSampler2D::SampleGrid(double *x, double *y, size_t id)
{
  if(nevent_remain_) --nevent_remain_;
  if(min_nevent_[id] && !--min_nevent_[id]) --ngrid_to_refine_;
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

double GridSampler2D::Sample(double *x, double *y)
{
  double r = Random();
  size_t id = upper_bound(frac_cumsum_.begin(), frac_cumsum_.end(), r) - frac_cumsum_.begin();
  return SampleGrid(x, y, id);
}

double GridSampler2D::EvenlySample(double *x, double *y)
{
  size_t id = Random() * frac_cumsum_.size();
  return SampleGrid(x, y, id) * (frac_cumsum_.size() * frac_[id]);
}

void GridSampler2D::SetMinNevent(size_t n)
{
  nevent_remain_ = n;
  for(size_t i = 0; i < min_nevent_.size(); ++i) {
    min_nevent_[i] = max(min_nevent_[i], (size_t)round(n * frac_[i]));
  }
  UpdateNgridToRefine();
}

void GridSampler2D::SetGridMinNevent(size_t n)
{
  for(size_t i = 0; i < min_nevent_.size(); ++i) {
    min_nevent_[i] = max(min_nevent_[i], n);
  }
  UpdateNgridToRefine();
}

bool GridSampler2D::SmartSample(double *x, double *y, double *w)
{
  if(!ngrid_to_refine_) return false;
  *w = nevent_remain_ ? Sample(x, y) : EvenlySample(x, y);
  return true;
}

void GridSampler2D::UpdateNgridToRefine()
{
  size_t n = 0;
  for(size_t value : min_nevent_) n += !!value;
  ngrid_to_refine_ = n;

  size_t nrow = grid_.size();
  size_t ncol = grid_[0].size();
  difo << "lattice minimal event numbers:";
  for(size_t i = 0; i < nrow - 1; ++i) {
    difo << '\n';
    for(size_t j = 0; j < ncol - 1; ++j) {
       difo << '\t' << min_nevent_[i * (ncol - 1) + j];
    }
  }
  difo << dend;
  difo << "minimal total even number: " << nevent_remain_ << dend;
  difo << "number of grid to refine: " << ngrid_to_refine_ << dend;
}
