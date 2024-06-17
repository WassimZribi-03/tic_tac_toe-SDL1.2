#ifndef XO_H
#define XO_H

#define HUMAN_WINS -9
#define MACHINE_WINS 9
#define GAME_IS_DRAW 99

#define TO_RIGHT 1
#define TO_LEFT 2
#define TO_DOWN 3
#define TO_UP 4

#define UP_HLINE 10
#define DOWN_HLINE 20
#define LEFT_VLINE 30
#define RIGHT_VLINE 40

#define ROT_PLAY_GOOD 100
#define ROT_PLAY_BAD 200



typedef struct
{
    SDL_Surface* machineSurface;
    SDL_Surface* humanSurface;

    SDL_Rect animPosRect[6];

    SDL_Rect xoR[3][3];

    int tab[3][3];

    int tour;
    int winner;

}Xo;


int tictactoe( SDL_Surface* screen );

void Init_Xo1( Xo *xo );

int Checkwin_Xo(Xo *xo);

void Put_Xo(Xo* xo,int x,int y);

void Diplay_Xo1( Xo* xo, SDL_Surface* ecran, int animpos );

void Free_Xo(Xo *xo);
bool Play_Xo(SDL_Surface *ecran);

int minimax( Xo* xo, int joueur);
int mini(int val1, int val2);
int maxi(int val1, int val2);
int calcul_coup( Xo* xo );

void play_human1( Xo* xo, int ligcol );

void play_machine( Xo* xo, int x, int y );

int getClickedCell( int mouseX, int mouseY );

int isCellFree( Xo* xo, int ligcol );

int getCellToSelect( int lastligcol, int direction);


void setLineRect(Xo xo, SDL_Rect* rect, int ligcol, int lineType);

void setCellRect(SDL_Rect* rect, int lig, int col); // for fillig in XoR ( T_600 compatible )

bool matrix_is_empty( Xo* tac );
bool matrix_is_full( Xo* tac );
bool matrix_has_one( Xo* tac );
bool matrix_has_two( Xo* tac );

int getLineValue( Xo* tic, int linecol );
int getColValue( Xo* tic, int linecol );
int getDdiaglValue( Xo* tic, int linecol );
int getRdiaglValue( Xo* tic, int linecol );

int bestshot( Xo* tic, int joueur );
int getValue( Xo* tic, int line, int col );



#endif // XO_H
