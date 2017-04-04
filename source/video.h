/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"se.h"


/**this class implements the .flc video player
 * all code here is under a special license because borrowed from
 * flxplay by Jasper Berlijn
 * may he be thanked several times for taking that spine out of my foot.
 * here is the license :
 *
 * Don't care ware... ;)
 *
 * Something like: "you can do with it what you want to do with it. But if
 * your cola vaporises you can't sue me. But if you make changes to the code
 * you can always send to diffs to me"
 *
 * grt,
 *
 * - Jasper
 *
 *    +-----
 *    | Beheer Commisaris      | Homepage: http://www.il.fontys.nl/~jasper
 *    | IGV Interlink          | PGP-key:  finger jasper@il.fontys.nl      |
 */
class video {
	private:
		/**drop number video frames*/
		void Framedrop(Uint32 number);
		int FlcCheckHeader(char *filename);
		void FlcReadFile(Uint32 size);
		void FlcInitFirstFrame();
		int FlcCheckFrame();
		void FlcDoOneFrame();
		void COLORS256();
		void SS2();
		void DECODE_BRUN();
		void DECODE_LC();
		void DECODE_COLOR();
		void DECODE_COPY();
		void BLACK();
		
		FILE *file;
		Uint8 *pMembuf;
		Uint32 membufSize;
		Uint8 *pFrame;
		Uint8 *pChunk;
		/*
		*/
		Uint32 HeaderSize;    /* Fli file size */
		Uint16 HeaderCheck;   /* Fli header check */
		Uint16 HeaderFrames;  /* Number of frames in flic */
		Uint16 HeaderWidth;   /* Fli width */
		Uint16 HeaderHeight;  /* Fli heigth */
		Uint16 HeaderDepth;   /* Color depth */
		Uint16 HeaderSpeed;   /* Number of video ticks between frame */
		Uint32 FrameSize;     /* Frame size in bytes */
		Uint16 FrameCheck;    /* Frame check */
		Uint16 FrameChunks;   /* Number of chunks in frame */
		Uint32 ChunkSize;     /* Size of chunk */
		Uint16 ChunkType;     /* Type of chunk */
		/*
		*/
		SDL_Color colors[256];
		int screen_w;
		int screen_h;
		int screen_depth;
		SDL_Surface *mainscreen;
		Uint32 initframe;

	public:
		Uint16 FrameCount;    /* Frame Counter */
		video(char *filename);
		void handle();
		~video();
};

