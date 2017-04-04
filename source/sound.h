/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef GSOUND
#define GSOUND
#include"common.h"

class sound;

#include"se.h"


/**sound engine*/
class sound {
	private:
		//!hash table to retrieve all musics
		hash_map<char*, Mix_Music*, hash<char*>, eqstr> musics;
	    //!hash table to retrieve all sounds
		hash_map<char*, Mix_Chunk*, hash<char*>, eqstr> sounds;
		//!do we have sound enabled ?
		bool sound_is_on;
		//!rename a music file path from local to global
		char* rename_music(char* shortname);
		//!rename a sound file path from local to global
		char* rename_sound(char* shortname);

	public:
		//!constructor
		sound();
		//!destructor
		~sound();
		//!play music
		void play_music(char* name);
		//!stop music
		void stop_music();
		//! play sound effect
		void play_sound(char* name);
		//!free music memory
		void free_music(char* name);
		//!free sound memory
		void free_sound(char* name);
};
#endif
