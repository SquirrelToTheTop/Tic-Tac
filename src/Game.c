#include "Params.h"
#include "Game.h"

/* Test for a winner, ie : N aligned sign diagonaly, verticaly or horizontaly
*
*  Parameters :
*               board : input board to check
*               any_winner : return value 1 if there is a winner or 0 if not
*               win_sign   : if any_winner == 1 win_sing contain the character of the winnner !
*
*/
void test_4_winner(int **board, int *any_winner, int *win_sign){
  int i, j, rst_diag1, rst_diag2;
  int rst_line[NCELL], rst_column[NCELL];

  /* compute product of values in board
   * if value of product == 1, then the winner is "X"
   * according to value set to represent "X" sign
   * if value of product == 8 (2*2*2) then winner is "O"
   */
  rst_diag1 = 1;
  rst_diag2 = 1;
  for(i=0; i<NCELL; i++){
    rst_diag1 *= board[i][i];
    rst_diag2 *= board[i][NCELL-i-1];
    rst_line[i] = 1;
    rst_column[i] = 1;
    for(j=0; j<NCELL; j++){
      rst_line[i] *= board[i][j];
      rst_column[i] *= board[j][i];
    }
  }

  /* test value for diagonal */
  *any_winner = (rst_diag1 == WIN_O || rst_diag2 == WIN_O || rst_diag1 == 1 || rst_diag2 == 1 ) ? TRUE : FALSE;
  *win_sign = ( (rst_diag1>1) || (rst_diag2 > 1) && (*any_winner) ) ? SIGN_O : SIGN_X;
  if( *any_winner)
    return;
  
  /* test all value */
  for(i=0; i<NCELL; i++){
    *any_winner = (rst_line[i] == WIN_O || rst_line[i] == 1 || rst_column[i] == WIN_O || rst_column[i] == 1 ) ? TRUE : FALSE;
    *win_sign = (rst_line[i] > 1 || rst_column[i] > 1 && *any_winner) ? SIGN_O : SIGN_X;
    if( *any_winner )
      return;
  }
  
}

/* Test for a winner, ie : N aligned sign diagonaly, verticaly or horizontaly
*
*  Parameters :
*               board : input board to check
*               any_winner : return value 1 if there is a winner or 0 if not
*               win_sign   : if any_winner == 1 win_sing contain the character of the winnner !
*
*  USING ASSEMBLY INLINE FOR FUN
*/
void asm_test_4_winner(int **board, int *any_winner, int *win_sign){
  int i, j, rst_diag1, rst_diag2;
  int rst_line[NCELL], rst_column[NCELL];

  /* compute product of values in board
   * if value of product == 1, then the winner is "X"
   * according to value set to represent "X" sign
   * if value of product == 8 (2*2*2) then winner is "O"
   */
  rst_diag1 = 1;
  rst_diag2 = 1;
  for(i=0; i<NCELL; i++){
    __asm__ __volatile__( "imul %%ebx, %%eax;": "=a" (rst_diag1) : "a" (rst_diag1), "b" (board[i][i]));
    
    __asm__ __volatile__( "imul %%ebx, %%eax;": "=a" (rst_diag2) : "a" (rst_diag2), "b" (board[i][NCELL-i-1]));
    
    rst_line[i] = 1;
    rst_column[i] = 1;
    for(j=0; j<NCELL; j++){
      
      __asm__ __volatile__( "imul %%ebx, %%eax;": "=a" (rst_line[i]) : "a" (rst_line[i]), "b" (board[i][j]));
      
      __asm__ __volatile__( "imul %%ebx, %%eax;": "=a" (rst_column[i]) : "a" (rst_column[i]), "b" (board[j][i]));
      
    }
  }

  /* test value for diagonal */
  *any_winner = (rst_diag1 == WIN_O || rst_diag2 == WIN_O || rst_diag1 == 1 || rst_diag2 == 1 ) ? TRUE : FALSE;
  *win_sign = ( (rst_diag1>1) || (rst_diag2 > 1) && (*any_winner) ) ? SIGN_O : SIGN_X;
  if( *any_winner)
    return;
  
  /* test all value */
  for(i=0; i<NCELL; i++){
    *any_winner = (rst_line[i] == WIN_O || rst_line[i] == 1 || rst_column[i] == WIN_O || rst_column[i] == 1 ) ? TRUE : FALSE;
    *win_sign = (rst_line[i] > 1 || rst_column[i] > 1 && *any_winner) ? SIGN_O : SIGN_X;
    if( *any_winner )
      return;
  }
  
}