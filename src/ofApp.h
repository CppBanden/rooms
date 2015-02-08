#pragma once

#include "ofMain.h"
#include "entities.h"

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		static const int scalingFactor = 7;
		static const int viewportWidth = 1280;
		static const int viewportHeight = 720;
		static const int backgroundWidth = scalingFactor*128;
		static const int backgroundHeight = scalingFactor*32;
		static const int marginTop = (viewportHeight - backgroundHeight) / 2;
		static const int marginLeft = (viewportWidth - backgroundWidth) / 2;
		static const int playerWidth = scalingFactor * 8;
		static const int playerHeight = scalingFactor * 8;

		float 	counter;
		float	position;
		float	positionPrikX;
		float	positionPrikY;

		Player *			player;
		std::list<Room *>	rooms;

		Room * grave;
		Room * black;

		float musicVolume;
		float musicVolumeStep;

		ofSoundPlayer music;
};
