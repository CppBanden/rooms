#include "entities.h"

void Player::Init()
{
	// load sprites
	char filename[256];

	for (int i = 0; i != spriteWalkCount; i++)
	{
		sprintf(filename, "sprites/person_walk%d.png", i);
		spriteWalk[i].loadImage(filename);
	}

	// set initial position
		
	// set initial room

}

void Player::Update()
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