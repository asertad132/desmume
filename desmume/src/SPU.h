/*  Copyright (C) 2006 Theo Berkau

    This file is part of DeSmuME

    DeSmuME is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    DeSmuME is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DeSmuME; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef SPU_H
#define SPU_H

#include "types.h"
#include <iosfwd>

#define SNDCORE_DEFAULT         -1
#define SNDCORE_DUMMY           0
#define SNDCORE_FILEWRITE       1

typedef struct
{
   int id;
   const char *Name;
   int (*Init)(int buffersize);
   void (*DeInit)();
   void (*UpdateAudio)(s16 *buffer, u32 num_samples);
   u32 (*GetAudioSpace)();
   void (*MuteAudio)();
   void (*UnMuteAudio)();
   void (*SetVolume)(int volume);
} SoundInterface_struct;

extern SoundInterface_struct SNDDummy;
extern SoundInterface_struct SNDFile;

struct channel_struct
{
	int num;
   u8 vol;
   u8 datashift;
   u8 hold;
   u8 pan;
   u8 waveduty;
   u8 repeat;
   u8 format;
   u8 status;
   u32 addr;
   u16 timer;
   u16 loopstart;
   u32 length;
   s8 *buf8;
   s16 *buf16;
   double sampcnt;
   double sampinc;
   // ADPCM specific
   int lastsampcnt;
   s16 pcm16b, pcm16b_last;
   int index;
   u16 x;
   s16 psgnoise_last;
} ;

class SPU_struct
{
public:
	SPU_struct(int buffersize);
   u32 bufpos;
   u32 buflength;
   s32 *sndbuf;
   s16 *outbuf;
   u32 bufsize;
   channel_struct channels[16];

   void reset();
   ~SPU_struct();
   void KeyOn(int channel);
   void WriteByte(u32 addr, u8 val);
   void WriteWord(u32 addr, u16 val);
   void WriteLong(u32 addr, u32 val);
   
   //kills all channels but leaves SPU otherwise running normally
   void ShutUp();
};

int SPU_ChangeSoundCore(int coreid, int buffersize);
SoundInterface_struct *SPU_SoundCore();

int SPU_Init(int coreid, int buffersize);
void SPU_Pause(int pause);
void SPU_SetVolume(int volume);
void SPU_Reset(void);
void SPU_DeInit(void);
void SPU_KeyOn(int channel);
void SPU_WriteByte(u32 addr, u8 val);
void SPU_WriteWord(u32 addr, u16 val);
void SPU_WriteLong(u32 addr, u32 val);
u32 SPU_ReadLong(u32 addr);
void SPU_Emulate_core(void);
void SPU_Emulate_user(void);

extern SPU_struct *SPU_core, *SPU_user;
extern int spu_core_samples;

void spu_savestate(std::ostream* os);
bool spu_loadstate(std::istream* is);

#endif
