#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "ofMain.h"

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

enum PlayerState
{
	PlayerState_Walk,
	PlayerState_FindAction,
	PlayerState_PerformAction,
};

struct PlayerAction
{
	virtual PlayerAction * Update(Player * player);
};

struct Player
{
	static const int spriteWalkCycle = 6;
	static const char* spriteWalkFiles[] = {
		"sprites/person_walk0.png",
		"sprites/person_walk1.png",
		"sprites/person_walk2.png",
		"sprites/person_walk3.png",
		"sprites/person_walk4.png",
		"sprites/person_walk5.png",
	};
	int spriteWalkIndex = 0;

	ofImage spriteWalk[spriteWalkCycle];// ofImage-array of spriteWalkCycle elements
	ofImage spriteFaceBack;
	ofImage spriteFaceFront;

	Room * room;
	PlayerState state;
	PlayerAction * action;

	ofVec2f pos;
	ofVec2f vel;

	void Init()
	{
		// load sprites

		// set initial position
		
		// set initial room
	}
	
	void Update()
	{
		bool pressedL = ofGetKeyPressed(OF_KEY_LEFT);// left
		bool pressedR = ofGetKeyPressed(OF_KEY_RIGHT);// right
		bool pressedA = ofGetKeyPressed(' ');// action!

		if (pressedL && pressedR)
		{
			pressedL = false;
			pressedR = false;
		}

		switch (state)
		{
			case PlayerState_Walk:
			{
				if (pressedA)
				{
					state = PlayerState_FindAction;
				}
				else
				{
					if (pressedL)
						vel.x = -1.0f;
					if (pressedR)
						vel.x = 1.0f;

					pos.x += vel.x;
					//TODO: test if we pass through an exit
				}
			} break;

			case PlayerState_FindAction:
			{
				if (pressedA)
				{
					//TODO: find action
					//action = room->FindAction(this);
					action = nullptr;
					if (action != nullptr)
					{
						state = PlayerState_PerformAction;
					}
				}
				else
				{
					state = PlayerState_Walk;
				}
			} break;

			case PlayerState_PerformAction:
			{
				if (action != nullptr)
				{
					action = action->Update(this);
				}

				if (action == nullptr);
				{
					state = PlayerState_FindAction;
				}
			} break;
		}
	}
};

#endif//__ENTITIES_H__