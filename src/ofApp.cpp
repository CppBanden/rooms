#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	counter = 0;
	position = -60.0f;
	ofSetCircleResolution(50);
	ofBackground(0,0,0);
	ofSetWindowTitle("the first");

	// load graphics
	background.loadImage("images/60000bees.png");
	background.resize(128*10, 32*10);

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
	background.draw(1, 32*8);
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