/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"se.h"
using namespace std;
/** this is the main game variable */
se *game;

int main(int argc, char **argv)
{
	Uint32 new_ticks, diff;
#ifdef DEBUG
    std::cout << "Starting game" << std::endl;
#endif
	//open video, sound, bugs buffer, ....
	game = new se;

	//start by introduction
	game->introduction();
	//what time is it doc ?
	game->ticks = SDL_GetTicks();

	//now enter main game loop (forever pick finger with prickle)
	while (true) {
		//handle the game (or try to)
		game->handle();
		//update the screen
		game->update();

		//how many time elapsed ?
		new_ticks = SDL_GetTicks();
		diff = new_ticks - game->ticks;
		game->ticks = new_ticks;
		//were we quick enough ??
		if (diff < game->framerate) {
			//yes, haha i've got an 1.8Ghz Athlon
			//let's give the extra time back to linux
			SDL_Delay(game->framerate - diff);
			game->frame++;
		} else {
			//no, this bloated box would go faster on wheels
			//TODO: should we sleep or not???
			SDL_Delay(game->framerate - (diff % game->framerate));
			game->frame += 1 + (Uint32) (diff / game->framerate);
		}
	}
}
