/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"basic_rollover.h"

brollover::brollover(char *filename, SDL_Rect pos)
{
	position.x = pos.x;
	position.w = pos.w;
	position.y = pos.y;
	position.h = pos.h;


	img_deactivated = new image(filename);

	img_activated = img_deactivated->create_activated_image();
	
	img = img_deactivated;
}

/**constructor of basic rollover
 * \par filename : image name
 * \par pos : rollover position
 * \par x : x position of image inside rollover
 * \par y : y position of image inside rollover
 * */
brollover::brollover(char *filename, SDL_Rect pos, Uint32 x, Uint32 y)
{
	position.x = pos.x;
	position.w = pos.w;
	position.y = pos.y;
	position.h = pos.h;

	image *tmp = new image(filename);

	SDL_Surface *deactivated = SDL_CreateRGBSurface(SDL_SWSURFACE,
			pos.w,
			pos.h,
			game->screen->format->BitsPerPixel,
			game->screen->format->Rmask,
			game->screen->format->Gmask,
			game->screen->format->Bmask,
			game->screen->format->Amask);

	SDL_FillRect(deactivated, NULL, SDL_MapRGB(deactivated->format, 12, 32, 48));

	SDL_Rect imgpos;
	imgpos.x = x;
	imgpos.y = y;
	imgpos.w = tmp->get_surface()->w;
	imgpos.h = tmp->get_surface()->h;
	SDL_BlitSurface(tmp->get_surface(), NULL, deactivated, &imgpos);

	delete tmp;

	img_deactivated = new image(deactivated);

	img_activated = img_deactivated->create_activated_image();
	img = img_deactivated;
}




/**constructor
 * \par img2 : an already existing image (deactivated)
 * \par pos : position on screen
 * NOTE that img2 isn't copied so don't free it 
 * */
brollover::brollover(image *img2, SDL_Rect pos) {
	position.x = pos.x;
	position.y = pos.y;
	position.w = pos.w;
	position.h = pos.h;

	img_deactivated = img2;
	img_activated = img2->create_activated_image();
	img = img_deactivated;
}


/**constructor
 * \par string : a text string centered on rollover
 * \par font : font used
 * \par pos : position on screen
 * */
brollover::brollover(char *string, font *f, SDL_Rect pos) {
	position.x = pos.x;
	position.y = pos.y;
	position.w = pos.w;
	position.h = pos.h;

	//render text
	SDL_Surface *txt = f->render_text(string);
	
	//create final image, and center text
	SDL_Surface *deactivated = SDL_CreateRGBSurface(SDL_SWSURFACE,
			pos.w,
			pos.h,
			game->screen->format->BitsPerPixel,
			game->screen->format->Rmask,
			game->screen->format->Gmask,
			game->screen->format->Bmask,
			game->screen->format->Amask);

	SDL_FillRect(deactivated, NULL, SDL_MapRGB(deactivated->format, 12, 32, 48));

	SDL_Rect imgpos;
	imgpos.x = (pos.w - txt->w) / 2;
	imgpos.y = (pos.h - txt->h) / 2;
	imgpos.w = txt->w;
	imgpos.h = txt->h;
	SDL_BlitSurface(txt, NULL, deactivated, &imgpos);

	SDL_FreeSurface(txt);

	img_deactivated = new image(deactivated);

	//create activated image
	img_activated = img_deactivated->create_activated_image();
	img = img_deactivated;
}


brollover::~brollover()
{
	delete img_activated;
	delete img_deactivated;
}

/**we change default image to the one activated,
 */
void brollover::activate(Sint32 x, Sint32 y)
{
	img = img_activated;
	display();
}

/**we change default image to the one deactivated
 */
void brollover::deactivate()
{
	img = img_deactivated;
	display();
}

/**schedule the rollover for display when main loop next calls game->update()*/
void brollover::display()
{
	img->display(&position);
}

void brollover::click() {
}

/**this method checks if the mouse pointer is inside the rollover
 * \par x : x position of mouse
 * \par y : y position of mouse
 * \return true or false*/
Uint8 brollover::is_in(Sint32 x, Sint32 y) {
	if ((position.x <= x)&&(x <= position.x + position.w)&&(position.y <= y)&&(y <= position.y + position.h)) 
	{
		activate(x, y);
		return true;
	}
	else return false;
}

bool brollover::activated() {
	if (img==img_activated)
		return true;
	else
		return false;
}
