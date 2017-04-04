/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef STARMAP
#define STARMAP
#include"se.h"


typedef struct sstar {
	Uint16 r;
	double phi;
	double theta;
	int iStartype;
} star;

//For testing i need a hard coded Startype
const int STARTYPE = 9;
 

/**this class handles the star mapping system*/
class starmap : public rollover {
	private:
		//!radius
		Uint16 r;
		//!horizonthal rotation
		double phi;
		//!vertical rotation
		double theta;

		//!main image surface
		SDL_Surface* img;

		//!number of stars 
		Uint16 stars_number;

		//!stars table
		star *stars;

		//first star image
		//TODO: replace with selfmade Stars
		SDL_Surface* star_img[10][4];
		
	public:
		//! random initial constructor
		starmap(Uint16 sn, SDL_Rect pos);
		//! destructor
		~starmap();
		//! deactivate (does little)
		void deactivate();
		//! activate (for now does little)
		void activate(Sint32 x, Sint32 y);
		//! display on screen 
		void display();
		//! click action
		void click();
		//! are coordinates inside us ?
		Uint8 is_in(Sint32 x, Sint32 y);
		//! are we activated ?
		bool activated();
		//! set r
		void set_r(Uint16 new_r);
		//! set phi
		void set_phi(double new_phi);
		//! set theta
		void set_theta(double new_theta);
		//! get back current value of phi
		double get_phi();
		//! render all stars on image
		void update();
		//! reinitialize with a different number of stars
		void reinit(Uint16 sn);
};

#endif
