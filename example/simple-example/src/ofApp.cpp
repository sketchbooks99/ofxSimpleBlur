#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0);

	bloom.allocate(ofGetWidth(), ofGetHeight());
	bloom.setStrength(1.0);
	bloom.setBlurSize(3.0);
	bloom.setThreshold(0.5);
	bloom.setupShaders();


	box.set(300);

	gui.setup();
	gui.setPosition(10, 10);
	gui.add(blurSize.set("blurSize", 5.0, 0.1, 20.0));
	gui.add(strength.set("strength", 1.0, 0.1, 10.0));
	gui.add(threshold.set("threshold", 0.5, 0.0, 1.0));

	glLineWidth(10.0f);
}

//--------------------------------------------------------------
void ofApp::update() {
	// set parameters 
	bloom.setBlurSize(blurSize);
	bloom.setStrength(strength);
	bloom.setThreshold(threshold);

	// bloom
	bloom.begin();
	ofEnableDepthTest();
	cam.begin();

	ofPushMatrix();
	ofRotateRad(ofGetElapsedTimef(), 1.0, 0.3, 0.7);
	box.drawWireframe();
	ofPopMatrix();

	cam.end();
	ofDisableDepthTest();

	bloom.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// debug horizontal/vertical blur and bloom result
	bloom.draw(0, 0, ofGetWidth() / 2, ofGetHeight() / 2);
	bloom.debugBlur(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight() / 2);
	bloom.debugBrightness(0, ofGetHeight() / 2, ofGetWidth() / 2, ofGetHeight() / 2);

	ofDrawBitmapString("RESULT", ofGetWidth() / 4, 20);
	ofDrawBitmapString("BLUR", ofGetWidth() * 3 / 4, 20);
	ofDrawBitmapString("THRESHOLD", ofGetWidth() / 4, ofGetHeight()/2 + 20);

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
