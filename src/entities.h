#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "ofMain.h"

enum RoomExit
{
	StairsUp,
	StairsDown,
	DoorEast,
	DoorWest,
};

struct Room
{
	ofImage background;
	ofImage foreground;
};

struct Player
{
	ofVec2f pos;
	ofVec2f vel;

	ofImage step1;
	ofImage step2;

};

#endif//__ENTITIES_H__