/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"common.h"
#include"image.h"

/**mouse class, for handling mouse graphics and display*/
class mouse {
	private:
		//!current mouse image
		image *img;
		//!x coordinate of pointer pixel in image
		Uint32 xdec;
		//!y coordinate of pointer pixel in image
		Uint32 ydec;
		//!last mouse coordinates
		SDL_Rect mousepos;
		//!arrays of real images
		//TODO: should be 12 images, see parser
		image *mouses[9];
		//!display mouse or not ?
		bool active;
	public:
		//!constructor
		mouse();
		//!destructor
		~mouse();
		//!handles moving and displaying
		void move(SDL_Event *event);
		//!handles moving and displaying
		void move(Sint32 x, Sint32 y);
		//!doesn't move but redraws mouse
		void display();
		//!changes the mouse image
		void change_image(Uint32 number);
		//!activate mouse handling
		void enable();
		//!deactivate mouse handling
		void disable();
		//!get current mouse x coordinate
		Sint16 get_x();
		//!get current mouse y coordinate
		Sint16 get_y();

};
