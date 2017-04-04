/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GSCREEN
#define GSCREEN
#include"common.h"

class gscreen;

#include"rollover.h"
/**this class is the main game class.
 * Each part of the game will inherit from this class
 * Only one will be selected at a given time in main loop 
 * virtual methods will be differents in each screen of the game*/
class gscreen {
	protected:
		SDL_Event event;
		//!all things displayed on this screen
		vector<ose*> oses;
		//!all rollovers displayed on this screen
		vector<rollover*> rollovers;
		//!currently focused rollover
		rollover* focused_rollover;
	public:
		gscreen();
		virtual ~gscreen();
		//!add a rollover to rollover_list
		void add_rollover(rollover* r);
		//!add an ose to the list
		void add_ose(ose* o);
		//!update rollovers state according to mouse position
		Uint8 handle_rollover(Sint32 x, Sint32 y);
		//!display the screen image
		void display();
		//!this handles bases common event (eg fullscreen on/off, ...)
		virtual void handle();
};
#endif
