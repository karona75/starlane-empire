/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"intro.h"

#include <GL/gl.h>

gintro::gintro() {
	char* filename = game->rename("intro65a.flc");
	intro_video = new flc_decoder;
	intro_video->open(filename);
	game->sound_engine->play_music("intro00.ogg");
	first_music = true;
	game->framerate = 125;
}

gintro::~gintro() {
	game->sound_engine->free_music("intro00.ogg");
	game->sound_engine->free_sound("intro01.ogg");
	delete intro_video;
}

void gintro::handle() {

	intro_video->handle();
	intro_video->display();

	if((intro_video->getcurrentframe() >= 1020)&&(first_music)) {
		cout<<"CHANGING MUSIC at "<<intro_video->getcurrentframe()<<endl;
		first_music = false;
		game->sound_engine->play_music("intro01.ogg");
		game->framerate = 140;
	}
	if(intro_video->getcurrentframe() >= 1130) {
		game->mainmenu();
	}
}


