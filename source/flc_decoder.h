/*
   Copyright (C) 2005 by Pablo Nicolás Núñez Pölcher <talosds@gmx.de>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __FLC_DECODER_H
#define __FLC_DECODER_H

#include <iostream>
#include <fstream>

#include <GL/gl.h>
#include <SDL/SDL.h>

#define FLI_COLOR256	4
#define FLI_SS2		7
#define FLI_COLOR64	11
#define	FLI_LC		12
#define FLI_BLACK	13
#define FLI_BRUN	15
#define	FLI_COPY	16

using namespace std;

typedef struct {
	Uint32	size;		/* Size of FLIC incluiding this header */
	Uint16	type;		/* File type 0xAF11, 0xAF12, 0xAF30, 0xAF44, ... */
	Uint16	frames;		/* Number of frames in first segment */
	Uint16	width;		/* FLIC frame width in pixels */
	Uint16	height;		/* FLIC frame height in pixels */
	Uint16	depth;		/* Bits per pixel (usually 8) */
	Uint16	flags;		/* Set to zero or to three */
	Uint16	speed;		/* Delay between frames */
	Uint16	reserved1;	/* Set to zero */

	Uint32	created;	/* Date of FLIC creation (FLC only) */
	Uint32	creator;	/* Serial number or compiler id (FLC only) */
	Uint32	updated;	/* Date of FLIC update (FLC only) */
	Uint32	updater;	/* Serial number (FLC only), see creator */

	Uint16	aspect_dx;	/* Width of square rectangle (FLC only) */
	Uint16	aspect_dy;	/* height of square rectangle (FLC only) */
	Uint16	ext_flags;	/* EGI: flags for specific EGI extensions */
	Uint16	keyframes;	/* EGI: key-image frequency */
	Uint16	totalframes;	/* EGI: total number of frames (segments) */
	Uint32	req_memory;	/* EGI: maximum chunk size (uncompressed) */
	Uint16	max_regions;	/* EGI: max. number of regions in a CHK_REGION chunk */
	Uint16	transp_num;	/* EGI: number of transparent levels */
	Uint8	reserved2[24];	/* Set to zero */
	Uint32	oframe1;	/* Offset to frame 1 (FLC only) */
	Uint32	oframe2;	/* Offset to frame 2 (FLC only) */
	Uint8	reserved3[40];	/* Set to zero */
} FLIC_HEADER;

typedef struct {
	Uint8	red, green, blue;
} FLIC_PALETTE;

class flc_decoder {

		private:

			ifstream *flc_stream;
			SDL_Surface *image_surface;
			FLIC_PALETTE *palette;
			FLIC_HEADER *header;
			char *chunk_buffer;
			Uint32 chunk_pointer;
			Uint16 frame_number;
			Uint32 last_ticks;
			Uint32 flc_speed;
			GLint *vPort;

			void decode_color64();
			void decode_color256();
			void decode_ss2();
			void decode_black();
			void decode_brun();
			void decode_copy();
			void decode_lc();

			void decode_chunk();

		public:

			flc_decoder();
			~flc_decoder();

			int handle();
			int open(const char *filename);
			void display();
			int getcurrentframe() { return frame_number; }
};

#endif
