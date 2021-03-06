#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

/* include file Forest.h which is the header for Tree structure */
#include "Forest.h"
#include "Params.h"
#include "Game.h"

int main ( int argc, char** argv ){

  // usual local 
  int i, j, n_sign_on_board=0;

  // Define game
  int **board;
  
  // who's next player - first played will be 'x' 
  int x_played=FALSE, computer_played=TRUE;
  
  
  // Board game and tree of possibility ---------------------------------------
  board = initialize_board_mem();
  if( board == NULL ){
    printf("%s> Error : Allocation of board failed ! Kernel Panic ! \n", RED);
    return 1;
  }else{
    printf("\n%s> Allocation of board success !\x1b[0m\n",GREEN);
  }
  
  
  // Tree structure 
  Tree *bonzai = initialize_tree_corrected_depth();
  if( bonzai == NULL ){
    printf("%s> Error : Allocation of bonzai failed ! Kernel Panic ! \n",RED);
    return 1;
  }else
    printf("\n%s> Allocation of tree of possibility success !\x1b[0m\n",GREEN);
  
  // check if root has been set 
  if( bonzai->root == NULL){
    printf("Error no root has been defined !\n");
    return 1;
  }
    
  // if debug mode, show tree
  #ifdef DEBUG
    printf("\n> root @ %p\n\n", bonzai->root);
    printf("%s--------------------- TREE STRUCTURE AND DATAS ---------------------\x1b[0m \n",GREEN);
    
    printf("\n LEAFS (mem add, value, sign, id_row, id_col, subleafs)\n\n");
    
    show_tree(bonzai->root); 
  #endif
  

  /* ------------------------------------------------- SDL PART --------------- */
  SDL_Surface* screen;
  SDL_Surface* white_cell;
  SDL_Surface* bmpX_cell;
  SDL_Surface* bmpO_cell;
  
  SDL_Rect white_pos, sign_pos;
  
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
      
      // the player with the sign 'x' clicked
      if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && !x_played) {
	SDL_GetMouseState(&tmp_x, &tmp_y);
	
	// WARNING : rel_pos_x -> index for column, rel_pos_y -> index for line
	rel_pos_x = (int)(tmp_x / (WIDHT_CELL+OFF_SET));
	rel_pos_y = (int)(tmp_y / (HEIGHT_CELL+OFF_SET));
	
        // ADD SIGN TO THE GRID
	if( board[rel_pos_y][rel_pos_x] == NO_SIGN ){
	  sign_pos.x = (rel_pos_x+1)*OFF_SET + rel_pos_x*WIDHT_CELL;
	  sign_pos.y = (rel_pos_y+1)*OFF_SET + rel_pos_y*HEIGHT_CELL;
	  SDL_BlitSurface(bmpX_cell, NULL, screen, &sign_pos);
	  
	  // add value to board
	  board[rel_pos_y][rel_pos_x] = SIGN_X;
	  
          /* add 1 to n_sign_on_board
           * WARNING must be before the call to add_to_tree because used as level 
           */
          n_sign_on_board ++;

          // add value to tree
          /*if( add_to_tree(bonzai, n_sign_on_board, rel_pos_x, rel_pos_y, 'X') ){
            printf("> Sign added to tree (%d, ", n_sign_on_board);
            printf("%c, %d, %d): succes !\n", 'X', rel_pos_x, rel_pos_y);
          }else{
            printf("%s> New sign cannot be added to tree ! \x1b[0m \n", RED);
          }*/

          // if debug mode, show tree
          #ifdef DEBUG
            printf("\n> root @ %p\n\n", bonzai->root);
            printf("\x1b[31m--------------------- TREE STRUCTURE AND DATAS ---------------------\x1b[0m \n");
    
            printf("\n LEAFS (mem add, value, sign, id_row, id_col, subleafs)\n\n");
    
            show_tree(bonzai->root); 
          #endif
          
	  // test for win
	  test_4_winner(board, &winner, &winner_sign);
          
          // set boolean for next player
          x_played = TRUE;
          computer_played = FALSE;
          
	}
      }
      
      /* ----------- COMPUTER IA PART ----------------------------------------- */
      if( x_played && !computer_played && n_sign_on_board != NCELL*NCELL){
        if( computer_move_random(board, &rel_pos_x, &rel_pos_y) ){
          
          board[rel_pos_y][rel_pos_x] = SIGN_O;
          
          sign_pos.x = (rel_pos_x+1)*OFF_SET + rel_pos_x*WIDHT_CELL;
	  sign_pos.y = (rel_pos_y+1)*OFF_SET + rel_pos_y*HEIGHT_CELL;
	  SDL_BlitSurface(bmpO_cell, NULL, screen, &sign_pos);
          
          printf("> Move success @ %d, %d \n", rel_pos_y, rel_pos_x);
        }else
         printf("> Error : computer can't plays\n");
        
        // test for win
        asm_test_4_winner(board, &winner, &winner_sign);
        
        /* add 1 to n_sign_on_board
        * WARNING must be before the call to add_to_tree because used as level 
        */
        n_sign_on_board ++ ;
        
        /*if( add_to_tree(bonzai, n_sign_on_board, rel_pos_x, rel_pos_y, 'X') ){
          printf("> Sign added to tree (%d, ", n_sign_on_board);
          printf("%c, %d, %d): succes !\n", 'X', rel_pos_x, rel_pos_y);
        }else{
          printf("%s> New sign cannot be added to tree ! \x1b[0m \n", RED);
        }*/
        
        computer_played = TRUE;
        x_played = FALSE;
      }
      /* ------------------------------------------------------------------ */

      if( winner ){
	printf("\nThe winner is %d ! My job is done here ! Syonara pucci !\n", winner_sign);
	done = TRUE;
      }

    } // end of message processing

        // finally, update the screen :)
    SDL_Flip(screen);
  } // end main loop

  // all is well ;
  printf("\n--> Exited SDL cleanly\n\n");

  /* SDL free allocated memory */
  SDL_FreeSurface(white_cell);
  SDL_FreeSurface(bmpO_cell);
  SDL_FreeSurface(bmpX_cell);
  SDL_FreeSurface(screen);
  
  /* free allocated memory */
  free_board_mem(board);
   
  return 0;
}
