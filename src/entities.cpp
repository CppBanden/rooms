#include "entities.h"
#include "ofApp.h"

void Player::Init(int spriteWidth, int spriteHeight)
{
	// load sprites
	char filename[256];
	for (int i = 0; i != spriteWalkCount; i++)
	{
		sprintf(filename, "sprites/person_walk%d.png", i >> 1);
		spriteWalk[i].loadImage(filename);
		spriteWalk[i].getPixelsRef().resize(spriteWidth, spriteHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
		spriteWalk[i].update();
	}

	spriteFaceBack.loadImage("sprites/person_back.png");
	spriteFaceBack.getPixelsRef().resize(spriteWidth, spriteHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
	spriteFaceBack.update();

	spriteFaceFront.loadImage("sprites/person_front.png");
	spriteFaceFront.getPixelsRef().resize(spriteWidth, spriteHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
	spriteFaceFront.update();

	// set initial position
	pos = ofVec2f(0.0f, 0.0f);
	vel = ofVec2f(0.0f, 0.0f);
	facing = 1.0f;

	// set initial room, state, action
	room = nullptr;
	state = PlayerState_Walk;
	action = nullptr;
}

void Player::SetFacing(float facing)
{
	if (this->facing == facing)
		return;

	this->facing = facing;

	for (int i = 0; i != spriteWalkCount; i++)
	{
		spriteWalk[i].mirror(false, true);
	}

	spriteFaceBack.mirror(false, true);
	spriteFaceFront.mirror(false, true);
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
				if (pressedL || pressedR)
				{
					SetFacing(pressedL ? -1.0f : 1.0f);
					vel.x = facing * (ofApp::scalingFactor * 0.25f);
				}
				else
				{
					vel.x = 0.0f;
				}

				pos.x += vel.x;
				//TODO: test if we pass through an exit

				if (vel.x == 0.0f)
				{
					spriteWalkIndex = 0;
				}
				else
				{
					spriteWalkIndex++;
					spriteWalkIndex %= spriteWalkCount;
				}
			}
		} break;

		case PlayerState_FindAction:
		{
			if (pressedA)
			{
				//TODO: find action
				action = nullptr;//room->FindAction(this);
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
			if (action == nullptr)
			{
				state = PlayerState_FindAction;
			}
		} break;
	}
}

void Player::Draw(int offsetX, int offsetY)
{
	float x = offsetX + pos.x;
	float y = offsetY + pos.y;

	ofImage * image = nullptr;

	switch (state)
	{
	case PlayerState_Walk: image = &spriteWalk[spriteWalkIndex]; break;
	case PlayerState_FindAction: image = &spriteFaceBack; break;
	case PlayerState_PerformAction: image = &spriteFaceBack; break;
	}

	if (image != nullptr)
	{
		image->draw(x, y);
	}
}