/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GFONT
#define GFONT
#include"common.h"
#include <SDL_ttf.h>

/**this class handles each chars of a font individually,
 * necessary since height differs from char to char*/
class character {
	public:
		//! image builded 
		SDL_Surface *img;
		//! width of char
		int x;
		//! height of char
		int y;
		//!constructor from font FILE positioned correctly
		character(TTF_Font* font, SDL_Color clrFg, SDL_Color clrBg, char i);
		//!destructor
		~character();
};

class font {
	private:
		//!height of all chars
		int y;
		//!pointers on all chars
		character* chars[96];
		TTF_Font* starFont;
	public:
		//!constructor (constructs also all characters)
		font(char* filename, SDL_Color foreground, SDL_Color background, int fontsize);
		//!destructor
		~font();
		//!main use of this class
		SDL_Surface *render_text(char* text);
		//!multiline text rendering
		SDL_Surface *render_text(char* text, Uint32 linesize);
};

#endif
