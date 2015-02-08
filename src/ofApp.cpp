#include "ofApp.h"
#include "entities.h"

struct Door : PlayerAction
{
	enum Direction
	{
		Left,
		Right,
		Up,
		Down,
	};

	Direction dir;
	Room * sourceRoom;
	Room * targetRoom;

	float stepCount;
	float stepX;
	float stepY;
	float stepOpacity;

	void Start(Player * player)
	{
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
		sourceRoom->opacity = std::max(0.0f, sourceRoom->opacity - stepOpacity);
		targetRoom->opacity = std::min(1.0f, targetRoom->opacity + stepOpacity);

		sourceRoom->pos += ofVec2f(stepX, stepY);
		targetRoom->pos += ofVec2f(stepX, stepY);

		if (stepX > 0.0f)
			player->pos.x -= ofApp::scalingFactor;
		else if (stepX < 0.0f)
			player->pos.x += ofApp::scalingFactor;
		if (stepY > 0.0f)
			player->pos.y -= ofApp::scalingFactor;
		else if (stepY < 0.0f)
			player->pos.y += ofApp::scalingFactor;

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
	ofBackground(0, 0, 0);
	ofSetWindowTitle("rooms");
	ofSetFrameRate(15); // if vertical sync is off, we can go a bit fast... this caps the framerate at 15fps.

	// create room 1
	Room * room1 = new Room();
	room1->Init("sprites/room1_bg.png", NULL);

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
	room2->Init("sprites/room2_bg.png", "sprites/room2_fg.png");

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

	// create room 2
	Room * room3 = new Room();
	room3->Init("sprites/room3_bg.png", NULL);

	// link doors
	door1e->targetRoom = room2;
	door1w->targetRoom = room2;

	door2e->targetRoom = room1;
	door2w->targetRoom = room1;
	door2d->targetRoom = room3;

	// init rooms
	rooms.push_back(room1);
	rooms.push_back(room2);
	rooms.push_back(room3);

	// init player
	player = new Player();
	player->Init();
	player->pos.x = scalingFactor * 3;
	player->pos.y = backgroundHeight - playerHeight - 3 * scalingFactor;
	player->room = room1;
	player->room->pos = ofVec2f(marginLeft, marginTop);
	player->room->opacity = 1.0f;
}

//--------------------------------------------------------------
void ofApp::update()
{
	player->Update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	const float opacityStep = 1.0f / 20.0f;

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
	}
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