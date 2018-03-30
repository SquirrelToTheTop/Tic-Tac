#ifndef GAME_H
#define GAME_H

#include "Forest.h"

/* allocate memory for board game */
int ** initialize_board_mem();

/* free memory for board game */
void free_board_mem(int **);

/* test for a winner in the board game */
void test_4_winner(int **, int *, int *);

/* same as test_4_winner with assembly code for fun */
void asm_test_4_winner(int **, int *, int *);

/* computer IA move */
int computer_move(int **, Tree *, int *, int *);

/* computer plays on random cell */
int computer_move_random(int **, int *, int *);

#endif