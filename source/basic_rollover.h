/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef BROLL
#define BROLL
#include"rollover.h"

/**this class handles the simplests basic rollover buttons*/
class brollover : public rollover {
	private:
		//! activated image (when mouse focused)
		image *img_activated;
		//! deactivated image (when mouse isn't over us)
		image *img_deactivated;
		//! pointer to real surface to use (one of the activated or deactivated)
		image *img;
	public:
		//! constructor from image file
		brollover(char *filename, SDL_Rect pos);
		//! constructor from image file, with image smaller than roll
		brollover(char *filename, SDL_Rect pos, Uint32 x, Uint32 y);
		//! constructor from surface
		brollover(image *img2, SDL_Rect pos);
		//! constructor from text string
		brollover(char *string, font *f, SDL_Rect pos);
		//! destructor
		~brollover();
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
		//! are we activated ?
		bool activated();

};

#endif
