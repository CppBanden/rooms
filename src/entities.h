#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "ofMain.h"

struct Player;
struct PlayerAction;
struct Room;

enum RoomExit
{
	RoomExit_StairsUp,
	RoomExit_StairsDown,
	RoomExit_DoorEast,
	RoomExit_DoorWest,
};

struct Room
{
	ofImage background;
	ofImage foreground;

	std::list<PlayerAction *> actions;
};

struct PlayerAction
{
	virtual PlayerAction * Update(Player * player);
};

enum PlayerState
{
	PlayerState_Walk,
	PlayerState_FindAction,
	PlayerState_PerformAction,
};

struct Player
{
	static const int spriteWalkCount = 12;
	int spriteWalkIndex = 0;

	ofImage spriteWalk[spriteWalkCount];// ofImage-array of spriteWalkCount elements
	ofImage spriteFaceBack;
	ofImage spriteFaceFront;

	Room * room;
	PlayerState state;
	PlayerAction * action;

	ofVec2f pos;
	ofVec2f vel;
	float facing;

	void Init(int spriteWidth, int spriteHeight);
	void SetFacing(float facing);
	void Update();
	void Draw(int offsetX, int offsetY);
};

#endif//__ENTITIES_H__