/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef MROLL
#define MROLL
#include"rollover.h"

/**this class handles rollover menues*/
class mrollover : public rollover {
	private:
		//! all menu entries
		rollover** entries;
		//! pointer to real entries in use
		rollover** visible_entries;
		//! number of entries visible
		Uint8 num_vis_entries;
		//! number of entries
		Uint8 num_entries;
		//!first visible entry (initially 0)
		Uint8 first_entry;
		//! position on screen
		SDL_Rect position;
		//! up arrow
		rollover *up_arrow;
		//! down arrow
		rollover *down_arrow;
		//! builds from several images 
		void init(Uint8 visible_elements, Uint8 elements, Uint32 x, Uint32 y, SDL_Surface** images);
		//! deactivate all visible menu entries
		void deactivate_visibles();
		//! height of an entry in pixels
		Uint32 entry_height;
		//!currently focused entry
		Sint32 focused_entry;
	public:
		//! constructor from several images files
		mrollover(Uint8 visible_elements, Uint8 elements, Uint32 x, Uint32 y, char** files);
		//! constructor from several images files with a given background
		mrollover(Uint8 visible_elements, Uint8 elements, Uint32 x, Uint32 y, char** files, SDL_Surface* background);
		//! destructor
		~mrollover();
		//! deactivate rollover and display
		void deactivate();
		//! activate rollover
		void activate(Sint32 x, Sint32 y);
		//! display on screen
		void display();
		//! click action
		void click();
		//! are coordinates inside of rollover ?
		Uint8 is_in(Sint32 x, Sint32 y);
		//! returns the number of the entry currently focused
		Sint32 get_focused_entry();
};
#endif
