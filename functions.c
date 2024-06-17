#include <stdbool.h>
//#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include <SDL/SDL_rotozoom.h>

#include "quizz.h"


//****************************************************************************************//





int tictactoe( SDL_Surface* screen ) {


   Xo xo;

   char* text1 = malloc( 20 * sizeof( char ) );
   char* text2 = malloc( 10 * sizeof( char ) );
   char* text3 = malloc( 30 * sizeof( char ) );


   double animcounter = 0;

   SDL_Rect hlineUp;
   SDL_Rect hlineDown;
   SDL_Rect vlineLeft;
   SDL_Rect vlineRight;

   int cellToSelect = 0;
   int lastSelectedCell = 0;

   int hitcell = -1;

   int retval, retvalx, retvaly;

   SDL_Event event;
   int gameover = 0;

   int result = 0;
   int cumulresult = 0;
   int trials = 0;

   int fonts_initialized = 0;

   SDL_Surface* textSurface = NULL;


   SDL_Color textColor = {255, 0, 0};  // RED

   SDL_Color swhitecreamy = {255, 255, 200};
   SDL_Color sbrown = {66, 35, 19};

   Uint32 whitecreamy = SDL_MapRGB( screen->format, 0xff, 0xff, 0xbb );
   Uint32 brown = SDL_MapRGB( screen->format, 0x42, 0x23, 0x13 );

   SDL_Rect restartRectangle = {250, 500, 400, 50};
   SDL_Rect textRectangle = {330, 510, 200, 30};

   SDL_Rect finalRectangle = {250, 150, 400, 100};
   SDL_Rect finaltextRectangle = {350, 180, 200, 30};

   Uint32 darkblue = SDL_MapRGB( screen->format, 0x00, 0x00, 0x7f );

   Uint32 blue = SDL_MapRGB( screen->format, 0x33, 0x33, 0xff );
   Uint32 green = SDL_MapRGB( screen->format, 0x55, 0xbb, 0x55 );
   Uint32 red = SDL_MapRGB( screen->format, 0xbb, 0x33, 0x33 );
   Uint32 lineColor = blue;

   SDL_Surface *back = SDL_DisplayFormat( IMG_Load( "T_600_600.png" ) );
   SDL_Rect bgPosRect = back->clip_rect;
   bgPosRect.x = 150;

   SDL_Surface *roundsback = NULL;

   Init_Xo1( &xo );


   // Initialization of the FONT System
   fonts_initialized = TTF_Init();

   if( fonts_initialized < 0 ) {
      fprintf( stderr, "Errors in initializing the font system : %s\n", SDL_GetError() );
      //return 1;
   }

   // Load a system font (e.g., "Arial")
   TTF_Font* font = TTF_OpenFont( "font1.ttf", 26 );
   if( !font ) {
      printf( "Failed to load system font: %s\n", TTF_GetError() );
      //return 1;
   }

   SDL_Surface* drawSurface = NULL;
   SDL_Color drawTextColor = {0, 0, 170}; // blue color
   SDL_Rect drawTextPosition = {410, 300, 0, 0}; // Position of the text on the screen

   SDL_Surface* badLuckSurface = NULL;
   SDL_Color badLuckTextColor = {170, 170, 0}; // Orange color
   SDL_Rect badLuckTextPosition = {410, 300, 0, 0};// Position of the text on the screen

   SDL_Surface* goodSurface = NULL;
   SDL_Color goodTextColor = {0, 170, 0}; // Green color
   SDL_Rect goodTextPosition = {410, 300, 0, 0}; // Position of the text on the screen

// Render text onto a surface
   goodSurface = TTF_RenderText_Solid( font, "Goooood !", goodTextColor );
   drawSurface = TTF_RenderText_Solid( font, "Game is Draw !", drawTextColor );
   badLuckSurface = TTF_RenderText_Solid( font, "Hard Luck !", badLuckTextColor );

   SDL_Surface* restartSurface = NULL;

   SDL_Rect rot_rect;
   SDL_Surface* rot;
   SDL_Surface* zoom;

   int angle = 0;
   double factor = 0.0;
   int zoomshrink = 1;


   //   GAME MAIN LOOP/////////////////////////////////////////////////////////////////////////////
   while( trials < 3 ) {

      while( gameover == 0 ) {

         SDL_BlitSurface( back, NULL, screen, &bgPosRect );
         setLineRect( xo,  &hlineUp,  cellToSelect, UP_HLINE );
         SDL_FillRect( screen, &hlineUp, lineColor );

         setLineRect( xo,  &hlineDown,  cellToSelect, DOWN_HLINE );
         SDL_FillRect( screen, &hlineDown, lineColor );

         setLineRect( xo,  &vlineLeft,  cellToSelect, LEFT_VLINE );
         SDL_FillRect( screen, &vlineLeft, lineColor );

         setLineRect( xo,  &vlineRight,  cellToSelect, RIGHT_VLINE );
         SDL_FillRect( screen, &vlineRight, lineColor );

         // start event handling
         while( SDL_PollEvent( &event ) ) {

            switch( event.type ) {
            /*case SDL_MOUSEMOTION: {}
            break;*/
            case SDL_MOUSEBUTTONUP: {
               if( event.button.button == SDL_BUTTON_LEFT ) {
                  hitcell = getClickedCell( event.button.x, event.button.y );
                  if( hitcell >= 0 ) {
                     play_human1( &xo, hitcell );
                  }
               }
            }
            break;

            case SDL_KEYDOWN:

               switch( event.key.keysym.sym ) {

               case SDLK_LEFT:

                  cellToSelect = getCellToSelect( lastSelectedCell, TO_LEFT );
                  if( isCellFree( &xo, cellToSelect ) ) lineColor = blue;
                  else lineColor = red;

                  break;
               case SDLK_RIGHT:

                  cellToSelect = getCellToSelect( lastSelectedCell, TO_RIGHT );
                  if( isCellFree( &xo, cellToSelect ) ) lineColor = blue;
                  else lineColor = red;

                  break;
               case SDLK_UP:

                  cellToSelect = getCellToSelect( lastSelectedCell, TO_UP );
                  if( isCellFree( &xo, cellToSelect ) ) lineColor = blue;
                  else lineColor = red;

                  break;
               case SDLK_DOWN:

                  cellToSelect = getCellToSelect( lastSelectedCell, TO_DOWN );
                  if( isCellFree( &xo, cellToSelect ) ) lineColor = blue;
                  else lineColor = red;

                  break;
               case SDLK_RETURN:

                  if( isCellFree( &xo, lastSelectedCell ) ) {
                     play_human1( &xo, lastSelectedCell );
                  }
                  break;
               case SDLK_KP_ENTER :

                  if( isCellFree( &xo, lastSelectedCell ) ) {
                     play_human1( &xo, lastSelectedCell );
                  }
                  break;
               default:

                  break;

               }/////////////////////
               break;

            case SDL_QUIT:
               gameover = 1;
               break;

            }// end switch
         }
         // end event handling

         retval = -100;
         retvalx = -100;
         retvaly = -100;

         result = Checkwin_Xo( &xo );

         if( result == HUMAN_WINS ) { xo.tour = -1; xo.winner = 1;}  // -9
         else if( result == MACHINE_WINS )  { xo.tour = -1; xo.winner = 2;}   // 9
         else if( result == GAME_IS_DRAW )  { xo.tour = -1; xo.winner = 0;}  // 99

         if( xo.tour == 2 ) {

            retval = bestshot( &xo, 1 );

            if( retval == -1 ) {
               //printf( " \n\nAucune suggestion de jeu trouvée !\n" );
               if( matrix_is_full( &xo ) ) {
                  xo.winner = 0;
                  result = GAME_IS_DRAW;
                  break;
               }
            } else {

               retvalx = ( int ) retval / 10;
               retvaly = retval % 10;
               play_machine( &xo, retvalx, retvaly );
            }

         }

         Diplay_Xo1( &xo, screen, ( int )animcounter );

         if( animcounter > 5.6 ) animcounter = 0;
         else animcounter += 0.2;

         lastSelectedCell = cellToSelect;

         SDL_Flip( screen );

         if( result != 0 ) break;

         result = Checkwin_Xo( &xo );

         if( result == HUMAN_WINS ) { xo.tour = -1; xo.winner = 1;}         // -9
         else if( result == MACHINE_WINS ) { xo.tour = -1; xo.winner = 2;}  //  9
         else if( result == GAME_IS_DRAW ) { xo.tour = -1; xo.winner = 0;}  // 99

         if( result != 0 ) break;

         SDL_Delay( 50 );
      }
      //   END GAME MAIN LOOP/////////////////////////////////////////////


      if( !gameover ) {

         if( xo.winner == 1 ) {
            printf( "the Human wins !\n" );
            cumulresult += 1;

         } else if( xo.winner == 2 ) {
            printf( "the Machine wins !\n" );
            cumulresult += -1;

         } else if( xo.winner == 0 ) {
            printf( "the Game was draw !\n" );
         }
      }

      else printf( "Game was interrupted !\n" );

      int quit = 0;

      Uint32 start;
      Uint32 delay = 0;
      int restartIn = 3;

      text1 = strcpy( text1, "Next Round in " );

      start = SDL_GetTicks();

      roundsback = SDL_DisplayFormat( IMG_Load( "rounds.png" ) );

      while( !quit ) {//start of the second game loop

         if( gameover ) break;

         while( SDL_PollEvent( &event ) ) {

            switch( event.type ) {

            case SDL_QUIT :
               quit = 1;
               break;


            }//end of switching event types

         }// end of polling and managing events

         // the display logic

         text1 = strcpy( text1, "Next Round in " );
         sprintf( text2, "%d", restartIn );
         text3 = strcat( text1, text2 );
         text3 = strcat( text3, " secondes" );

         restartSurface = TTF_RenderText_Solid( font, text3, sbrown );

         SDL_BlitSurface( roundsback, NULL, screen, &bgPosRect );

         SDL_FillRect( screen, &restartRectangle, whitecreamy );

         SDL_BlitSurface( restartSurface, NULL, screen, &textRectangle );

         if( result == MACHINE_WINS ) {

            angle += 10;
            if( angle >= 360 ) angle = angle - 360;

            rot = rotozoomSurface( badLuckSurface, angle, 1.0, 1 );
            SDL_Rect rotrect = {410, 200, 0, 0};
            rotrect.x -= rot->w / 2 - badLuckSurface->w / 2 ;
            rotrect.y -= rot->h / 2 - badLuckSurface->h / 2 ;
            SDL_BlitSurface( rot, NULL, screen, &rotrect );
         }

         else if( result == GAME_IS_DRAW ) {

            if( factor >= 2 ) {
               factor = 2;
               zoomshrink = -1;
            }
            if( factor <= 0 ) {
               factor = 0;
               zoomshrink = 1;
            }
            if( zoomshrink == 1 ) factor += 0.1;
            else factor -= 0.1;

            zoom = zoomSurface( drawSurface, factor, factor, 1 );
            SDL_Rect zoomrect = {400, 200, 0, 0};
            zoomrect.x -= zoom->w / 2 - drawSurface->w / 2 ;
            zoomrect.y -= zoom->h / 2 - drawSurface->h / 2 ;
            SDL_BlitSurface( zoom, NULL, screen, &zoomrect );
         }

         else if( result == HUMAN_WINS ) {

            angle += 10;
            if( angle >= 360 ) angle = angle - 360;

            if( factor >= 2 ) {
               factor = 2;
               zoomshrink = -1;
            }
            if( factor <= 0 ) {
               factor = 0;
               zoomshrink = 1;
            }
            if( zoomshrink == 1 ) factor += 0.1;
            else factor -= 0.1;

            rot = rotozoomSurface( goodSurface, angle, factor, 1 );
            SDL_Rect rotrect = {410, 200, 0, 0};
            rotrect.x -= rot->w / 2 - goodSurface->w / 2 ;
            rotrect.y -= rot->h / 2 - goodSurface->h / 2 ;
            SDL_BlitSurface( rot, NULL, screen, &rotrect );
         }


         SDL_Flip( screen );

         SDL_Delay( 50 );

         delay = SDL_GetTicks() - start;
         if ( (delay / 3000) > 1 ) break;

         restartIn = 5 - (int)( delay / 1000 );

      }//end of the second game loop

      result = 0;
      cellToSelect = 0;
      lastSelectedCell = 0;
      hitcell = -1;
      gameover = 0;
      Init_Xo1(&xo);

      if (trials == 2) {

         if (cumulresult < 0){ // cumulresult negatif

            text1 = strcpy( text1, "You loose   " );
            sprintf( text2, "%d", -cumulresult );

         }else if (cumulresult == 0){

            text1 = strcpy( text1, "The whole game was draw " );
            text2 = strcpy( text2, "!" );

         } else { // cumulresult positif

            text1 = strcpy( text1, "You win   " );
            sprintf( text2, "%d", cumulresult );

         }

         text3 = strcat( text1, text2 );
         text3 = strcat( text3, "  /  3" );

         restartSurface = TTF_RenderText_Solid( font, text3, swhitecreamy );

         SDL_BlitSurface( roundsback, NULL, screen, &bgPosRect );

         SDL_FillRect( screen, &finalRectangle, darkblue );

         SDL_BlitSurface( restartSurface, NULL, screen, &finaltextRectangle );

         SDL_Flip( screen );

         SDL_Delay(3000);

      }

      trials++;

   }  // end of THE GAME

   printf("\n\nEnd of the game, cumul = %d\n\n\n", cumulresult);


   if( textSurface != NULL ) SDL_FreeSurface( textSurface );
   if( goodSurface != NULL ) SDL_FreeSurface( goodSurface );
   if( badLuckSurface != NULL ) SDL_FreeSurface( badLuckSurface );

   Free_Xo( &xo );
   SDL_FreeSurface( back );

   //free( text1 );
   //free( text2 );
   //free( text3 );


   //SDL_Quit();

   return cumulresult;

}







//****************************************************************************************//

bool matrix_is_full( Xo* tac ) {    // valid for T_600   and   animation

   int count = 0;

   for( int i = 0; i < 3; i++ ) {
      for( int j = 0; j < 3; j++ ) {
         if( tac->tab[i][j] != 0 ) count++;
      }
   }
   return ( count == 9 );
}


bool matrix_has_one( Xo* tac ) {   // valid for T_600   and   animation

   int count = 0;

   for( int i = 0; i < 3; i++ ) {
      for( int j = 0; j < 3; j++ ) {
         if( tac->tab[i][j] != 0 ) count++;
      }
   }
   return ( count == 1 );
}


bool matrix_has_two( Xo* tac ) {   // valid for T_600   and   animation

   int count = 0;

   for( int i = 0; i < 3; i++ ) {
      for( int j = 0; j < 3; j++ ) {
         if( tac->tab[i][j] != 0 ) count++;
      }
   }
   return ( count == 2 );
}



bool matrix_is_empty( Xo* tac ) {   // valid for T_600   and   animation

   int count = 0;

   for( int i = 0; i < 3; i++ ) {
      for( int j = 0; j < 3; j++ ) {
         if( tac->tab[i][j] != 0 ) count++;
      }
   }
   return ( count == 0 );
}


//****************************************************************************************//
//****************************************************************************************//

int getLineValue( Xo* tic, int linecol ) {     // valid for T_600   and   animation

   if( linecol % 10 > 2 ) return 0;

   int l = linecol / 10;
   int c = linecol % 10;

   return tic->tab[l][c] + getLineValue( tic, linecol + 1 );

}

int getColValue( Xo* tic, int linecol ) {     // valid for T_600   and   animation

   if( linecol / 10 > 2 ) return 0;

   int l = linecol / 10;
   int c = linecol % 10;

   return tic->tab[l][c] + getColValue( tic, linecol + 10 );
}


int getDdiaglValue( Xo* tic, int linecol ) { // Direct diagonal       // valid for T_600   and   animation

   if( linecol > 22 ) return 0;

   int l = linecol / 10;
   int c = linecol % 10;

   return tic->tab[l][c] + getDdiaglValue( tic, linecol + 11 );
}


int getRdiaglValue( Xo* tic, int linecol ) { // Reverse diagonal       // valid for T_600   and   animation

   if( linecol > 20 ) return 0;

   int l = linecol / 10;
   int c = linecol % 10;

   return tic->tab[l][c] + getRdiaglValue( tic, linecol + 9 );
}


//****************************************************************************************//
//****************************************************************************************//



int bestshot( Xo* tic, int joueur ) {     // valid for T_600   and   animation

   int bestValueJ = -1000;
   int bestValue_J = 1000;

   int bestCoupJ = -1;
   int bestCoup_J = -1;
   int bestCoup = -1;

   int val;
   int coup;

   if( matrix_is_empty( tic ) ) return 11;
   if( matrix_has_one( tic ) && ( tic->tab[1][1] == 0 ) ) return 11;

   for( int i = 0; i < 3; i++ ) {

      for( int j = 0; j < 3; j++ ) {

         if( tic->tab[i][j] == 0 ) {

            // simulation joueur1
            tic->tab[i][j] = joueur;
            val = getValue( tic, i, j );
            coup = i * 10 + j;

            if( val > bestValueJ ) {
               bestValueJ = val;
               bestCoupJ = coup;
            }

            // retour à l'état original
            tic->tab[i][j] = 0;

            if( bestValueJ == 99 ) return bestCoupJ;

            // simulation joueur2
            tic->tab[i][j] = -joueur;
            val = getValue( tic, i, j );
            coup = i * 10 + j;

            if( val < bestValue_J ) {
               bestValue_J = val;
               bestCoup_J = coup;
            }

            // retour à l'état original
            tic->tab[i][j] = 0;

            if( bestValue_J == -99 ) return bestCoup_J;

            if( ( -bestValue_J ) > bestValueJ ) {
     //          bestValue = -bestValue_J;
               bestCoup = bestCoup_J;
            } else {
      //         bestValue = -bestValueJ;
               bestCoup = bestCoupJ;
            }

         }
      }
   }

   return bestCoup;
}



int getValue( Xo* tic, int line, int col ) {    // valid for T_600   and   animation

   int lvalue = 0;
   int cvalue = 0;
   int dvalue = 0;
   int rvalue = 0;
   //int value = 0;

   lvalue = getLineValue( tic, line * 10 );
   if( lvalue == -3 ) return -99;
   else if( lvalue == 3 ) return 99;

   cvalue = getColValue( tic, col );
   if( cvalue == -3 ) return -99;
   else if( cvalue == 3 ) return 99;


   if( line == col ) {
      dvalue = getDdiaglValue( tic, 0 );
      if( dvalue == -3 ) return -99;
      else if( dvalue == 3 ) return 99;
   }
   if( line + col == 2 ) {
      rvalue = getRdiaglValue( tic, 2 );
      if( rvalue == -3 ) return -99;
      else if( rvalue == 3 ) return 99;
   }

   return lvalue + cvalue + dvalue + rvalue;

}


//****************************************************************************************//
//****************************************************************************************//

//////////////////////////////////////////////////////////////////////////////////////////////////////

int getCellToSelect( int lastligcol, int direction){   // valid for T_600

   int lastlig, lastcol;
   int curligcol = 0;

   if ( lastligcol < 0 ) return 0;

   lastlig = lastligcol / 10;
   lastcol = lastligcol % 10;

   switch(direction){

      case TO_RIGHT:

         if (lastcol == 2 )
            curligcol = lastligcol - 2;
         else curligcol = lastligcol + 1;
         break;

      case TO_LEFT:

         if (lastcol == 0 )
            curligcol = lastligcol + 2;
         else curligcol = lastligcol - 1;
         break;

      case TO_DOWN:

         if (lastlig == 2 )
            curligcol = lastligcol - 20;
         else curligcol = lastligcol + 10;
         break;

      case TO_UP:

         if (lastlig == 0 )
            curligcol = lastligcol + 20;
         else curligcol = lastligcol - 10;
         break;

   }

   return curligcol;

}



void setCellRect(SDL_Rect* rect, int lig, int col){   // valid for T_600

   int cellx, celly;

   int originx = 188, originy = 70;

   //int hbloc = 152 + 38, vbloc = 143 + 35;
    int hbloc = 152 + 35, vbloc = 143 + 33;

   cellx = originx + col*hbloc;
   celly = originy + lig*vbloc;

   rect->x = cellx;
   rect->y = celly;
   rect->w = 152;
   rect->h = 143;

}



int getClickedCell(int mouseX, int mouseY){ // valid for T_600

   if ( (mouseX < 188) || (mouseX > 720) || (mouseY < 70) || (mouseY > 569)) return -1;

   int lig = -1;
   int col = -1;
   int ligcol = -1;

   if ( ( mouseX >= 188 ) && ( mouseX <= 340 ) ) col = 0;
   if ( ( mouseX >= 378 ) && ( mouseX <= 530 ) ) col = 1;
   if ( ( mouseX >= 568 ) && ( mouseX <= 720 ) ) col = 2;

   if ( ( mouseY >= 70 ) && ( mouseY <= 213 ) ) lig = 0;
   if ( ( mouseY >= 248 ) && ( mouseY <= 391 ) ) lig = 1;
   if ( ( mouseY >= 426 ) && ( mouseY <= 569 ) ) lig = 2;

   if ( (lig >= 0) && (col >= 0) ) {

      ligcol = lig*10 + col;
   }

   return ligcol;
}



int isCellFree( Xo* xo, int ligcol){  // valid for T_600

   if (ligcol < 0) return 0;

   int lig = ligcol / 10;
   int col = ligcol % 10;

   if( xo->tab[lig][col] == 0 ) return 1;

   return 0;
}


void setLineRect(Xo xo, SDL_Rect* rect, int ligcol, int lineType){   // valid for T_600

   if ( ligcol < 0 ) return;
   //if ( (lineType != UP_HLINE) && (lineType != DOWN_HLINE) ) return;

   int cellx, celly;

   int lig = ligcol / 10;
   int col = ligcol % 10;

   SDL_Rect xoRect = xo.xoR[lig][col];

   switch(lineType){

      case UP_HLINE:

         cellx = xoRect.x - 1;
         celly = xoRect.y - 1;
         rect->w = 153; // 152
         rect->h = 3;
         break;

      case DOWN_HLINE:

         cellx = xoRect.x - 1; // xoRect.x;
         celly = xoRect.y + 144;; // xoRect.y + 143;;
         rect->w = 154; // 152
         rect->h = 3;
         break;

      case LEFT_VLINE:

         cellx = xoRect.x - 1; // xoRect.x;
         celly = xoRect.y -1;  // xoRect.y
         rect->w = 3;
         rect->h = 145; // 143;
         break;

      case RIGHT_VLINE:

         cellx = xoRect.x + 151;  // xoRect.x + 152;
         celly = xoRect.y - 1; // xoRect.y;
         rect->w = 3;
         rect->h = 145; // 143;
         break;
   }

   rect->x = cellx;
   rect->y = celly;

}



void Init_Xo1( Xo *xo ) {  // valid for T_600 and animation

   xo->humanSurface = IMG_Load("human_anim.png");
   xo->machineSurface = IMG_Load("machine_anim.png");

   int lig, col;
   SDL_Rect xrect;

   for( lig = 0; lig < 3; lig++){
      for( col = 0; col < 3; col++){

         setCellRect(&xrect, lig, col);

         xo->xoR[lig][col].x = xrect.x;
         xo->xoR[lig][col].y = xrect.y;

         xo->xoR[lig][col].w = xrect.w;
         xo->xoR[lig][col].h = xrect.h;
      }
   }

   for( int i = 0; i < 6; i++ ){
      xo->animPosRect[i].x = 150*i;
      xo->animPosRect[i].y = 0;
      xo->animPosRect[i].w = 150;
      xo->animPosRect[i].h = 150;
   }


   for( int i = 0; i < 3; i++ ) {
      for( int j = 0; j < 3; j++ ) {
         xo->tab[i][j] = 0;
      }
   }

   xo->tour = 1;
   xo->winner = -9;
}


//****************************************************************************************//
//****************************************************************************************//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//           NON UTILISEs   -----------------------------------

int minimax( Xo *xo, int joueur ) {


   int gagnant, meilleure_valeur, valeur_coup;

   // Vérifier s'il y a un gagnant ou si la partie est terminée
   gagnant = Checkwin_Xo(xo);
   //gagnant = xo->winner;///////////////////////////////////////////////// ??????????????????????

   // Si la partie est terminée

   if( gagnant != 0 ) {
      if( ( joueur == 1 ) && ( gagnant == 1 ) )
         // Si la machine a gagné
         return 1;
      else if( ( joueur == -1 ) && ( gagnant == -1 ) )
         // Si le joueur humain a gagné
         return -1;
      else
         // c'est une partie nulle
         return 0;
   }


   // Initialiser la meilleure valeur en fonction du joueur actuel ( 1 = machine, -1 = humain )

   if( joueur == 1 ) meilleure_valeur = -1000; // Initialiser à une valeur très petite pour la maximisation
   else meilleure_valeur = 1000; // Initialiser à une valeur très grande pour la minimisation


   //Pour i de 1 à 9 faire // Pour chaque case vide de tabsuivi, simuler le coup et calculer sa valeur
   for( int i = 0; i < 3; i++ ) {

      for( int j = 0; j < 3; j++ ) {

         if( xo->tab[i][j] == 0 ) {

            xo->tab[i][j] = joueur; // Simuler le coup pour le joueur actuel

            // Calculer la valeur du coup en appelant récursivement minimax pour l'autre joueur

            valeur_coup = minimax( xo, -joueur );

            xo->tab[i][j] = 0; // Annuler le coup pour revenir à l'état précédent

            if( joueur == 1 ) { // Mettre à jour la meilleure valeur en fonction du joueur actuel

               // Maximisation pour la machine

               meilleure_valeur = maxi( meilleure_valeur, valeur_coup );

            } else {

               // Minimisation pour le joueur humain

               meilleure_valeur = mini( meilleure_valeur, valeur_coup );
            }

         }

      }

   }

   // Retourner la meilleure valeur trouvée après avoir exploré tous les coups possibles

   return meilleure_valeur;

}

int mini( int val1, int val2 ) {

   return val1 < val2 ? val1 : val2;

}


int maxi( int val1, int val2 ) {

   return val1 > val2 ? val1 : val2;

}

//  Cette fonction va mettre à jour le tabsuivi[i] à 1 si
//  le score rendu par la fonction minimax(tabsuivi,-1) est maximal.

int calcul_coup( Xo* xo ) {

   int meilleure_valeur, meilleur_coup, valeur_coup;
   int m, c;
   int best_hit_found = 0;

   meilleure_valeur = -1000; // Initialiser à une valeur très petite pour la maximisation

   meilleur_coup = -1; // Initialiser le meilleur coup à jouer

   for( int i = 0; i < 3; i++ ) {

      for( int j = 0; j < 3; j++ ) {

         if( xo->tab[i][j] == 0 ) {

            // Simuler le coup pour la machine (représentée par 1)

            xo->tab[i][j] = 1;

            // Calculer la valeur du coup en utilisant l'algorithme minimax

            valeur_coup = minimax( xo, -1 ); // Recherche de la valeur minimale pour le joueur humain

            // Annuler le coup pour revenir à l'état précédent

            xo->tab[i][j] = 0;

            // Mettre à jour le meilleur coup si la valeur du coup est meilleure

            if( valeur_coup > meilleure_valeur ) {

               meilleure_valeur = valeur_coup;
               meilleur_coup = i*10+j;///////////  changed to fit a 2D array
            }
         }

      }
   }


   // Jouer le meilleur coup trouvé

   if( meilleur_coup != -1 ) {

      m = (int)meilleur_coup / 10;
      c = meilleur_coup % 10;

      // Afficher le coup à jouer

      printf( "\nLe prochain coup à jouer est à la case [ %d , %d ]", m, c );

      // Mettre à jour le tableau avec le meilleur coup


      xo->tab[m][c] = 1; // Marquer la case comme jouée par la machine
      best_hit_found = 1;

      xo->tour = 1;

   } else {

      // Afficher un message d'erreur si aucun coup valide n'a été trouvé

      printf( "\nAucun coup valide trouvé.\n" );

   }

   return best_hit_found;

}

//       fin    NON UTILISEs   -----------------------------------
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//****************************************************************************************//
//****************************************************************************************//


void play_machine( Xo* xo, int x, int y ) { // machine = bot  ( values == -1   ,   tour = 1 )

   if( xo->tab[x][y] != 0 ) return;

   xo->tab[x][y] = 1;

   xo->tour = 1;
}



void play_human1( Xo* xo, int ligcol ) { // human = user  ( values == -1   ,   tour = 1 )     // valid for T_600

   int lig = ligcol / 10;
   int col = ligcol % 10;

   if( xo->tab[lig][col] != 0 ) return;

   xo->tab[lig][col] = -1;

   xo->tour = 2;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void Put_Xo( Xo* xo, int x, int y ) {  // valid for T_600

   xo->tab[x][y] = 1;
   xo->tour = 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void Diplay_Xo1( Xo* xo, SDL_Surface* ecran, int animpos ) {  // updated for animation    // valid for T_600

   if (animpos > 5) animpos = 0;

   for( int i = 0; i < 3; i++ ) {

      for( int j = 0; j < 3; j++ ) {

         if( xo->tab[i][j] == -1 ) {

            SDL_BlitSurface( xo->humanSurface,  &xo->animPosRect[animpos], ecran, &xo->xoR[i][j] );// human (-1)

         } else if( xo->tab[i][j] == 1 ) {

            SDL_BlitSurface( xo->machineSurface, &xo->animPosRect[animpos], ecran, &xo->xoR[i][j] );// machine (1)
         }
      }
   }
   SDL_Flip( ecran );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////

int Checkwin_Xo( Xo *xo ) {   // valid for T_600

   int i, j;

   int scoreLineP1 = 0;  //  line score for palayer1
   int scoreLineP2 = 0;  //  line score for palayer2

   int scoreColP1 = 0;  //  column score for palayer1
   int scoreColP2 = 0;  //  column score for palayer2

   int scoreDaig1P1 = 0;  //  first diagonal score for palayer1
   int scoreDaig1P2 = 0;  //  first diagonal score for palayer2

   int scoreDaig2P1 = 0;  //  second diagonal score for palayer1
   int scoreDaig2P2 = 0;  //  second diagonal score for palayer2


   //
   for( i = 0; i < 3; i++ ) {

      scoreLineP1 = scoreLineP2 = 0;
      scoreColP1 = scoreColP2 = 0;

      for( j = 0; j < 3; j++ ) {

         if( xo->tab[i][j] == -1 ) scoreLineP1 += xo->tab[i][j];
         else if( xo->tab[i][j] == 1 ) scoreLineP2 += xo->tab[i][j];

         if( xo->tab[j][i] == -1 ) scoreColP1 += xo->tab[j][i];
         else if( xo->tab[j][i] == 1 ) scoreColP2 += xo->tab[j][i];

         if( i == j ) {
            if( xo->tab[i][j] == -1 ) scoreDaig1P1 += xo->tab[i][j];
            else if( xo->tab[i][j] == 1 ) scoreDaig1P2 += xo->tab[i][j];
         }

         if( i + j  == 2 ) {
            if( xo->tab[i][j] == -1 ) scoreDaig2P1 += xo->tab[i][j];
            else if( xo->tab[i][j] == 1 ) scoreDaig2P2 += xo->tab[i][j];
         }
      }

      if( ( scoreLineP1 == -3 ) || ( scoreLineP2 == 3 ) ) break;
      if( ( scoreColP1 == -3 ) || ( scoreColP2 == 3 ) ) break;

   }

   if( ( scoreLineP1 == -3 ) || ( scoreColP1 == -3 ) || ( scoreDaig1P1 == -3 ) || ( scoreDaig2P1 == -3 ) ) return HUMAN_WINS;
   if( ( scoreLineP2 == 3 ) || ( scoreColP2 == 3 ) || ( scoreDaig1P2 == 3 ) || ( scoreDaig2P2 == 3 ) ) return MACHINE_WINS;

   return 0;

}

void Free_Xo( Xo *xo ){

   if ( xo->humanSurface != NULL) SDL_FreeSurface(xo->humanSurface);
   if ( xo->machineSurface != NULL) SDL_FreeSurface(xo->machineSurface);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
