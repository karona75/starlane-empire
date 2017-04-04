/*
   Copyright (C) 2005 by Pablo Nicol� N�z P�cher <talosds@gmx.de>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "image.h"

/* Helper functions. */
int _round(double x)
{
	return (int)(x + 0.5);
}

int nextpoweroftwo(int x)
{
	double logbase2 = log(x) / log(2);
	return _round(pow(2,ceil(logbase2)));
}

image_engine::image_engine() {};
image_engine::~image_engine() {};

/**loads an image by filename and register it to the engine
 * \par char* name : local filename 
 * returns image in case of success
 * returns the ancient image if an image is already registered under that name
 * ireturns NULL if it isn't possible to find such an image
 * */
image* image_engine::load(char* name) {
	//first check image 
	image* test = images[name];
	if (test == NULL) {
		//try to load image
		test = new image(name);
		if (test == NULL) {
			//load failed
			cerr<<"Warning : image "<<name<<" failed to load"<<endl;
			return NULL;
		}
		else {
			//everything ok, register image
			images[name] = test;
			return test;
		}
	}
	else {
		cerr<<"Warning : image "<<name<<" is already registered"<<endl;
		return test;
	}
}

/** display an image on screen
 * if image isn't loaded try to load it first
 * \par char *name : name under which the image is registered
 * \par SDL_Rect *pos : position on screen
 * return 0 if everything is ok
 * return 1 if image isn't available
 * */
int image_engine::display(char* name, SDL_Rect *pos) {
	//get image by name
	image *img = images[name];
	if (img == NULL) {
#ifdef DEBUG
		cerr<<"image_engine::display warning: loading image first"<<endl;
#endif
		img = load(name);
		if (img == NULL)
			return 1;
	}
	//display it 
	img->display(pos);
	return 0;
}

/** display an image in fullscreen
 * \par char *name : name under which the image is registered
 * return 0 if everything is ok
 * return 1 if image isn't available
 * */
int image_engine::display(char* name) {
	//get image by name
	image *img = images[name];
	if (img == NULL) {
#ifdef DEBUG
		cerr<<"image_engine::display warning: loading image first"<<endl;
#endif
		img = load(name);
		if (img == NULL) return 1;
	}
	//display it 
	img->display();
	return 0;
}

/**gets a registered image by its name
 * \par char* name : its name
 * tries to load the image if not registered
 * returns NULL if not possible to load it*/
image* image_engine::get(char* name) {
	image* res = images[name];
	if (res == NULL)
		res = load(name);
	return res;
}

/** register an image under a name
 * \par char* name : the name given to it
 * \par image* img : the image to register
 * returns 0 if everything is ok
 * returns 1 if an image is already registered to this name*/
int image_engine::register_image(char* name, image* img) {
	image *test = images[name];
	if (test != NULL) return 1;
	images[name] = img;
	return 0;
}

/**free an image in memory and de-register it
 * \par char* name : the name of the image to delete
 */
void image_engine::free(char* name) {
	image* img = images[name];
	if (img == NULL) {
		cerr<<"Warning : freeing the non existent image: "<<name<<endl;
		return;
	}
	delete img;
	images[name] = NULL;
}

/**builds a black image (usefull for backgrounds)
 * \par Uint32 w : width
 * \par Uint32 h : height */
image::image(Uint32 w, Uint32 h) {
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
				 w,
				 h,
				 game->screen->format->BitsPerPixel,
				 game->screen->format->Rmask,
				 game->screen->format->Gmask,
				 game->screen->format->Bmask,
				 game->screen->format->Amask);
	
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
}


/**load an image from a local filename
 * \par char* filename : the name of the image */
image::image(char* filename) {
	char* fullname = rename(filename);
	SDL_Surface *s = IMG_Load(fullname);
	if (s == NULL) {
		cerr << "Warning : unable to load : " << fullname << " : " <<
			SDL_GetError() << endl;
	}
	free(fullname);
	//convert image to screen format
	surface = SDL_DisplayFormat(s);
	SDL_FreeSurface(s);
	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 0, 254, 0));
}

/**build the image using an already existant surface
 * \par SDL_Surface* existing surface : the existing surface
 * NO COPY, don't free the existing surface afterwards, don't register it twice to the engine
 * */
image::image(SDL_Surface* existing_surface) {
	surface = existing_surface;
}

/**free an image
 */
image::~image() {
	delete surface;
}

/**converts image filename for matching images installation path,
 * remember to free string manually afterwards
 * \param char* name: the relative image filename*/
char* image::rename(char* name) {
	Uint32 size;
	int dash_position = -1;
	//see if we can match a '_' for names like 02_image.png
	for(unsigned int i = 0 ; i < strlen(name) ; i++)
		if (name[i]=='_') {
			dash_position = i;
			break;
		}
	char* realname;
	if (dash_position == -1) {
		//standard image name
		size = 1 + strlen("graphx/") + strlen(name);
		realname = (char *) malloc(size * sizeof(char));
		realname[0] = 0;
		strcat(realname, "graphx/");
		strcat(realname, name);
	} else {
		//image extracted using ascendancy's parser
		size = 1 + strlen("graphx/") + strlen(name) - (dash_position+1) - 4 + strlen(name); //4 is for ".png"
		realname = (char *) malloc((1+size) * sizeof(char)); //+1 for \0
		realname[0] = 0;
		strcat(realname, "graphx/");
		strcat(realname, name+dash_position+1);
		realname[size - strlen(name)-1] = '/';
		realname[size - strlen(name)] = 0;
		strcat(realname, name);
	}
	return game->rename(realname);
}


/**display image on screen at the given position
 * scale the image if necessary
 * \par SDL_Rect* position : screen coordinates*/
void image::display(SDL_Rect* position)
{
 	SDL_Surface	*intermediary;
 	int		w, h;
 	GLuint		texture;

 	// Calculate width and height of the intermediary image.
 	w = nextpoweroftwo(surface->w);
 	h = nextpoweroftwo(surface->h);

 	// Create an intermediary image with the required size.
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	intermediary = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0xff000000,
		0x00ff0000, 0x0000ff00, 0x000000ff);
#else
	intermediary = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0x000000ff,
		0x0000ff00, 0x00ff0000, 0xff000000);
#endif

	// Blit the image to the intermediary buffer.
	SDL_BlitSurface(surface, 0, intermediary, 0);

	// Generate a texture and bind it.
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	// Use linear filters. (not the nicest quality, but more
	// than acceptable).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, intermediary->pitch / intermediary->format->BytesPerPixel);

	// Bind the image to the texture.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, intermediary->pixels);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	// Enable texturing.
	glEnable(GL_TEXTURE_2D);

	// Bind the texture.
	glBindTexture(GL_TEXTURE_2D, texture);

 	// Set the intensity of the colours.
 	glColor3f(game->images->get_gamma(), game->images->get_gamma(), game->images->get_gamma());
 
	// Draw the image.
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex2f(position->x, game->screen->h - position->y);
	glTexCoord2f((float)position->w / w, 0);
	glVertex2f((float)position->w + position->x, game->screen->h - position->y);
	glTexCoord2f((float)position->w / w, (float) position->h / h);
	glVertex2f((float)position->w + position->x, game->screen->h - ((float) position->h + position->y));
 	glTexCoord2f(0, (float)position->h / h);
 	glVertex2f(position->x, game->screen->h - ((float)position->h + position->y));
 	glEnd();

 	// Finish before releasing the texture (important!).
 	glFinish();

 	// Free the texture surface.
	SDL_FreeSurface(intermediary);

	// Delete the texture.
	glDeleteTextures(1, &texture);
}

/**display image in fullscreen
 * */
void image::display() {
	SDL_Rect pos;
	pos.x = 0;
	pos.y = 0;
	pos.w = game->screen->w;
	pos.h = game->screen->h;
	display(&pos);
}

/** create an image which is a copy of us, with background activated
 * */
image* image::create_activated_image() {
	SDL_Surface *img_activated = SDL_CreateRGBSurface(SDL_SWSURFACE,
					 surface->w,
					 surface->h,
					 game->screen->format->BitsPerPixel,
					 game->screen->format->Rmask,
					 game->screen->format->Gmask,
					 game->screen->format->Bmask,
					 game->screen->format->Amask);
	//copy img from deactivated

	SDL_BlitSurface(surface, NULL, img_activated, NULL);

	//now change blue in activated image

	if (SDL_MUSTLOCK(img_activated))
		SDL_LockSurface(img_activated);

	switch (img_activated->format->BitsPerPixel) {
		case 16:
			{
			Uint16 lightblue = SDL_MapRGB(img_activated->format, 16, 68, 74);
			Uint16 darkblue = SDL_MapRGB(img_activated->format, 12, 32, 48);
			Uint16 *pixels = (Uint16*) img_activated->pixels;

			for (Sint32 i = 0; i < img_activated->h; i++)
				for (Sint32 j = 0; j < img_activated->w; j++)
					if (pixels[img_activated->pitch * i / 2 + j] == darkblue)
						pixels[img_activated->pitch * i / 2 + j] = lightblue;
			}
			break;
		case 32:
			{
			Uint32 lightblue = SDL_MapRGB(img_activated->format, 16, 68, 74);
			Uint32 darkblue = SDL_MapRGB(img_activated->format, 12, 32, 48);
			Uint32 *pixels = (Uint32*) img_activated->pixels;

			for (Sint32 i = 0; i < img_activated->h; i++)
				for (Sint32 j = 0; j < img_activated->w; j++)
					if (pixels[img_activated->pitch * i / 4 + j] == darkblue)
						pixels[img_activated->pitch * i / 4 + j] = lightblue;

			}
			break;
		default: 
			cerr<<"warning : create_activated_image : pixel format not supported"<<endl;
	}

	if (SDL_MUSTLOCK(img_activated))
		SDL_UnlockSurface(img_activated);

	image *result = new image(img_activated);
	return result;

}

SDL_Surface* image::get_surface() {
	return surface;
}

/**create an image object on screen
 * \par pos : screen position
 * \par pimg : the image (not freed on image change)*/
image_container::image_container(SDL_Rect *pos, image *pimg) {
	free_image = false;
	position.x = pos->x;
	position.y = pos->y;
	position.w = pos->w;
	position.h = pos->h;
	img = pimg;
}

/**create an image object on screen
 * \par Uint32 x : x coordinate
 * \par Uint32 y : y coordinate
 * \par char* text : the text to display
 * \par font* tfont : the font to use
 * images are freed if the text is modified*/
image_container::image_container(Uint32 x, Uint32 y, char* text, font* tfont) {
	free_image = true;
	img = new image(tfont->render_text(text));
	position.x = x;
	position.y = y;
	position.w = img->get_surface()->w;
	position.h = img->get_surface()->h;
}

/**create an image object on screen
 * \par Uint32 x : x coordinate
 * \par Uint32 y : y coordinate
 * \par char* text : the text to display
 * \par font* tfont : the font to use
 * \par Uint32 linesize : maximum size of line in pixel
 * images are freed if the text is modified*/
image_container::image_container(Uint32 x, Uint32 y, char* text, font* tfont, Uint32 linesize) {
	free_image = true;
	img = new image(tfont->render_text(text, linesize));
	position.x = x;
	position.y = y;
	position.w = img->get_surface()->w;
	position.h = img->get_surface()->h;
}

/**create an image object on screen
 * the image is retrieved from the image engine
 * \par SDL_Rect* pos : screen position
 * \char *imagename : name of the image to search for in the engine */
image_container::image_container(SDL_Rect *pos, char* imagename) {
	free_image = false;
	position.x = pos->x;
	position.y = pos->y;
	position.w = pos->w;
	position.h = pos->h;
	img = game->images->get(imagename);
}

/**create an image object on screen
 * element is fullscreen
 * \par pimg : the image (not freed on image change)*/
image_container::image_container(image* pimg) {
	free_image = false;
	position.x = 0;
	position.y = 0;
	position.w = game->screen->w;
	position.h = game->screen->h;
	img = pimg;
}

image_container::~image_container() {
	if (free_image == true) delete img;
}

/**change the image into some text
 * \par char* text : the text to use
 * \par font* tfont : the font to use 
 * note that this updates the container width and height*/
void image_container::set_text(char* text, font* tfont) {
	if (free_image == true) delete img;
	free_image = true;
	img = new image(tfont->render_text(text));
	position.w = img->get_surface()->w;
	position.h = img->get_surface()->h;
}

/**change the image into some text with linesize
 * \par char* text : the text to use
 * \par font* tfont : the font to use 
 * \par Uint32 linesize : maximum size of line in pixel
 * note that this updates the container width and height*/
void image_container::set_text(char* text, font* tfont, Uint32 linesize) {
	if (free_image == true) delete img;
	free_image = true;
	img = new image(tfont->render_text(text, linesize));
	position.w = img->get_surface()->w;
	position.h = img->get_surface()->h;
}

/**display the element*/
void image_container::display() {
	img->display(&position);
}

/**register this image to a screen*/
void image_container::bind(gscreen *screen) {
	screen->add_ose(this);
}

void image_engine::set_gamma(float g) {
	// Truncate gamma values if out of range.
	if(g < 0.0f) g = 0.0f;
	if(g > 1.0f) g = 1.0f;

	// Set gamma value.
	gamma = g;
}

/**draw a 1 pixel thin box on image
 * \par SDL_Rect box : box coordinates
 * \par SDL_Color color : line color
 * */
void image::draw_box(SDL_Rect box, SDL_Color color) {

	//direct pixels acces, lock image
	if (SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);

	switch(surface->format->BitsPerPixel) {
		case 16:
			{
			Uint16 col = SDL_MapRGB(surface->format, color.r, color.g, color.b);
			Uint16 *pixels = (Uint16*) surface->pixels;

			//draw horizontal lines
			for (Sint32 i = box.x; i < box.x + box.w; i++) {
				pixels[surface->pitch * box.y / 2 + i] = col;
				pixels[surface->pitch * (box.y + box.h - 1) / 2 + i] = col;
			}
			//draw vertical lines
			for (Sint32 j = box.y; j < box.y + box.h; j++) {
				pixels[surface->pitch * j / 2 + box.x] = col;
				pixels[surface->pitch * j / 2 + box.x + box.w - 1] = col;
			}
			}
			break;
		case 32:
			{
			Uint32 col = SDL_MapRGB(surface->format, color.r, color.g, color.b);
			Uint32 *pixels = (Uint32*) surface->pixels;

			//draw horizontal lines
			for (Sint32 i = box.x; i < box.x + box.w; i++) {
				pixels[surface->pitch * box.y / 4 + i] = col;
				pixels[surface->pitch * (box.y + box.h - 1) / 4 + i] = col;
			}
			//draw vertical lines
			for (Sint32 j = box.y; j < box.y + box.h; j++) {
				pixels[surface->pitch * j / 4 + box.x] = col;
				pixels[surface->pitch * j / 4 + box.x + box.w - 1] = col;
			}
			}
			break;
		default:
		cerr<<"warning : direct pixel access not implement for this bpp"<<endl;
	}

	//finished, unlock
	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
	
	return;
}
