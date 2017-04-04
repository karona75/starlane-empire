/*
   Copyright (C) 2005 by Wagner Frederic <wagnerf@loria.fr>>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GRACERING
#define GRACERING
#include "ose.h"
#include <fstream>
#include "settings.h"

using namespace std;

/**racering: colored ring and race image*/
class racering : public ose {
	private:
		//!color number
		Uint32 color;
		//!race number
		Uint32 race;
		//!imagenames of Racelogos
		char* n[21];
		//!imagenames of colored rings
		char* r[7]; 	
		//!position of the race image
		SDL_Rect raceimg_position;
	public:
		//!new racering at given position
		racering(Uint32 x, Uint32 y);
		~racering();
		//!set ring color
		void set_color(Uint32 color_number);
		//!set race image
		void set_race(Uint32 race_number);
		//!display image
		void display();
		//!bind to a screen
		void bind(gscreen *screen);
};


/**stack_image: image that can change between several images*/
class stack_image : public ose {
	private:
		//!currently displayed image
		Uint32 current_level;
		//!number of images
		Uint32 max_level;
		//!imagenames 
		char** names;
	public:
		//!new stack_image at given position with given images names
		stack_image(Uint32 x, Uint32 y, Uint32 size, char** img_names);
		//!new stack_image at given position with special images names 
		stack_image(Uint32 x, Uint32 y, Uint32 size, char* basename);
		//!new stack_image at given position with very special images names 
		stack_image(Uint32 x, Uint32 y, Uint32 size, Uint32 colors, char* basename);
		~stack_image();
		//!set image to display
		void set_level(Uint32 level);
		//!go to next image
		void next_level();
		//!display image at current level
		void display();
		//!bind to a screen
		void bind(gscreen *screen);
};

/**race_texts:  class for holding all racesensitive strings*/
class race_texts
{
	private:
		//!Short description Texts for the races
		char** pstrShort_race_desc;
		//!File with Short Race description
		ifstream ifShort_race_desc;
		//QADH little helper
		char dirty_little_helper[210];
		settings *settings_lang;

	public:
		//!new race_texts with specified language
		race_texts(char* lang);
		//!Destructor
		~race_texts();
		//!load the short description texts for the races
		void load_short_race_desc(char* file);
		//!deliver the selected description
		char* get_short_race_desc(Uint32 index);
};


#endif
