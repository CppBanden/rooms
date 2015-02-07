#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	counter = 0;
	position = -60.0f;
	ofSetCircleResolution(50);
	ofBackground(108, 45, 88);
	ofSetWindowTitle("the first");

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
	//--------------------------- circles
	//let's draw a circle:
	ofSetColor(37, 126, 120);
	ofFill();		// draw "filled shapes"
	ofCircle(position,400,60);

	// prik
	positionPrikX = position - 60*sin(counter);
	positionPrikY = 400;
	ofSetColor(64, 184, 175);
	ofFill();		// draw "prik"
	ofCircle(positionPrikX,positionPrikY,4);

	// use the bitMap type
	// note, this can be slow on some graphics cards
	// because it is using glDrawPixels which varies in
	// speed from system to system.  try using ofTrueTypeFont
	// if this bitMap type slows you down.
	ofSetHexColor(0x000000);
	ofDrawBitmapString("well, well, well, how the turntables", 75,500);
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