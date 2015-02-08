#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "ofMain.h"

struct Player;
struct PlayerAction;
struct Room;

struct Room
{
	bool dark;
	float opacity;
	float opacityText;
	ofImage bgImage;
	ofImage fgImage;
	ofImage txImage;
	ofVec2f pos;

	std::list<PlayerAction *> actions;

	void Init(const char * bgFilename, const char * fgFilename, const char * txFilename);
	void Update();
	void DrawBack();
	void DrawActions();
	void DrawFront();
	void DrawText();

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

	virtual void Start(Player * player) = 0;
	virtual PlayerAction * Update(Player * player) = 0;
};

struct Player
{
	static const int spriteWalkCount = 12;
	int spriteWalkIndex;

	ofImage spriteWalk[spriteWalkCount];// ofImage-array of spriteWalkCount elements
	ofImage spriteWalkDark[spriteWalkCount];// ofImage-array of spriteWalkCount elements
	ofImage spriteFaceBack;
	ofImage spriteFaceBackDark;
	ofImage spriteFaceFront;
	ofImage spriteFaceFrontDark;

	Room * room;
	PlayerState state;
	PlayerAction * action;

	ofVec2f pos;
	ofVec2f vel;
	float facing;

	ofSoundPlayer footsteps[8];
	int footstepCount;
	int footstepIndex;

	void Init();
	void Footstep();
	void SetFacing(float facing);
	void Update();
	void Draw();
};

struct World
{
	Player * player;
	std::list<Room *> rooms;
};

#endif//__ENTITIES_H__