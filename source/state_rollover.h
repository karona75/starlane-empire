/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef SROLL
#define SROLL
#include"rollover.h"

/**this class handles rollover buttons which may have different states toggled*/
class srollover : public rollover {
	private:
		//! activated surfaces
		image** imgs_activated;
		//! deactivated surfaces
		image** imgs_deactivated;
		//! pointer to real surface in use
		image* img;
		//! current state number
		Uint8 state;
		//! total number of states
		Uint8 states_number;
	public:
		//! constructor from several images files
		srollover(Uint8 states, Uint32 x, Uint32 y, char* legend, char** files);
		//! destructor
		~srollover();
		//! deactivate rollover and display
		void deactivate();
		//! activate rollover
		void activate(Sint32 x, Sint32 y);
		//! display on screen
		void display();
		//! click action
		void click();
		//! are coordinates inside of rollover ?
		Uint8 is_in(Sint32 x, Sint32 y);
		//! get current state
		Uint8 get_state();
};
#endif
