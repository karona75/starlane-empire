/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef CROLL
#define CROLL
#include"rollover.h"

/**this class handles the simplests basic rollover buttons*/
class crollover : public rollover {
	private:
		//! currently activated image (when mouse focused)
		image *img_activated;
		//! currently deactivated image (when mouse isn't over us)
		image *img_deactivated;
		//! activated images
		image **imgs_activated;
		//! deactivated images
		image **imgs_deactivated;
		//! pointer to real image to use (one of the activated or deactivated)
		image *img;
		//! current race image number
		Uint32 current_image;
		//! color currently selected number
		Uint32 current_color;
	public:
		//! constructor (everything in static)
		crollover();
		//! destructor
		~crollover();
		//! deactivate rollover and display
		void deactivate();
		//! activate rollover
		void activate(Sint32 x, Sint32 y);
		//! display on screen
		void display();
		//! click action
		Uint8 click(Sint32 x, Sint32 y);
		//! are coordinates inside of rollover ?
		Uint8 is_in(Sint32 x, Sint32 y);
		//! are we activated ?
		bool activated();
		//! set current image (race logo)
		void set_image(Uint32 level);	
		//! set color (modify number, image, and redisplay)
		void set_color(Uint32 color);
		//give the selected Image plz (for example raceflag)
		SDL_Surface* get_img();
};

#endif
