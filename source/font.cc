/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"font.h"
#include"se.h"
#include <SDL_ttf.h>

/**constructs the different letters images.
 * \param font : font FILE positioned on current char
 * \param c1 : color in 16bits (game depth)
 * \param c2 : color in 16bits (game depth) 
 * \param y : height of char (at beginning of FILE) */

character::character(TTF_Font* font, SDL_Color clrFg, SDL_Color clrBg, char letter) {
         //j'ai toujours dit que programmer � forgeait le charact�e
         //bon j'ai honte elle est mauvaise celle-l
	char text[2];
	text[0]=(letter+32);
	text[1]='\0';
	TTF_SizeText(font, text, &x, &y);
	img = TTF_RenderText_Solid(font, text, clrFg);
}

/**frees surface, blah blah blah*/
character::~character() {
	SDL_FreeSurface(img);
}

/**constructs all chars*/
font::font(char* filename, SDL_Color foreground, SDL_Color background, int fontsize) {
	//first : correct file name
	char* realname = game->rename(filename);
	
	//open file
	// Load the Font
	starFont = TTF_OpenFont(realname, fontsize);
	
	//free the name
	free(realname);

	//now read all chars
	for (Uint32 i = 0; i < 96 ; i++)
	{
		chars[i] = new character(starFont, foreground, background,(char) i);
	}
	TTF_CloseFont(starFont);
}

/**frees all mem*/
font::~font() {
	Uint32 i;
	for(i=0 ; i < 96; i++)
		delete chars[i];
}

/**this method renders the text using the color allocated
 * in the font*/
SDL_Surface *font::render_text(char* text) {
	SDL_Surface* final_img = NULL;
	Uint32 final_x = 0;
	y = chars[1]->y;
	//first go through all letters to calculate size
	for(Uint32 i = 0; i < strlen(text) ; i++) {
		final_x += chars[text[i]-32]->x;
	}
	//now create rendered string and fill it
	final_img = SDL_CreateRGBSurface(SDL_SWSURFACE, final_x, y, game->screen->format->BitsPerPixel, game->screen->format->Rmask, game->screen->format->Gmask, game->screen->format->Bmask, game->screen->format->Amask);
	SDL_Rect src, dest;
	src.x = 0;
	src.y = 0;
	src.h = y;
	dest.x = 0;
	dest.y = 0;
	dest.w = 0;
	dest.h = y;
	for(Uint32 i = 0 ; i < strlen(text) ; i++) {
		src.w = chars[text[i]-32]->x;
		dest.w = src.w;

		SDL_BlitSurface(chars[text[i]-32]->img, &src, final_img, &dest);
		dest.x += chars[text[i]-32]->x;
	}

	//now set colorkey as the usual color
	// was 0,254,0
	SDL_SetColorKey(final_img, SDL_SRCCOLORKEY, SDL_MapRGB(final_img->format, 0, 0, 0));

	return final_img;
};


/**this method renders some texts, wrapping words at end of lines
 * \par char* text : the text to use
 * \par Uint32 linesize : maximum size of line in pixel
 */
SDL_Surface *font::render_text(char* text, Uint32 linesize) {
	SDL_Surface* final_img = NULL;
	Uint32 final_x = 0;
	Uint32 current_x = 0;
	Uint32 final_y = chars[1]->y;
	//regexp marker
	bool in_word = false;
	//mark the beginning of the currently parsed word
	Uint32 word_start = 0;
	//counter to check we're not looping (see below)
	Uint32 word_start2 = 0;
	Uint32 i = 0;
	y = chars[1]->y;

	while(text[i] != 0) {
		//check if we're on space
		if (text[i]==32) 
		{
			if (in_word) in_word = false;
		}
		else 
		{
			if (!in_word) 
			{
				in_word = true;
				word_start = i;
			}
		}

		current_x += chars[text[i]-32]->x;
		//check if we should jump to next line
		if (current_x > linesize) 
		{
			current_x = 0;
			final_y += y + 2;
			//wordstart - 1 because we add 1 at the end of the loop
			if (text[i] != 32)
				i = word_start-1;
			in_word = false;
			//check if we're not looping
			if (word_start2 == word_start)
			{
				cerr<<"font::render_text : word too big to fit in line !!"<<endl;
				return NULL;
			}
			word_start2 = word_start;
			final_x = linesize;
		}
		else final_x = (final_x>current_x)?final_x:current_x;
		i++;
	}

	//ready to go now
	//allocate image

	final_img = SDL_CreateRGBSurface(SDL_SWSURFACE, final_x, final_y, game->screen->format->BitsPerPixel, game->screen->format->Rmask, game->screen->format->Gmask, game->screen->format->Bmask, game->screen->format->Amask);

	//fill image with correct background color
	SDL_FillRect(final_img, NULL, SDL_MapRGB(final_img->format, 0, 0, 0));

	//start the same regexp, blitting chars to surface
	
	i = 0;
	word_start = 0;
	current_x = 0;
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.h = y;
	
	while (text[i]!=0) {
		//check if we're on space
		if (text[i]==32) {
			if (in_word)  {
				in_word = false;
				for (Uint32 j = word_start; j < i ; j++ ) {
					dest.w = chars[text[j] - 32]->x;
					SDL_BlitSurface(chars[text[j] - 32]->img, NULL, final_img, &dest);
					dest.x += chars[text[j] - 32]->x;
				}
			}
			dest.x += chars[0]->x;
		}
		else {
			if (!in_word) {
				in_word = true;
				word_start = i;
			}
		}
		current_x += chars[text[i]-32]->x;
		//check if we should jump to next line
		if (current_x > linesize) {
			current_x = 0;
			if (text[i] != 32)
				i = word_start - 1;
			in_word=false;
			dest.y += y + 2;
			dest.x = 0;
		}
		i++;
	}

	//eventually print last word
	
	if (in_word)  {
		for (Uint32 j = word_start; j < i ; j++ ) {
			dest.w = chars[text[j] - 32]->x;
			SDL_BlitSurface(chars[text[j] - 32]->img, NULL, final_img, &dest);
			dest.x += chars[text[j] - 32]->x;
		}
	}
	
	//now set colorkey as the usual color
	
	SDL_SetColorKey(final_img, SDL_SRCCOLORKEY, SDL_MapRGB(final_img->format, 0, 254, 0));
	
	return final_img;
}
