#include "ofMain.h"
#include "ofApp.h"

int main(){
    ofSetupOpenGL(RATIO*WINDOW_WIDTH,RATIO*WINDOW_HEIGHT,OF_FULLSCREEN);
    ofRunApp(new ofApp());
}