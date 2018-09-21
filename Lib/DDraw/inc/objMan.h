#ifndef _OBJ_MAN_H_
#define _OBJ_MAN_H_

#include "graphic.h"

typedef int			HOTSPOT[2];

typedef struct tagENTITY{
	i16			posX, posY;
	IMAGE***	arBitmap;
	HOTSPOT**	arHotSpot;
}ENTITY;

#endif /*_OBJ_MAN_H_*/
