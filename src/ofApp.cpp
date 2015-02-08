#include "ofApp.h"
#include "entities.h"
#include <cmath>

struct Door : PlayerAction
{
	enum Direction
	{
		Left,
		Right,
		Up,
		Down,
		DownStart,
	};

	Direction dir;
	Room * sourceRoom;
	Room * targetRoom;

	int delay;
	int delayRemaining;

	float stepCount;
	float stepX;
	float stepY;
	float stepOpacity;

	Door() : delay(0) {}

	void Start(Player * player)
	{
		delayRemaining = delay;

		sourceRoom = player->room;

		float margin = ofApp::scalingFactor * 10.0f;
		float offsetX = ofApp::backgroundWidth + margin;
		float offsetY = ofApp::backgroundHeight + margin;

		switch (dir)
		{
			case Left:
			{
				offsetX *= -1.0f;
				offsetY = 0.0f;
			} break;
			case Right:
			{
				offsetX *= 1.0f;
				offsetY = 0.0f;
			} break;
			case Up:
			{
				offsetX = 0.0f;
				offsetY *= -1.0f;
			} break;
			case Down:
			{
				offsetX = 0.0f;
				offsetY *= 1.0f;
			} break;
			case DownStart:
			{
				offsetX = 0.0f;
				offsetY *= 1.0f;
			} break;
		}

		targetRoom->pos = ofVec2f(ofApp::marginLeft + offsetX, ofApp::marginTop + offsetY);
		targetRoom->opacity = 0.0f;
		stepCount = 50.0f;
		stepX = -offsetX / stepCount;
		stepY = -offsetY / stepCount;
		stepOpacity = 1.0f / stepCount;
	}

	PlayerAction * Update(Player * player)
	{
		if (--delayRemaining > 0)
			return this;

		sourceRoom->opacity = std::max(0.0f, sourceRoom->opacity - stepOpacity);
		targetRoom->opacity = std::min(1.0f, targetRoom->opacity + stepOpacity);

		sourceRoom->pos += ofVec2f(stepX, stepY);
		targetRoom->pos += ofVec2f(stepX, stepY);

		if (sourceRoom->opacity > 0.75 || targetRoom->opacity > 0.75f)
		{
			if (stepX > 0.0f)
				player->pos.x -= ofApp::scalingFactor;
			else if (stepX < 0.0f)
				player->pos.x += ofApp::scalingFactor;
			if (stepY > 0.0f)
				player->pos.y -= ofApp::scalingFactor;
			else if (stepY < 0.0f)
				player->pos.y += ofApp::scalingFactor;
		}

		if (targetRoom->opacity > 0.5f)
		{
			if (player->room != targetRoom)
			{
				player->room = targetRoom;
				switch (dir)
				{
					case Left: {
						player->pos.x = ofApp::backgroundWidth;
					} break;
					case Right: {
						player->pos.x = -ofApp::playerWidth;
					} break;
					case Up: {
						player->pos.y = ofApp::backgroundHeight;
					} break;
					case Down: {
						player->pos.y = -ofApp::playerHeight;
					} break;
					case DownStart: {
						player->pos.y = -ofApp::playerHeight;
						player->pos.x = ofApp::playerWidth;
					} break;
				}
			}
			
			switch (dir)
			{
				case Left: {
					player->pos.x = std::fmaxf(player->pos.x, ofApp::backgroundWidth - ofApp::playerWidth - ofApp::scalingFactor);
				} break;
				case Right: {
					player->pos.x = std::fminf(player->pos.x, ofApp::scalingFactor);
				} break;
				case Up: {
					player->pos.y = std::fmaxf(player->pos.y, ofApp::backgroundHeight - ofApp::playerHeight - ofApp::scalingFactor);
				} break;
				case Down: {
					player->pos.y = std::fminf(player->pos.y, ofApp::scalingFactor);
				} break;
				case DownStart: {
					player->pos.y = std::fminf(player->pos.y, ofApp::scalingFactor);
				} break;
			}
		}

		if (targetRoom->opacity == 1.0f)
		{
			targetRoom->pos = ofVec2f(ofApp::marginLeft, ofApp::marginTop);
			return NULL;
		}
		else
		{
			return this;
		}
	}
};

//--------------------------------------------------------------
void ofApp::setup()
{
	musicVolume = 0.0f;
	musicVolumeStep = 0.01f;

	music.setVolume(musicVolume);
	music.loadSound("sounds/Lasses_Tippi_music.wav", false);
	music.setLoop(true);
	music.play();

	ofBackground(0, 0, 0);
	ofSetWindowTitle("rooms");
	ofSetFrameRate(10);//10); // if vertical sync is off, we can go a bit fast... this caps the framerate at 15fps.

	// create room 0
	Room * room0 = new Room();
	room0->Init("sprites/room0_bg.png", NULL, NULL);
	room0->dark = true;

	Door * door0e = new Door();
	door0e->requiredState = PlayerState_Walk;
	door0e->dim.x = 20.0f * scalingFactor;
	door0e->dim.y = backgroundHeight;	
	door0e->pos.x = backgroundWidth;
	door0e->pos.y = 0.0f;
	door0e->dir = Door::Right;
	room0->actions.push_back(door0e);

	// create room 1
	Room * room1 = new Room();
	room1->Init("sprites/room1_bg.png", NULL, "sprites/text1.png");

	Door * door1e = new Door();
	door1e->requiredState = PlayerState_Walk;
	door1e->dim.x = 20.0f * scalingFactor;
	door1e->dim.y = backgroundHeight;	
	door1e->pos.x = backgroundWidth;
	door1e->pos.y = 0.0f;
	door1e->dir = Door::Right;
	room1->actions.push_back(door1e);

	Door * door1w = new Door();
	door1w->requiredState = PlayerState_Walk;
	door1w->dim.x = 20.0f * scalingFactor;
	door1w->dim.y = backgroundHeight;	
	door1w->pos.x = 0.0f - door1w->dim.x;
	door1w->pos.y = 0.0f;
	door1w->dir = Door::Left;
	room1->actions.push_back(door1w);

	// create room 2
	Room * room2 = new Room();
	room2->Init("sprites/room2_bg2.png", NULL, "sprites/text2.png");

	Door * door2e = new Door();
	door2e->requiredState = PlayerState_Walk;
	door2e->dim.x = 20.0f * scalingFactor;
	door2e->dim.y = backgroundHeight;
	door2e->pos.x = backgroundWidth;
	door2e->pos.y = 0.0f;
	door2e->dir = Door::Right;
	room2->actions.push_back(door2e);

	Door * door2w = new Door();
	door2w->requiredState = PlayerState_Walk;
	door2w->dim.x = 20.0f * scalingFactor;
	door2w->dim.y = backgroundHeight;
	door2w->pos.x = 0.0f - door2w->dim.x;
	door2w->pos.y = 0.0f;
	door2w->dir = Door::Left;
	room2->actions.push_back(door2w);

	Door * door2d = new Door();
	door2d->requiredState = PlayerState_FindAction;
	door2d->dim.x = playerWidth;
	door2d->dim.y = backgroundHeight;
	door2d->pos.x = backgroundWidth / 2.0f - scalingFactor * 35.0f;
	door2d->pos.y = 0.0f;
	door2d->dir = Door::Down;
	room2->actions.push_back(door2d);

	// create room 3
	Room * room3 = new Room();
	room3->Init("sprites/room5b_bg.png", "sprites/room5b_fg.png", "sprites/text3.png");

	Door * door3e = new Door();
	door3e->requiredState = PlayerState_Walk;
	door3e->dim.x = 20.0f * scalingFactor;
	door3e->dim.y = backgroundHeight;	
	door3e->pos.x = backgroundWidth;
	door3e->pos.y = 0.0f;
	door3e->dir = Door::Right;
	room3->actions.push_back(door3e);

	Door * door3w = new Door();
	door3w->requiredState = PlayerState_Walk;
	door3w->dim.x = 20.0f * scalingFactor;
	door3w->dim.y = backgroundHeight;	
	door3w->pos.x = 0.0f - door1w->dim.x;
	door3w->pos.y = 0.0f;
	door3w->dir = Door::Left;
	room3->actions.push_back(door3w);

	// create room 4
	Room * room4 = new Room();
	room4->Init("sprites/room3_bg.png", "sprites/room3_fg.png", "sprites/text4.png");

	Door * door4d = new Door();
	door4d->requiredState = PlayerState_FindAction;
	door4d->delay = 120;
	door4d->dim.x = playerWidth;
	door4d->dim.y = backgroundHeight;
	door4d->pos.x = backgroundWidth - 2 * playerWidth;
	door4d->pos.y = 0.0f;
	door4d->dir = Door::DownStart;
	room4->actions.push_back(door4d);

	Door * door4w = new Door();
	door4w->requiredState = PlayerState_Walk;
	door4w->dim.x = 20.0f * scalingFactor;
	door4w->dim.y = backgroundHeight;	
	door4w->pos.x = 0.0f - door1w->dim.x;
	door4w->pos.y = 0.0f;
	door4w->dir = Door::Left;
	room4->actions.push_back(door4w);

	// link doors
	door0e->targetRoom = room1;

	door1w->targetRoom = room0;
	door1e->targetRoom = room2;

	door2w->targetRoom = room1;
	door2e->targetRoom = room3;
	door2d->targetRoom = room4;

	door3w->targetRoom = room2;
	door3e->targetRoom = room4;

	door4w->targetRoom = room3;
	door4d->targetRoom = room0;

	// init rooms
	rooms.push_back(room0);
	rooms.push_back(room1);
	rooms.push_back(room2);
	rooms.push_back(room3);
	rooms.push_back(room4);

	grave = room4;
	black = room0;

	// init player
	player = new Player();
	player->Init();
	player->pos.x = -playerWidth;// start off-screen
	player->pos.y = backgroundHeight - playerHeight - 3 * scalingFactor;
	player->room = room0;
	player->room->pos = ofVec2f(marginLeft, marginTop);
	player->room->opacity = 1.0f;
}

//--------------------------------------------------------------
void ofApp::update()
{
	musicVolume += musicVolumeStep;
	if (musicVolume > 1.0f)
		musicVolume = 1.0f;
	if (musicVolume < 0.0f)
		musicVolume = 0.0f;

	music.setVolume(musicVolume * 1.0f);

	Room * oldRoom = player->room;

	oldRoom->Update();
	player->Update();

	if (oldRoom == grave && player->room == black && musicVolumeStep > 0.0f)
	{
		musicVolumeStep *= -2.0f;
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	for (std::list<Room *>::iterator it = rooms.begin(); it != rooms.end(); it++)
	{
		Room * room = *it;
		room->DrawBack();
		
		if (room == player->room)
		{
			//room->DrawActions(); // debug
			player->Draw();
		}

		room->DrawFront();
		room->DrawText();
	}

	//float tx = 0.0f;
	//float ty = 0.0f;
	//ofSetHexColor(0xFFFFFFFF);
	//ofDrawBitmapString("hello!", marginLeft, marginTop + backgroundHeight);
}



// ---- UNSUED STUFF ----

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}