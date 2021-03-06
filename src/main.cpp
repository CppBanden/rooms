#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char ** argv)
{
	if (argc > 1 && strcmp(argv[1], "-f") == 0)
	{
		ofSetupOpenGL(ofApp::viewportWidth, ofApp::viewportHeight, OF_FULLSCREEN); // <-------- setup the GL context
	}
	else
	{
		ofSetupOpenGL(ofApp::viewportWidth, ofApp::viewportHeight, OF_WINDOW); // <-------- setup the GL context
	}
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
