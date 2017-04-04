/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GSTARLANE
#define GSTARLANE
//se main class
#include"common.h"
#include"mouse.h"
#include"screen.h"
#include"font.h"
#include"settings.h"
#include"sound.h"
#include"image.h"
#include"rules.h"

/** this is the main game class. All objects are stored in it. */
class se {
	private:
		//!screen intro
		gscreen *sintroduction;
		//!screen video intro
		gscreen *sintro;
		//!screen mainmenu
		gscreen *smainmenu;
		//!new game creation
		gscreen *scfgnew;
		//!lua state
		lua_State *lst; //drogen wie bei uns
		//!switch opengl in 2D
		static void gl_enable2D();
		//!switch opengl in 3D
		static void gl_disable2D();
		//!selected Language is current from sector "lang" out of settings.xml
		char* language;
		
		Uint8 intro_running;

		
	public:
		//!screen pointer to currently displayed game part
		gscreen *gamescreen;
		/// screen (use LOCK for a direct access)
		SDL_Surface *screen;
		//! should we update screen or not ?
		bool screen2update;
		/// last number of ticks
		Uint32 ticks;
		/// current frame playing
		Uint32 frame;
		/// number of ticks between two frames
		Uint32 framerate;
		//!pointer on mouse
		mouse *mousepointer;
		//!big font
		font *bigfont;
		//!small font
		font *smallfont;
		//!sound engine for both music and effects control
		sound *sound_engine;
		//!image engine
		image_engine *images;
		//!rules engine
		rules_engine *rules;
		/// constructor
		se();
		/// destructor
		~se();
		//! loads an image 
		SDL_Surface* image_load(char *filename);
		//! loads one image in different races colors
		SDL_Surface** color_images_load(char *filename);
		/// updates the screen
		void update();
		//! ends the game
		void quit();
		//! changes char* filename using real datapath
		char* rename(char* shortname);
		//! toggles fullscreen on/off
		void fullscreen();
		//! wrapper to game->gamescreen->handle()
		void handle();
		//! starts introduction
		void introduction();
		//! starts mainmenu
		void mainmenu();
		//! launches new game
		void cfgnew();
		//! launches intro video
		void intro();
		//!execute a lua script
		void lua(char* filename);
		//!register a function to lua
		void lua_reg(char* name, lua_CFunction function);
		//!returns the name of a given race
		char* get_racename(Uint32 number);
		//!returns the selected Language
		char* get_language();
		//!sets the selected Language out of settings.xml
		void set_language(char* lang);
};

extern se *game;
#endif
