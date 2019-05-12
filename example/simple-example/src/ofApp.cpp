#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0);

	blur.allocate(ofGetWidth(), ofGetHeight());
	blur.setSize(5.0);
	blur.setStrength(1.0);
	blur.setupShader();

	box.set(300);

	gui.setup();
	gui.setPosition(10, 10);
	gui.add(blurSize.set("blurSize", 5.0, 0.1, 20.0));
	gui.add(strength.set("strength", 1.0, 0.1, 10.0));

	glLineWidth(10.0f);
}

//--------------------------------------------------------------
void ofApp::update() {
	// set parameters 
	blur.setSize(blurSize);
	blur.setStrength(strength);

	// blur
	blur.begin();
	ofEnableDepthTest();
	cam.begin();

	ofPushMatrix();
	ofRotateRad(ofGetElapsedTimef(), 1.0, 0.3, 0.7);
	box.drawWireframe();
	ofPopMatrix();

	cam.end();
	ofDisableDepthTest();

	blur.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// debug horizontal/vertical blur and bloom result
	switch (mode) {
	case 0:
		blur.draw(0, 0);
		break;
	case 1:
		blur.bloomDraw(0, 0);
		break;
	case 2:
		blur.debugFbo();
		break;
	}

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'c':
		mode = (mode + 1) % 3;
		break;
	case 'q':
		exit();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
