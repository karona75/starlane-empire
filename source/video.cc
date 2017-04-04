/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"video.h"

#define ReadU16(tmp1, tmp2) *((Uint16*)tmp1) = ((Uint8)*(tmp2+1)<<8)+(Uint8)*(tmp2);
#define ReadU32(tmp1, tmp2) *((Uint32*)tmp1) = (((((((Uint8)*(tmp2+3)<<8)+((Uint8)*(tmp2+2)))<<8)+((Uint8)*(tmp2+1)))<<8)+(Uint8)*(tmp2));


#undef DEBUG

/**loads .flc header and get ready for playback*/
video::video(char* filename) {
#ifdef DEBUG
	cout<<"Initializing video"<<endl;
#endif
	pMembuf = NULL;
	membufSize = 0;
	if(FlcCheckHeader(filename)) {
		cerr<<"error: wrong header in video: "<<filename<<endl;
		return;
	}
	
	mainscreen = SDL_CreateRGBSurface(SDL_SWSURFACE,
					game->screen->w,
					game->screen->h,
					8, 0, 0, 0, 0);

	initframe = game->frame;
	FlcInitFirstFrame();
}

int video::FlcCheckHeader(char *filename) {
	if ((file = fopen(filename, "rb"))==NULL) {
		return(1);
	}

	FlcReadFile(128);

	ReadU32(&HeaderSize, pMembuf);
	ReadU16(&HeaderCheck, pMembuf+4);
	ReadU16(&HeaderFrames, pMembuf+6);
	ReadU16(&HeaderWidth, pMembuf+8);
	ReadU16(&HeaderHeight, pMembuf+10);
	ReadU16(&HeaderDepth, pMembuf+12);
	ReadU16(&HeaderSpeed, pMembuf+16);

#ifdef DEBUG
  printf("flc.HeaderSize: %d\n", HeaderSize);
  printf("flc.HeaderCheck: %d\n", HeaderCheck);
  printf("flc.HeaderFrames: %d\n", HeaderFrames);
  printf("flc.HeaderWidth: %d\n", HeaderWidth);
  printf("flc.HeaderHeight: %d\n", HeaderHeight);
  printf("flc.HeaderDepth: %d\n", HeaderDepth);
  printf("flc.HeaderSpeed: %d\n", HeaderSpeed);
#endif

  if((HeaderCheck==0x0AF12) || (HeaderCheck==0x0AF11)) { 
    screen_w=HeaderWidth;
    screen_h=HeaderHeight;
    screen_depth=8;
    if(HeaderCheck==0x0AF11) {
      HeaderSpeed*=1000/70;
    }
    return(0);
  }
  return(1);
} 

void video::FlcReadFile(Uint32 size)
{ if(size>membufSize) {
    if(!(pMembuf = (Uint8*) realloc((void*)pMembuf, size+1))) {
      cerr<<"video: Realloc failed: "<<size<<endl;
      //TODO should'nt we exit instead of just return ?
      return;
    }
  }

  if(fread(pMembuf, sizeof(Uint8), size, file)==0) {
    cerr<<"video: Can't read flx file"<<endl;
    //TODO exit instead of return ?
    return;
  }
} 

//TODO: surely we could be more efficient than this qadh
void video::Framedrop(Uint32 number) {
	for(Uint32 i = 0 ; i < number ; i++) {
		FlcReadFile(FrameSize);
		FlcCheckFrame();
	}
}

void video::FlcInitFirstFrame() {
  FrameSize=16;
  FrameCount=0;
  if(fseek(file, 128, SEEK_SET)) {
    cerr<<"Fseek read failed"<<endl;
    //TODO: is return enough
    return;
  };
  FlcReadFile(FrameSize);
}

void video::handle() {
    FrameCount++;
    if(FlcCheckFrame()) {
      if (FrameCount<=HeaderFrames) {
        cerr<<"video: Frame failure -- corrupt file?"<<endl;
        exit(1);
      } 
      }

    if ((game->frame - initframe - FrameCount > 0)) { 
    	cout<<"Dropping "<<(game->frame - initframe - FrameCount)<<" frames"<<endl;
    //	Framedrop(game->frame - initframe - FrameCount);
    //	FrameCount += (game->frame - initframe - FrameCount);
    }

    FlcReadFile(FrameSize);

    //TODO: what's that test ?
    if(FrameCheck!=0x0f100) {
      FlcDoOneFrame();
      //game->display(mainscreen);
    }

}

int video::FlcCheckFrame()
{ pFrame=pMembuf+FrameSize-16;
  ReadU32(&FrameSize, pFrame+0);
  ReadU16(&FrameCheck, pFrame+4);
  ReadU16(&FrameChunks, pFrame+6);


#ifdef DEBUG
  printf("flc.FrameSize: %d\n", FrameSize);
  printf("flc.FrameCheck: %d\n", FrameCheck);
  printf("flc.FrameChunks: %d\n", FrameChunks);
#endif

  pFrame+=16;
  if(FrameCheck==0x0f1fa) { 
    return(0);
  }

  if(FrameCheck==0x0f100) { 
#ifdef DEBUG
    printf("Ani info!!!\n");
#endif
    return(0);
  }

  return(1);
} 

void video::FlcDoOneFrame()
{ int ChunkCount; 
  ChunkCount=FrameChunks;
  pChunk=pMembuf;
  if ( SDL_LockSurface(mainscreen) < 0 )
    return;
  while(ChunkCount--) {
    ReadU32(&ChunkSize, pChunk+0);
    ReadU16(&ChunkType, pChunk+4);

#ifdef DEBUG
    printf("flc.ChunkSize: %d\n", ChunkSize);
    printf("flc.ChunkType: %d\n", ChunkType);
#endif

    switch(ChunkType) {
      case 4:
        COLORS256();
      break;
      case 7:
        SS2();
      break;
      case 11:
        DECODE_COLOR();
      break;
      case 12:
        DECODE_LC();
      break;
      case 13:
        BLACK();
      break;
      case 15:
        DECODE_BRUN();
      break;
      case 16:
        DECODE_COPY();
      break;
      case 18:
#ifdef DEBUG
        cerr<<"Video:Chunk 18 not yet done."<<endl;
#endif
      break;
      default:
        printf("Ieek an non implemented chunk type: %d\n", ChunkType);
    }
    pChunk+=ChunkSize;
  }
  SDL_UnlockSurface(mainscreen);
} 

void video::COLORS256()
{ Uint8 *pSrc;
  Uint16 NumColorPackets;
  Uint16 NumColors;
  Uint8 NumColorsSkip;
  int i;

  pSrc=pChunk+6;
  ReadU16(&NumColorPackets, pSrc);
  pSrc+=2;
  while(NumColorPackets--) {
    NumColorsSkip=*(pSrc++);
    if(!(NumColors=*(pSrc++))) {
      NumColors=256;
    }
    i=0;
    while(NumColors--) {
      colors[i].r=*(pSrc++);
      colors[i].g=*(pSrc++);
      colors[i].b=*(pSrc++);
      i++;
    }
    SDL_SetColors(mainscreen, colors, NumColorsSkip, i);
  }
} 

void video::SS2()
{ Uint8 *pSrc, *pDst, *pTmpDst;
  Sint8 CountData;
  Uint8 ColumSkip, Fill1, Fill2;
  Uint16 Lines, Count;

  pSrc=pChunk+6;
  pDst=(Uint8*)mainscreen->pixels;
  ReadU16(&Lines, pSrc);
  pSrc+=2;
  while(Lines--) {
    ReadU16(&Count, pSrc);
    pSrc+=2;

    while(Count & 0xc000) {
/* Upper bits 11 - Lines skip 
*/
      if((Count & 0xc000)==0xc000) {  // 0xc000h = 1100000000000000
        pDst+=(0x10000-Count)*mainscreen->pitch;
      }

      if((Count & 0xc000)==0x4000) {  // 0x4000h = 0100000000000000
/* Upper bits 01 - Last pixel
*/
#ifdef DEBUG
            printf("Last pixel not implemented");
#endif
      }
      ReadU16(&Count, pSrc);
      pSrc+=2;
    }

    if((Count & 0xc000)==0x0000) {      // 0xc000h = 1100000000000000
      pTmpDst=pDst;
      while(Count--) {
        ColumSkip=*(pSrc++);
        pTmpDst+=ColumSkip;
        CountData=*(pSrc++);
        if(CountData>0) {
          while(CountData--) {
            *(pTmpDst++)=*(pSrc++);
            *(pTmpDst++)=*(pSrc++);
          }
        } else { 
          if(CountData<0) {
            CountData=(0x100-CountData);
            Fill1=*(pSrc++);
            Fill2=*(pSrc++);
            while(CountData--) {
              *(pTmpDst++)=Fill1;
              *(pTmpDst++)=Fill2;
            }
          }
        }
      }
      pDst+=mainscreen->pitch;
    } 
  }
} 

void video::DECODE_BRUN()
{ Uint8 *pSrc, *pDst, *pTmpDst, Fill;
  Sint8 CountData;
  int HeightCount, PacketsCount;

  HeightCount=HeaderHeight;
  pSrc=pChunk+6;
  pDst=(Uint8*)mainscreen->pixels;
  while(HeightCount--) {
    pTmpDst=pDst;
    PacketsCount=*(pSrc++);
    while(PacketsCount--) {
      CountData=*(pSrc++);
      if(CountData>0) {
        Fill=*(pSrc++);
        while(CountData--) {
          *(pTmpDst++)=Fill;
        }
      } else { 
        if(CountData<0) {
          CountData=(0x100-CountData);
          while(CountData--) {
          *(pTmpDst++)=*(pSrc++);
          }
        }
      }
    }
    pDst+=mainscreen->pitch;
  }
} 

void video::DECODE_LC() 
{ Uint8 *pSrc, *pDst, *pTmpDst;
  Sint8 CountData;
  Uint8 CountSkip;
  Uint8 Fill;
  Uint16 Lines, tmp;
  int PacketsCount;

  pSrc=pChunk+6;
  pDst=(Uint8*)mainscreen->pixels;

  ReadU16(&tmp, pSrc);
  pSrc+=2;
  pDst+=tmp*mainscreen->pitch;
  ReadU16(&Lines, pSrc);
  pSrc+=2;
  while(Lines--) {
    pTmpDst=pDst;
    PacketsCount=*(pSrc++);
    while(PacketsCount--) {
      CountSkip=*(pSrc++);
      pTmpDst+=CountSkip;
      CountData=*(pSrc++);
      if(CountData>0) {
        while(CountData--) {
          *(pTmpDst++)=*(pSrc++);
        }
      } else { 
        if(CountData<0) {
          CountData=(0x100-CountData);
          Fill=*(pSrc++);
          while(CountData--) {
            *(pTmpDst++)=Fill;
          }
        }
      }
    }
    pDst+=mainscreen->pitch;
  }
} 

void video::DECODE_COLOR()
{ Uint8 *pSrc;
  Uint16 NumColors, NumColorPackets;
  Uint8 NumColorsSkip;
  int i;

  pSrc=pChunk+6;
  ReadU16(&NumColorPackets, pSrc);
  pSrc+=2;
  while(NumColorPackets--) {
    NumColorsSkip=*(pSrc++);
    if(!(NumColors=*(pSrc++))) {
      NumColors=256;
    }
    i=0;
    while(NumColors--) {
      colors[i].r=*(pSrc++)<<2;
      colors[i].g=*(pSrc++)<<2;
      colors[i].b=*(pSrc++)<<2;
      i++;
    }
    SDL_SetColors(mainscreen, colors, NumColorsSkip, i);
  }
} 

void video::DECODE_COPY()
{ Uint8 *pSrc, *pDst;
  int Lines = screen_h;
  pSrc=pChunk+6;
  pDst=(Uint8*)mainscreen->pixels;
  while(Lines-- > 0) {
    memcpy(pDst, pSrc, screen_w);
    pSrc+=screen_w;
    pDst+=mainscreen->pitch;
  }
} 

void video::BLACK()
{ Uint8 *pDst;
  int Lines = screen_h;
  pDst=(Uint8*)mainscreen->pixels;
  while(Lines-- > 0) {
    memset(pDst, 0, screen_w);
    pDst+=mainscreen->pitch;
  }
} 

video::~video() {
	SDL_FreeSurface(mainscreen);
	fclose(file);
	free(pMembuf);
}
