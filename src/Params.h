#ifndef PARAMS_H
#define PARAMS_H

// size of cell
#define WIDHT_CELL 100
#define HEIGHT_CELL 100

// border size
#define OFF_SET 5

// number of cell in one row/column
#define NCELL 3

// deth of the tree, less than NCELL*NCELL -1
// because (NCELL*NCELL-1)! is to much
#define TREE_DEPTH 8

// number of sign aligned for win
#define WIN_ALIGN 3

// signs
#define SIGN_X 1
#define SIGN_O 2
#define NO_SIGN 0
#define WIN_O 8

// boolean;
#define TRUE 1
#define FALSE 0

// set solo mode by default
#define SOLO TRUE

#endif