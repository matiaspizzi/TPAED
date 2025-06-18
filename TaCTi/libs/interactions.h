#ifndef INTERACTIONS_H_INCLUDED
#define INTERACTIONS_H_INCLUDED

#include "data.h"
#include "game.h"

int     update_menu             ();
int     update_enter_players    (tSession *s);
int     update_round            (tSession *s);
int     update_ranking          (tSession *s);
int     update_player_ready     (tSession *s, tPlays *p);
int     update_board            (tSession *s, tPlays *p);
int     update_game_over        (tSession *s, tPlays *p);
int     update_end_save         (tSession *s, tPlays *p);

#endif // INTERACTIONS_H_INCLUDED
