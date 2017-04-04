/*
   Copyright (C) 2005 by Wagner Frederic <wagnerf@loria.fr>>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GRULES
#define GRULES
#include "common.h"

/**player class : everything that belongs to a given player*/
class player {
	private:
		Uint32 race;
		Uint32 color;
	public:
		//!add a new player
		player(Uint32 player_race, Uint32 player_color);
		//!kill kill kill
		~player();
};


/**rules engine : controlling the game behaviour*/
class rules_engine {
	private:
		//TODO: add a list of all available races
		player** players;
		Uint32 difficulty;
		//!total number of players in game
		Uint32 num_players;
		//!get a random race not already used
		Uint32 get_random_race();
		//!get a random color not already used
		Uint32 get_random_color();
	public:
		//!single player init
		rules_engine(Uint32 players_number, Uint32 player_race, Uint32 player_color, Uint32 gdifficulty);
		//!shutdown whole game
		~rules_engine();
};


#endif
