#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>

#include <SDL/SDL_rotozoom.h>

#include "quizz.h"

int main( int argc, char *argv[] ){

   SDL_Surface* ecran;
   int result;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
      fprintf( stderr, "Erreur.\n" );
      return 1;
   }

   ecran = SDL_SetVideoMode( 900, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
   SDL_WM_SetCaption( "Tic-Tac-Toe GAME", NULL );

   result = tictactoe(ecran);

   printf("\n\n***************  result = %d\n", result);

}


