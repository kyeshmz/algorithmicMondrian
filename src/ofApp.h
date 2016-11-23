#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "ofxOsc.h"
#include "mondrian.h"

#define FRAME_RATE 50
#define HOST "127.0.0.1"
#define PORT 8000

class ofApp:public ofBaseApp{
    
    public:
        void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mousePressed(int x,int y,int button);
		void mouseReleased(int x,int y,int button);
        void mouseMoved(int x,int y);
        void mouseDragged(int x,int y,int button);
		void windowResized(int w,int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void screenShot(ofFbo f);
        void setBpm(float b,float t);
    
    ofFbo fbo;
    ofxPostGlitch glitch;
    
    ofImage image;
    ofImage mondrianize;
    bool screenShoted;
    bool jpGlitched;
    bool pnGlitched;
    bool pieted;
    bool white;
    
    ofxOscSender sender;
    int screenShotNum;
    int pnGlitchNum;
    int pietNum;
    
    float frameCount;
    float time;
    float bpm;
    float spb;
    float fps;
    vector<float> click;
    
    mondrian mondrian;
};
