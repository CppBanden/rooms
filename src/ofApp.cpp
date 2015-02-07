#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	counter = 0;
	position = -60.0f;
	ofSetCircleResolution(50);
	ofBackground(0,0,0);
	ofSetWindowTitle("the first");

	// load graphics
	background.loadImage("sprites/room1_bg.png");
	background.getPixelsRef().resize(1280,320,OF_INTERPOLATE_NEAREST_NEIGHBOR);  
	background.update();

	player.loadImage("sprites/person1_frm1.png");
	player.getPixelsRef().resize(80,80,OF_INTERPOLATE_NEAREST_NEIGHBOR);  
	player.update();

	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
}

//--------------------------------------------------------------
void ofApp::update(){
	counter = counter + 0.033f;
	position = position + 3.0f;
	if ( position >= 1080.0f)
		position = -60.0f;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);

	// draw image
	background.draw(1, 170);
	player.draw(1, 170);
	ofSetHexColor(0x000000);
}

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