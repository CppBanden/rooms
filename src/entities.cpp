#include "entities.h"
#include "ofApp.h"

void Player::Init(int spriteWidth, int spriteHeight)
{
	// load sprites
	spriteWalkIndex = 0;
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
	room = NULL;
	state = PlayerState_Walk;
	action = NULL;
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
				action = NULL;//room->FindAction(this);
				if (action != NULL)
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
			if (action != NULL)
			{
				action = action->Update(this);
			}
			if (action == NULL)
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

	ofImage * image = NULL;

	switch (state)
	{
	case PlayerState_Walk: image = &spriteWalk[spriteWalkIndex]; break;
	case PlayerState_FindAction: image = &spriteFaceBack; break;
	case PlayerState_PerformAction: image = &spriteFaceBack; break;
	}

	if (image != NULL)
	{
		image->draw(x, y);
	}
}

void Room::Init(const char * bgFilename, const char * fgFilename)
{
	opacity = 0.0f;

	if (bgFilename != NULL)
	{
		bgImage.loadImage(bgFilename);
		bgImage.getPixelsRef().resize(ofApp::backgroundWidth, ofApp::backgroundHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
		bgImage.update();
	}

	if (fgFilename != NULL)
	{
		fgImage.loadImage(fgFilename);
		fgImage.getPixelsRef().resize(ofApp::backgroundWidth, ofApp::backgroundHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
		fgImage.update();
	}
}

void Room::Update()
{
}

void Room::DrawBack(int offsetX, int offsetY)
{
	if (!bgImage.isAllocated())
		return;

	float x = offsetX;
	float y = offsetY;

	bgImage.draw(x, y);
}

void Room::DrawFront(int offsetX, int offsetY)
{
	if (!fgImage.isAllocated())
		return;

	float x = offsetX;
	float y = offsetY;

	fgImage.draw(x, y);
}

PlayerAction * Room::FindAction(Player * player)
{
	float pMinX = player->pos.x;
	float pMaxX = player->pos.x + ofApp::playerWidth;
	float pMinY = player->pos.y;
	float pMaxY = player->pos.y + ofApp::playerHeight;

	for (std::list<PlayerAction *>::iterator it = actions.begin(); it != actions.end(); it++)
	{
		PlayerAction * action = *it;
		float aMinX = action->pos.x;
		float aMaxX = action->pos.x + action->dim.x;
		float aMinY = action->pos.y;
		float aMaxY = action->pos.y + action->dim.y;

		bool overlapX = (pMinX < aMinX && aMinX < pMaxX) || (pMinX < aMaxX && aMaxX < pMaxX);
		bool overlapY = (pMinY < aMinY && aMinY < pMaxY) || (pMinY < aMaxY && aMaxY < pMaxY);

		if (overlapX && overlapY && action->requiredState == player->state)
		{
			return action;
		}
	}

	return NULL;
}