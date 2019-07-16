#pragma once
#include "interval.hpp"
#include <vector>

namespace grid2grid {
/*
A class describing a matrix n_rows x n_cols split into an arbitrary grid.
A grid is defined by rows_split and cols_split. More precisely, a matrix is
split into blocks such that a block with coordinates (i, j) is defined by:
    - row interval [rows_split[i], rows_split[i+1]) and similarly
    - column interval [cols_split[i], cols_split[i+1])
For each i, the following must hold:
    - 0 <= rows_split[i] < n_rows
    - 0 <= cols_split[i] < n_cols
*/
struct grid2D {
    // matrix dimensions
    int n_rows = 0;
    int n_cols = 0;
    // defines how rows are split
    std::vector<int> rows_split;
    // defines how cols are split
    std::vector<int> cols_split;

    grid2D() = default;
    grid2D(std::vector<int> &&r_split, std::vector<int> &&c_split);

    // returns index-th row interval, i.e. [rows_split[index], rows_split[index
    // + 1])
    interval row_interval(int index) const;

    // returns index-th column interval, i.e. [cols_split[index],
    // cols_split[index + 1])
    interval col_interval(int index) const;

    void transpose();
};

/*
A class describing a matrix split into an arbitrary grid (grid2D) where each
block is assigned to an arbitrary MPI rank. More precisely, a block with
coordinates (i, j) is assigned to an MPI rank defined by ranks[i][j].
*/
class assigned_grid2D {
  public:
    assigned_grid2D() = default;
    assigned_grid2D(grid2D &&g,
                    std::vector<std::vector<int>> &&proc,
                    int n_ranks);

    // returns the rank owning block (i, j)
    int owner(int i, int j) const;

    // returns a grid
    const grid2D &grid() const;

    // returns a total number of MPI ranks in the communicator
    // which owns the whole matrix. However, not all ranks have
    // to own a block of the matrix. Thus, it might happen that
    // maximum(ranks[i][j]) < n_ranks - 1 over all i, j
    int num_ranks() const;

    // returns index-th row interval
    interval rows_interval(int index) const;

    // returns index-th column interval
    interval cols_interval(int index) const;

    int block_size(int row_index, int col_index);

    // if flag='N' => no transpose
    // if flag='T' => transpose
    // if flag='C' => transpose and conjugate
    void transpose();

  private:
    std::vector<std::vector<int>> transpose(const std::vector<std::vector<int>>& v);

    grid2D g;
    std::vector<std::vector<int>> ranks;
    int n_ranks = 0;
};

} // namespace grid2grid
