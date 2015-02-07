#include "ofApp.h"
#include "entities.h"

struct Door : PlayerAction
{
	Room * sourceRoom;
	Room * targetRoom;

	void Start(Player * player)
	{
		sourceRoom = player->room;
		player->room = targetRoom;
		player->pos.x = 0.0f;
	}

	PlayerAction * Update(Player * player)
	{
		//oldRoom->pos.x -= ofApp::scalingFactor;
		return NULL;
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
	room1->pos = ofVec2f(marginLeft, marginTop);
	room1->opacity = 0.0f;

	Door * door1 = new Door();
	door1->requiredState = PlayerState_Walk;
	door1->dim.x = 20.0f * scalingFactor;
	door1->dim.y = backgroundHeight;	
	door1->pos.x = backgroundWidth;
	door1->pos.y = 0.0f;
	room1->actions.push_back(door1);

	// create room 2
	Room * room2 = new Room();
	room2->Init("sprites/room2_bg.png", "sprites/room2_fg.png");
	room2->pos = ofVec2f(marginLeft, marginTop);
	room2->pos.y += backgroundHeight + 2 * scalingFactor;
	room2->opacity = 0.0f;

	// link doors
	door1->targetRoom = room2;

	//Door * door2 = new Door();
	//door2->requiredState = PlayerState_Walk;
	//door2->dim.x = 20.0f * scalingFactor;
	//door2->dim.y = backgroundHeight;
	//door2->pos.x = backgroundWidth;
	//door2->pos.y = 0.0f;
	//room2->actions.push_back(door2);

	// init rooms
	rooms.push_back(room1);
	rooms.push_back(room2);

	// init player
	player = new Player();
	player->Init();
	player->pos.x = backgroundWidth / 2;
	player->pos.y = backgroundHeight - playerHeight - 3 * scalingFactor;
	player->room = room1;
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

		if (room == player->room)
		{
			room->opacity = std::min(1.0f, room->opacity + opacityStep);
		}
		else
		{
			room->opacity = std::max(0.0f, room->opacity - opacityStep);
		}

		room->DrawBack();
		room->DrawActions();

		if (room == player->room)
		{
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