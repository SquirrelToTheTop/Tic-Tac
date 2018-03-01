#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

// size of cell
#define WIDHT_CELL 100
#define HEIGHT_CELL 100

// border size
#define OFF_SET 5

// number of cell in one row/column
#define NCELL 3

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

#include <SDL/SDL.h>

/* include file Forest.h which is the header for Tree structure */
#include "Forest.h"

/* Function prototype */
void test_4_winner(int **, int *, char *);
void create_tree(int **);

Tree * initialize_tree();
void show_tree(Tree *);

int main ( int argc, char** argv ){

  int i, j;

  /* Define game*/
  int **board;
  
  /* allocate memory */
  board = (int **)malloc(sizeof(int *)*NCELL);
  for(i=0; i<NCELL; i++){
    board[i] = (int *) malloc(sizeof(int)*NCELL);
  }

    /* initialize content of board with NO_SIGN def */
  for(i=0; i<NCELL; i++){
    for(j=0; j<NCELL; j++){
      board[i][j] = NO_SIGN;
    }
  }

  /* SDL PART */
  SDL_Surface* screen;
  SDL_Surface* white_cell;
  SDL_Surface* bmpX_cell;
  SDL_Surface* bmpO_cell;
  
  SDL_Rect white_pos, x_cell_pos, o_cell_pos;
  
  int video_mode_x = WIDHT_CELL * NCELL + (NCELL+1) * OFF_SET;
  int video_mode_y = HEIGHT_CELL * NCELL + (NCELL+1) * OFF_SET;
  
  // initialize SDL video
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
    printf( "Unable to init SDL: %s\n", SDL_GetError() );
    return 1;
  }
  
  // make sure SDL cleans up before exit
  atexit(SDL_Quit);
  
  // create a new window
  screen = SDL_SetVideoMode(video_mode_x, video_mode_y, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  if ( !screen ){
    printf("Unable to set %dx%d video: %s\n", video_mode_x, video_mode_y, SDL_GetError());
    return 1;
  }

  // load images for X_cell and O_cell + while_cell as white rect
  bmpX_cell = SDL_LoadBMP("x.bmp");
  bmpO_cell = SDL_LoadBMP("o.bmp");
  white_cell = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDHT_CELL, HEIGHT_CELL, 32, 0, 0, 0, 0);
  
  if (bmpX_cell == NULL){
    printf("Unable to load bitmap image : %s\n", SDL_GetError());
    return 1;
  }
  
  // Title
  SDL_WM_SetCaption("TTT", NULL);
  
  // create background of screen
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  
  // create board with all white cell at start
  SDL_FillRect(white_cell, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
  for(i=0; i<NCELL; i++){
    white_pos.x = (i+1)*OFF_SET + i*WIDHT_CELL;
    for(j=0; j<NCELL; j++){
      white_pos.y = (j+1)*OFF_SET + j*HEIGHT_CELL;
      SDL_BlitSurface(white_cell, NULL, screen, &white_pos);
    }
  }
  
  SDL_Flip(screen);
  
  // program main loop
  int done = FALSE;
  int winner = FALSE;
  char winner_sign;
  
  while (!done){
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event)){
      // check for messages
      switch (event.type){
	// exit if the window is closed
        case SDL_QUIT:
	  done = TRUE;
	  break;
	
       // check for keypresses
	
        case SDL_KEYDOWN:
	  {
	    switch(event.key.keysym.sym){
	      // exit if ESCAPE is pressed
	      case SDLK_ESCAPE:
		done = TRUE;
		break;
	    
		// test if SPACE is pressed used to test if there is a winner
	      case SDLK_SPACE:
	        test_4_winner(board, &winner, &winner_sign);
		break;
	    }
	  }
      } // end switch

      SDL_PumpEvents();
      int tmp_x, tmp_y;
      int rel_pos_x, rel_pos_y;
      
      if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
	SDL_GetMouseState(&tmp_x, &tmp_y);
	
	/* WARNING : rel_pos_x -> index for column, rel_pos_y -> index for line */
	rel_pos_x = (int)(tmp_x / (WIDHT_CELL+OFF_SET));
	rel_pos_y = (int)(tmp_y / (HEIGHT_CELL+OFF_SET));
	
	if( board[rel_pos_y][rel_pos_x] == NO_SIGN ){
	  x_cell_pos.x = (rel_pos_x+1)*OFF_SET + rel_pos_x*WIDHT_CELL;
	  x_cell_pos.y = (rel_pos_y+1)*OFF_SET + rel_pos_y*HEIGHT_CELL;
	  SDL_BlitSurface(bmpX_cell, NULL, screen, &x_cell_pos);
	  
	  /* add value to board */
	  board[rel_pos_y][rel_pos_x] = SIGN_X;
	  
	  /* test for win */
	  test_4_winner(board, &winner, &winner_sign);
	}
      }

      if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
	SDL_GetMouseState(&tmp_x, &tmp_y);
	
	rel_pos_x = (int)(tmp_x / (WIDHT_CELL+OFF_SET));
	rel_pos_y = (int)(tmp_y / (HEIGHT_CELL+OFF_SET));
	
	if( board[rel_pos_y][rel_pos_x] == NO_SIGN ){
	  o_cell_pos.x = (rel_pos_x+1)*OFF_SET + rel_pos_x*WIDHT_CELL;
	  o_cell_pos.y = (rel_pos_y+1)*OFF_SET + rel_pos_y*HEIGHT_CELL;
	  SDL_BlitSurface(bmpO_cell, NULL, screen, &o_cell_pos);
	  
	  /* add value to board */
	  board[rel_pos_y][rel_pos_x] = SIGN_O;
	  
	  /* test for win */
	  test_4_winner(board, &winner, &winner_sign);
	}
      }

      if( winner ){
	printf("There is a winner ! My job is done here ! Syonara pucci !\n");
	done = TRUE;
      }

    } // end of message processing

        // finally, update the screen :)
    SDL_Flip(screen);
  } // end main loop

  // all is well ;
  printf("Exited SDL cleanly\n\n");

  /* SDL free allocated memory */
  SDL_FreeSurface(white_cell);
  SDL_FreeSurface(bmpO_cell);
  SDL_FreeSurface(bmpX_cell);
  SDL_FreeSurface(screen);
  
  /* free allocated memory */
  for(i=0; i<NCELL; i++){
    free(board[i]);
  }
  free(board);

  /* TEST OF TREE STRUCTURE */
  Tree *bonzai = initialize_tree();
  show_tree(bonzai);

  return 0;
}

/* Test for a winner, ie : N aligned sign diagonaly, verticaly or horizontaly
*
*  Parameters :
*               board : input board to check
*               any_winner : return value 1 if there is a winner or 0 if not
*               win_sign   : if any_winner == 1 win_sing contain the character of the winnner !
*
*/
void test_4_winner(int **board, int *any_winner, char *win_sign){
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
  *win_sign = (rst_diag1 > 1 || rst_diag2 > 1 && *any_winner) ? "O" : "X";
  if( *any_winner)
    return;
  
  /* test all value */
  for(i=0; i<NCELL; i++){
    *any_winner = (rst_line[i] == WIN_O || rst_line[i] == 1 || rst_column[i] == WIN_O || rst_column[i] == 1 ) ? TRUE : FALSE;
    *win_sign = (rst_line[i] > 1 || rst_column[i] > 1 && *any_winner) ? "O" : "X";
    if( *any_winner )
      return;
  }
  
}

/* Initialize the tree since there is NCELL x NCELL
 * the first node of the tree will have (NCELL x NCELL)-1
 * the second ((NCELL*NCELL)-1)-1 etc ... we'll allocate the
 * memory in consequence and hope for the best !
 *
 * headache with this
 *
 */
Tree * initialize_tree(){

  int i, j, k, n_lvl, current_n_leaf, next_n_leaf;
  int leaf_by_lvl[NCELL*NCELL];

  Tree *tree;
  Node *current, *first;

  /* number of level of the tree */
  n_lvl = NCELL*NCELL;

  /* allocate the tree =) */
  tree = (Tree *) malloc(sizeof(Tree *));
  if( !tree )
    printf("Allocation failed !");
  else
    printf("Allocation of Tree success - mem adresse %p\n", tree);

  /* compute leaf by level */
  leaf_by_lvl[0] = 1;
  for(i=1; i<n_lvl; i++)
    leaf_by_lvl[i] = n_lvl-i;

#ifdef DEBUG
  for(i=0; i<n_lvl; i++)
    printf("---> lvl %d -> n_leaf = %d\n", i, leaf_by_lvl[i]);
  printf("\n");
#endif

  /* allocate the first node of the tree, ie : root */
  current_n_leaf = NCELL*NCELL-1;
  first = (Node *) malloc(sizeof(Node *));
  //first->n_subnode = current_n_leaf;

  //first->next = (Node **) malloc(first->n_subnode * sizeof(Node *));

  first->previous = NULL;
  tree->root = first;

#ifdef DEBUG
  printf("|| -> Root at %p \n",tree->root);
#endif

  /* start allocation of the whole tree */
  current = first;
  n_lvl = 2;//NCELL*NCELL;
  next_n_leaf = current_n_leaf -1;
  for(i=0; i<n_lvl-1; i++){
    printf("Level i=%d, n_leaf = %d\n",i, current_n_leaf);
    for(j=0; j<leaf_by_lvl[i]; j++){
      current->n_subnode = leaf_by_lvl[i+1];
      current->next = (Node **) malloc((leaf_by_lvl[i+1])*sizeof(Node **));
      for(k=0; k<leaf_by_lvl[i+1]; k++){
	Node *ptr_tmp=NULL;

	ptr_tmp = (Node *) malloc(sizeof(Node *));
	if( !ptr_tmp )
	  exit(EXIT_FAILURE);

	ptr_tmp->value = 0;
	ptr_tmp->n_subnode = 0;
	ptr_tmp->next = NULL;
	ptr_tmp->previous = current;
	current->next[k] =&ptr_tmp;

#ifdef DEBUG
	printf("|||| -> Node %p as leaf %p \n",current, current->next[k]);
#endif
	free(ptr_tmp);
      }
      current = current->next[j];
    }
  }

  /* the next pointor of all final node should be set to nul */
  /* TODO but how ??? */

  return tree;
}

/*
 * Display the tree - used for debugging purpose !
 *
 */
void show_tree(Tree *bonzai){
  int i, j, count_lvl, count_leaf=0;
  Node *current;

  count_lvl = 0;
  current = bonzai->root;
  count_leaf = current->n_subnode;

  printf("\n");
  while(current->next[0] != NULL){
    printf("-----> leaf : %p having % sub-leaf \n", current, current->n_subnode);

    if( count_leaf == current->n_subnode){
      count_leaf = 0;
      current = current->next[count_leaf];
    }else
      count_leaf ++;

  }
}

/* Create tree of possibility using chained list for min max algorithm
 *
 *
 *
 *
 */
void create_tree(int **board){

}
