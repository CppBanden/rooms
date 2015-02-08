#include "entities.h"
#include "ofApp.h"
#include <cmath>

void DrawPixelPerfect(ofImage * image, float x, float y, float w, float h)
{
	float s = ofApp::scalingFactor;
	image->drawSubsection(x, y, w, h, 0.0f, 0.0f, w / s, h / s);
}

void Player::Init()
{
	// load sprites
	spriteWalkIndex = 0;
	char filename[256];
	for (int i = 0; i != spriteWalkCount; i++)
	{
		sprintf(filename, "sprites/person_walk%d.png", i >> 1);
		spriteWalk[i].loadImage(filename);
		spriteWalk[i].getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		spriteWalk[i].update();

		sprintf(filename, "sprites/persongrey_walk%d.png", i >> 1);
		spriteWalkDark[i].loadImage(filename);
		spriteWalkDark[i].getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		spriteWalkDark[i].update();
	}

	spriteFaceBack.loadImage("sprites/person_back.png");
	spriteFaceBack.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	spriteFaceBack.update();

	spriteFaceBackDark.loadImage("sprites/persongrey_back.png");
	spriteFaceBackDark.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	spriteFaceBackDark.update();

	spriteFaceFront.loadImage("sprites/person_front.png");
	spriteFaceFront.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	spriteFaceFront.update();

	spriteFaceFrontDark.loadImage("sprites/persongrey_front.png");
	spriteFaceFrontDark.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	spriteFaceFrontDark.update();

	// load footsteps
	footstepCount = 8;
	for (int i = 0; i != footstepCount; i++)
	{
		char filename[256];
		sprintf(filename, "sounds/Lasses_Tippi_footsteps_%d.wav", i + 1);
		footsteps[i].loadSound(filename, false);
		footsteps[i].setLoop(false);
		footsteps[i].setVolume(0.1f);
	}

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
		spriteWalkDark[i].mirror(false, true);
	}

	spriteFaceBack.mirror(false, true);
	spriteFaceBackDark.mirror(false, true);
	spriteFaceFront.mirror(false, true);
	spriteFaceFrontDark.mirror(false, true);
}

void Player::Footstep()
{
	footstepIndex = (int)ofRandom(7.0f);
	//footstepIndex++;
	//footstepIndex %= footstepCount;
	footsteps[footstepIndex].play();
	footsteps[footstepIndex].setVolume(0.1f);
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
			if (pos.y < ofApp::backgroundHeight - ofApp::playerHeight - 3 * ofApp::scalingFactor)
			{
				pos.y += ofApp::scalingFactor;
				pressedL = false;
				pressedR = false;
				pressedA = false;
			}

			if (pressedA)
			{
				state = PlayerState_FindAction;
			}
			else
			{
				action = room->FindAction(this);
				if (action != NULL)
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
						Footstep();
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
				if (action != NULL)
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
			if (action != NULL)
			{
				action = action->Update(this);
			}
			if (action == NULL)
			{
				state = PlayerState_Walk;
			}
		} break;
	}
}

void Player::Draw()
{
	ofImage * image = NULL;

	switch (state)
	{
		case PlayerState_Walk: {
			if (room->dark)
				image = &spriteWalkDark[spriteWalkIndex];
			else
				image = &spriteWalk[spriteWalkIndex];
		} break;
		case PlayerState_FindAction: {
			if (room->dark)
				image = &spriteFaceBackDark;
			else	
				image = &spriteFaceBack;
		} break;
		case PlayerState_PerformAction: {
			if (room->dark)
				image = &spriteFaceFrontDark;
			else
				image = &spriteFaceFront;
		} break;
	}

	if (image != NULL)
	{
		float minX = room->pos.x;
		float minY = room->pos.y;
		float maxX = room->pos.x + ofApp::backgroundWidth;
		float maxY = room->pos.y + ofApp::backgroundHeight;

		float x = room->pos.x + pos.x;
		float y = room->pos.y + pos.y;

		float w = ofApp::playerWidth;
		float h = ofApp::playerHeight;
		float s = ofApp::scalingFactor;

		float sx = 0.0f;
		float sy = 0.0f;
		float sw = w;
		float sh = h;

		if (x < minX)
		{
			float d = std::min(minX - x, w);
			x = minX; w -= d; sx = d; sw -= d;
		}
		else if (x > maxX - w)
		{
			float d = std::min(x - (maxX - w), w);
			w -= d; sw -= d;
		}

		if (y < minY)
		{
			float d = std::min(minY - y, h);
			y = minY; h -= d; sy = d; sh -= d;
		}
		else if (y > maxY - h)
		{
			float d = std::min(y - (maxY - h), h);
			h -= d; sh -= d;
		}

		ofSetColor(255 * room->opacity);
		image->drawSubsection(x, y, w, h, sx/s, sy/s, sw/s, sh/s);
		ofSetColor(255);
	}
}

void Room::Init(const char * bgFilename, const char * fgFilename)
{
	dark = false;
	opacity = 0.0f;

	if (bgFilename != NULL)
	{
		bgImage.loadImage(bgFilename);
		bgImage.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		bgImage.update();
	}

	if (fgFilename != NULL)
	{
		fgImage.loadImage(fgFilename);
		fgImage.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
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

	ofSetColor(255, 255, 255, 255 * opacity);
	DrawPixelPerfect(&bgImage, pos.x, pos.y, ofApp::backgroundWidth, ofApp::backgroundHeight);
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

	ofSetColor(255, 255, 255, 255 * opacity);
	DrawPixelPerfect(&fgImage, pos.x, pos.y, ofApp::backgroundWidth, ofApp::backgroundHeight);
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

		//std::cout << "outsideX " << outsideX << " outsideY " << outsideY << std::endl;

		if (!outsideX && !outsideY && action->requiredState == player->state)
		{
			return action;
		}
	}

	return NULL;
}