/*
   Copyright (C) 2005 by Pablo Nicolás Núñez Pölcher <talosds@gmx.de>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GIMAGE
#define GIMAGE

class image_engine;
class image;

#include "ose.h"
#include "se.h"
#include "font.h"


class image {
	private:
		//!real image
		SDL_Surface *surface;

	public:
		//!build a black image
		image(Uint32 w, Uint32 h);
		//!build image from file
		image(char* filename);
		//!build image from SDL_Surface*
		image(SDL_Surface* existing_surface);
		//!free memory
		~image();
		//!create an activated image for a rollover
		image* create_activated_image();
		//!display on screen
		void display(SDL_Rect *pos);
		//!display in fullscreen
		void display();
		//!convert name
		static char* rename(char* name);
		//!get current surface
		SDL_Surface* get_surface();
		//!draw a box on our image 
		void draw_box(SDL_Rect box, SDL_Color color);
};

class image_engine {
	private:
		//!hash map to store all images
		hash_map<char*, image*, hash<char*>, eqstr> images;
		//!gamma of the whole screen
		float gamma;

	public:
		image_engine();
		~image_engine();
			
		//!load an image
		image* load(char* name);
		//!register an image to the engine
		int register_image(char* name, image* img);
		//!display an image on screen
		int display(char* name, SDL_Rect *pos);
		//!display an image in fullscreen
		int display(char* name);
		//!free an image
		void free(char* name);
		//!get image by name
		image* get(char* name);
		//!set gamma of the screen
		void set_gamma(float g);
		//!get gamma of the screen
		float get_gamma() { return gamma; }
};

/**on screen image, can be gfx or text*/
class image_container: public ose {
	private:
		//!should we free ancient image on image change ?
		bool free_image;
		//!image to display
		image* img;
	public:
		//!create a fullscreen container with a given image
		image_container(image* pimg);
		//!create a container with a given image
		image_container(SDL_Rect *pos, image* pimg);
		//!create a container with some text rendered
		image_container(Uint32 x, Uint32 y, char* text, font* tfont);
		//!create a container with some text rendered
		image_container(Uint32 x, Uint32 y, char* text, font* tfont, Uint32 linesize);
		//!create a container with an image taken from the engine
		image_container(SDL_Rect *pos, char* imagename);
		~image_container();
		//!change the text displayed 
		void set_text(char* text, font* tfont);
		//!change the text displayed with linesize
		void set_text(char* text, font* tfont, Uint32 linesize);
		//!display
		void display();
		//!bind to a screen
		void bind(gscreen *screen);
};

#endif
