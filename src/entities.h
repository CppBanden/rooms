#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "ofMain.h"

struct Player;
struct PlayerAction;
struct Room;

struct Room
{
	float opacity;
	ofImage bgImage;
	ofImage fgImage;

	std::list<PlayerAction *> actions;

	void Init(const char * bgFilename, const char * fgFilename);
	void Update();
	void DrawBack(int offsetX, int offsetY);
	void DrawFront(int offsetX, int offsetY);

	PlayerAction * FindAction(Player * player);
};

enum PlayerState
{
	PlayerState_Walk,
	PlayerState_FindAction,
	PlayerState_PerformAction,
};

struct PlayerAction
{
	PlayerState requiredState;
	ofVec2f pos;
	ofVec2f dim;

	virtual PlayerAction * Update(Player * player) = 0;
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

struct World
{
	Player * player;
	std::list<Room *> rooms;
};

#endif//__ENTITIES_H__