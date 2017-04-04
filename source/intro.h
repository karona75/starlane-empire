/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"flc_decoder.h"
#include"screen.h"
#include"video.h"

class gintro : public gscreen {
	private:
		//are we playing the first intro music ?
		bool first_music;
		flc_decoder *intro_video;
	public: 
		gintro();
		~gintro();
		void handle();
};
