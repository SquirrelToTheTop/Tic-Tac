#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

/* include file Forest.h which is the header for Tree structure */
#include "Forest.h"
#include "Params.h"
#include "Game.h"

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
  bmpX_cell = SDL_LoadBMP("sprite/x.bmp");
  bmpO_cell = SDL_LoadBMP("sprite/o.bmp");
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
  int winner_sign;
  
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
		
	      default:
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
	printf("The winner is %d ! My job is done here ! Syonara pucci !\n", winner_sign);
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
  Tree *bonzai = initialize_tree_test();
  //show_tree(bonzai); 
  
  return 0;
}
