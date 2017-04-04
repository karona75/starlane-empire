/*
   Copyright (C) 2005 by Wagner Frederic <wagnerf@loria.fr>>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/*all races images classes : images changing with color
 * and race */

#include "rules.h"

player::player(Uint32 player_race, Uint32 player_color) {
#ifdef DEBUG
	cerr<<"creating a new player : race: "<<player_race<<" color: "<<player_color<<endl;
#endif
	race = player_race;
	color = player_color;
	//TODO: get a home planet from starmap
}

player::~player(){}

rules_engine::rules_engine(Uint32 players_number, Uint32 player_race, Uint32 player_color, Uint32 gdifficulty) {
#ifdef DEBUG
	cerr<<"starting a new game with "<<players_number<<" players and "<<gdifficulty<<" for difficulty"<<endl;
#endif
	num_players = players_number;
	difficulty = gdifficulty;
	//allocate players
	players = (player**) malloc (num_players * sizeof(player*));
	players[0] = new player(player_race, player_color);
	for(Uint32 i = 1; i < num_players ; i++) {
		players[i] = new player(get_random_race(), get_random_color());
	}
}

rules_engine::~rules_engine() {
	for(Uint32 i = 0 ; i < num_players ; i++)
		delete players[i];
	free((void*)players);
}

Uint32 rules_engine::get_random_race() {
	//TODO: do something
	return 0;
}

Uint32 rules_engine::get_random_color() {
	//TODO: do something
	return 0;
}
