#include "entities.h"
#include "ofApp.h"

void Player::Init()
{
	// load sprites
	char filename[256];
	for (int i = 0; i != spriteWalkCount; i++)
	{
		sprintf(filename, "sprites/person_walk%d.png", i >> 1);
		spriteWalk[i].loadImage(filename);
		spriteWalk[i].getPixelsRef().resize(ofApp::playerWidth, ofApp::playerHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
		spriteWalk[i].update();
	}

	spriteFaceBack.loadImage("sprites/person_back.png");
	spriteFaceBack.getPixelsRef().resize(ofApp::playerWidth, ofApp::playerHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
	spriteFaceBack.update();

	spriteFaceFront.loadImage("sprites/person_front.png");
	spriteFaceFront.getPixelsRef().resize(ofApp::playerWidth, ofApp::playerHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
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
				action = room->FindAction(this);
				if (action != nullptr)
				{
					action->Start(this);
					state = PlayerState_PerformAction;
				}
				else
				{
					if (pressedL || pressedR)
					{
						SetFacing(pressedL ? -1.0f : 1.0f);
						vel.x = facing * (ofApp::scalingFactor * 1.0f);
					}
					else
					{
						vel.x = 0.0f;
					}

					pos.x += vel.x;

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
			}
		} break;

		case PlayerState_FindAction:
		{
			if (pressedA)
			{
				action = room->FindAction(this);
				if (action != nullptr)
				{
					action->Start(this);
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
				state = PlayerState_Walk;
			}
		} break;
	}
}

void Player::Draw()
{
	float x = room->pos.x + pos.x;
	float y = room->pos.y + pos.y;

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

void Room::DrawBack()
{
	if (!bgImage.isAllocated())
		return;

	ofSetColor(255 * opacity);
	bgImage.draw(pos.x, pos.y);
	ofSetColor(255);
}

void Room::DrawActions()
{
	ofSetColor(0, 255, 0, 127);
	for (std::list<PlayerAction *>::iterator it = actions.begin(); it != actions.end(); it++)
	{
		PlayerAction * action = *it;
		float x = pos.x + action->pos.x;
		float y = pos.y + action->pos.y;
		float w = action->dim.x;
		float h = action->dim.y;
		ofDrawBox(x + w / 2, y + h / 2, 0.0f, w, h, 0.0f);
	}
	ofSetColor(255, 255, 255, 255);
}

void Room::DrawFront()
{
	if (!fgImage.isAllocated())
		return;

	ofSetColor(255 * opacity);
	fgImage.draw(pos.x, pos.y);
	ofSetColor(255);
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

		bool outsideX = (pMaxX < aMinX) || (pMinX > aMaxX);
		bool outsideY = (pMaxY < aMinY) || (pMinY > aMaxY);

		std::cout << "outsideX " << outsideX << " outsideY " << outsideY << std::endl;

		if (!outsideX && !outsideY && action->requiredState == player->state)
		{
			return action;
		}
	}

	return NULL;
}