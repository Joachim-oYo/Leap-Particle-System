#include "ofApp.h"

//--------------------------------------------------------------


void ofApp::setup(){
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofTrueTypeFont::setGlobalDpi(72);

    paragraph.setFont("lato_regular.ttf", 42);
    paragraph.setAlignment(ofxParagraph::ALIGN_CENTER);
    paragraph.setSpacing(20);
    paragraph.setWidth(1000);
    
    ofSetCircleResolution(50);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    int num = 3000;
    p.assign(num, demoParticle());
    currentMode = PARTICLE_MODE_REPEL;

    ofHideCursor();
    #ifdef __APPLE__
        CGDisplayHideCursor(NULL); // ofHideCursor() not working on OSX
    #endif
    resetParticles();
    
	leap.open();
    
    // keep app receiving data from leap motion even when it's in the background
    leap.setReceiveBackgroundFrames(true);
    
	cam.setOrientation(ofPoint(-20, 0, 0));
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
    attractPoints.clear();
    for(int i = 0; i < 4; i++){
        attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, 1920-100) , ofRandom(100, 1080-100) ) );
    }
    
    attractPointsWithMovement = attractPoints;
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
        p[i].setAttractPoints(&attractPointsWithMovement);;
        p[i].reset();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
	fingersFound.clear();
	
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, 0, ofGetWidth());
        leap.setMappingY(90, 490, ofGetHeight(), 0);
        leap.setMappingZ(-150, 150, 0, ofGetHeight());
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingerTips[f].set(tip);
                distFromPalm[f] = (abs(tip.distance(simpleHands[i].handPos)));
                fingersFound.push_back(id);
            }
        }
    }
    
	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();
    
    bool isFist;
    
    ofPoint cursorPos;

    if (simpleHands.size() != 0) {

        float average = 0;
        for (int i = 0 ; i < 5; i++) {
            average += distFromPalm[i];
        }
        average = average/5;
        isFist = (average < fistThreshold);
        cursorPos = ofPoint(simpleHands[0].handPos.x, simpleHands[0].handPos.y);
        fistStrengthStr = "The fist strength threshold is: " + ofToString(fistThreshold) +
        "\nThe current fist's strength is: " + ofToString(average);

        if (isFist) {
            if (currentMode != PARTICLE_MODE_ATTRACT) {
                ofResetElapsedTimeCounter();
                lastRecordedTime = ofGetElapsedTimef();
            }
            lastRecordedTime = ofGetElapsedTimef();
            currentMode = PARTICLE_MODE_ATTRACT;
        }
        else {
            if (currentMode != PARTICLE_MODE_REPEL) {
                ofResetElapsedTimeCounter();
                lastRecordedTime = ofGetElapsedTimef();
            }
            currentMode = PARTICLE_MODE_REPEL;
        }
    }
    else {
        fistStrengthStr = "There is currently no hand present.";
        if (currentMode != PARTICLE_MODE_OFF) {
            ofResetElapsedTimeCounter();
            lastRecordedTime = ofGetElapsedTimef();
        }
//        currentMode = PARTICLE_MODE_OFF;
        isFist = false;
        cursorPos = ofPoint(mouseX, mouseY);
    }
    
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
        p[i].cursor = cursorPos;
        p[i].update();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
	
//    ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
    
    
    ofPushStyle();
    ofNoFill();
    
    if (simpleHands.size() != 0) {
        ofSetColor(255);
        ofDrawCircle(simpleHands[0].handPos.x, simpleHands[0].handPos.y, 15);
        ofFill();
        ofDrawCircle(simpleHands[0].handPos.x, simpleHands[0].handPos.y, 7);
        
        if (currentMode == PARTICLE_MODE_REPEL) {
            float timeSince = ofGetElapsedTimef() - lastRecordedTime;
            ofPushStyle();
            ofSetColor(255);
            if (!bHide)
                ofDrawBitmapString(ofToString(timeSince), 300, 300);
            ofPopStyle();
            if (timeSince > 22) {
                ofResetElapsedTimeCounter();
                lastRecordedTime = ofGetElapsedTimef();
            }
            else if (timeSince > 6) {
                float alpha;
                if (timeSince < 15)
                    alpha = ofClamp(ofMap(timeSince, 6, 9, 0, 255), 0, 255);
                else
                    alpha = ofClamp(ofMap(timeSince, 15, 20, 255, 0), 0, 255);
                
                paragraph.setColor(ofColor(255, alpha));
                ofPushStyle();
                paragraph.setAlignment(ofxParagraph::ALIGN_CENTER);
                paragraph.setText("Try making a fist to collect");
                paragraph.draw(ofGetWidth()/2 - paragraph.getWidth()/2, 150);
                ofPopStyle();
            }
        }
    }
    else {
        float timeSince = ofGetElapsedTimef() - lastRecordedTime;
        ofPushStyle();
        ofSetColor(255);
        if (!bHide)
            ofDrawBitmapString(ofToString(timeSince), 300, 300);
        ofPopStyle();
        if (timeSince > 22) {
            ofResetElapsedTimeCounter();
            lastRecordedTime = ofGetElapsedTimef();
        }
        else if (timeSince > 5) {
            float alpha;
            if (timeSince < 17)
                alpha = ofClamp(ofMap(timeSince, 5, 8, 0, 255), 0, 255);
            else
                alpha = ofClamp(ofMap(timeSince, 17, 20, 255, 0), 0, 255);
            
            paragraph.setColor(ofColor(255, alpha));
            ofPushStyle();
            paragraph.setAlignment(ofxParagraph::ALIGN_CENTER);
            paragraph.setText("Place your hand above the plinth");
            paragraph.draw(ofGetWidth()/2 - paragraph.getWidth()/2, 150);
            ofPopStyle();
        }
        
        ofSetColor(255);
        ofDrawCircle(mouseX, mouseY, 15);
        ofFill();
        ofDrawCircle(mouseX, mouseY, 7);
    }
    ofPopStyle();
    
    if (!bHide) {
        ofPushStyle();
        ofSetColor(255);
        ofDrawBitmapString(fistStrengthStr, ofGetWidth()/2, ofGetHeight()/2);
        ofDrawBitmapString(ofToString(currentMode), ofGetWidth()/2, ofGetHeight()/3);
        ofPopStyle();
    }
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == '1'){
        currentMode = PARTICLE_MODE_ATTRACT;
    }
    else if( key == '2'){
        currentMode = PARTICLE_MODE_REPEL;
    }
    if( key == ' ' ){
        resetParticles();
    }
    else if (key == 'g' ){
        bHide = !bHide;
    }
    else if (key == '=') {
        fistThreshold++;
    }
    else if (key == '-') {
        fistThreshold--;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
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

//--------------------------------------------------------------
void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
