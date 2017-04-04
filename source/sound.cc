/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"sound.h"

/**sound engine class constructor. open sound card*/
sound::sound() {
	#ifdef DEBUG
	cout << "initializing sound" << endl;
	#endif
	sound_is_on = true;
	if (Mix_OpenAudio(44100, AUDIO_U8, 2, 512) < 0) {
		cerr << "Unable to initialize audio: " << SDL_GetError() << endl;
		sound_is_on = false;
	} 
}

/**sound engine class destructor. close sound card*/
sound::~sound() {
	Mix_CloseAudio();
}


/**play a music
 * \par name : the name of the music file to play
 * this method lookup for the name in the music hash table
 * to see if the music is already loaded
 * if no it loads the music and update hash table, before playing it*/
void sound::play_music(char* name) {
	if (sound_is_on == false) return;
	Mix_Music *current_music=NULL;
	current_music = musics[name];
	if (current_music==NULL) {
		//load music
		char *fullname = rename_music(name);
    	current_music = Mix_LoadMUS(fullname);
#ifdef DEBUG
		if (current_music == NULL) 
			cerr<<"Mix_LoadMUS("<<fullname<<") failed: "<<Mix_GetError()<<endl;
#endif
		musics[name] = current_music;
	}
	Mix_FadeInMusic(current_music, 0, 4000);
}

/**play a sound
 * \par name : the name of the sound file to play
 * this method lookup for the name in the sound hash table
 * to see if the sound is already loaded
 * if no it loads the sound and update hash table, before playing it*/
void sound::play_sound(char* name) {
	if (sound_is_on == false) return;
	Mix_Chunk *current_sound=NULL;
	current_sound = sounds[name];
	if (current_sound==NULL) {
		//load sound
		char *fullname = rename_sound(name);
    current_sound = Mix_LoadWAV(fullname);
		sounds[name] = current_sound;
	}
	Mix_PlayChannel(-1, current_sound, 0);
}

/**free some music
 * \par name : music name
 * lookup for a music in the hashtable and frees it if possible */
void sound::free_music(char* name) {
	Mix_Music *cm = musics[name];
	if (cm!=NULL) {
		Mix_FreeMusic(cm);
		musics[name] = NULL;
	}
}

/**free some sound
 * \par name : sound name
 * lookup for a sound in the hashtable and frees it if possible */
void sound::free_sound(char* name) {
	Mix_Chunk *cs = sounds[name];
	if (cs!=NULL) {
		Mix_FreeChunk(cs);
		sounds[name] = NULL;
	}
}

/**converts filename for matching installation path,
 * remember to free strings manually 
 * \param shortname: the relative filename*/
char* sound::rename_music(char* shortname) {
	Uint32 size;
	size = 1 + strlen("music/") + strlen(shortname);
	char* tmpname = (char *) malloc(size * sizeof(char));
	tmpname[0] = 0;
	strcat(tmpname, "music/");
	strcat(tmpname, shortname);
	return game->rename(tmpname);
}

/**converts filename for matching installation path,
 * remember to free strings manually 
 * \param shortname: the relative filename*/
char* sound::rename_sound(char* shortname) {
	Uint32 size;
	size = 1 + strlen("sounds/") + strlen(shortname);
	char* tmpname = (char *) malloc(size * sizeof(char));
	tmpname[0] = 0;
	strcat(tmpname, "sounds/");
	strcat(tmpname, shortname);
	return game->rename(tmpname);
}

