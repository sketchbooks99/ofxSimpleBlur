#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings s;
	s.setGLVersion(3, 2);
	s.setSize(1024, 768);
	ofCreateWindow(s);

	ofRunApp(new ofApp());

}
