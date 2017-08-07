# sudokuSolver
A bit better sudoku solver (that the basic backtracker)
Basically - this is improved backtracking-sudoku-solver algorithm, which uses "projections" to decrease number of possible input values for cells. 
I've managed to decrease time on some grids from ~1200 seconds (using bare backtracking method) to ~20-300 seconds per grid.
Every grid was taken from the site http://sudoku.org.ua/ using POST-method map.php?id=XXX, where XXX is a valid integer value. Be care though, that the output string has a "map": value, whereas grid is stored columns by rows, not rows by columns.
