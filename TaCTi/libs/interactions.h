#ifndef INTERACTIONS_H_INCLUDED
#define INTERACTIONS_H_INCLUDED



int     update_menu             ();
int     update_enter_players    (tSession *s);
int     update_round            (tSession *s);
int     update_ranking          (tSession *s);
int     update_player_ready     (tSession *s, tPlays *p);
int     update_board            (tSession *s, tPlays *p);

#endif // INTERACTIONS_H_INCLUDED
