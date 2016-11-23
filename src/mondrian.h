#pragma once

#include "ofMain.h"
#include "ofxTrueTypeFontUL2.h"
#include "ofxProcessFFT.h"

#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900
#define MARGIN 120
#define PADDING 20
#define RATIO 1

class mondrian{
    public:
        mondrian();
    
        void setup();
        void reset();
        void update();
        void draw();
    
        void ryojiikeda();
        void composition();
        void boogieWoogie();
    
        void setColorMode(int c);
        void changeColor();
    
        void typed(int key);
        void pressed(int key);
        void released(int key);
    
        class mondrian2f{
        public:
            int x;
            int y;
            bool drawX;
            bool drawY;
            bool section;
            
            ofColor lineX;
            ofColor lineY;
            ofColor plane;
            ofColor point;
        
            mondrian2f(int _x,int _y,bool _drawX,bool _drawY){
                x=_x;
                y=_y;
                drawX=_drawX;
                drawY=_drawY;
                section=false;
                
                lineX=ofColor(0);
                lineY=ofColor(0);
                plane=ofColor(255);
                point=ofColor(0);
            };
        };
    
        class car{
        public:
            float x;
            float y;
            int xNum;
            int yNum;
            int direction;
            
            int length;
            int speed;
            
            ofColor body;
            
            car();
            car(int _x,int _y,int _xNum,int _yNum,int _direction){
                x=_x;
                y=_y;
                xNum=_xNum;
                yNum=_yNum;
                direction=_direction;
            };
        };
    
        class architecture{
        public:
            int xNum1;
            int yNum1;
            int xNum2;
            int yNum2;
            
            ofColor exterior;
            ofColor interior;
            
            architecture();
            architecture(int _xNum1,int _yNum1,int _xNum2,int _yNum2){
                xNum1=_xNum1;
                yNum1=_yNum1;
                xNum2=_xNum2;
                yNum2=_yNum2;
            };
        };
    
    enum {SYNC,BLAST};
    enum {COMPOSITION,BOOGIE_WOOGIE,IKEDA};
    enum {RYB,CMY,K};
    enum {LINE,PLANE,POINT,CAR};
    enum {UP,DOWN,LEFT,RIGHT};
    
    ProcessFFT fft;
    
    vector<int> x;
    vector<int> y;
    vector< vector<mondrian2f> > xy;
    vector<car> c;
    vector<architecture> a;
    
    bool drawModeChanged;
    int drawMode;
    int mondrianMode;
    int colorMode;
    
    bool stop;
    bool moving;
    
    ofColor red;
    ofColor yellow;
    ofColor blue;
    ofColor white;
    ofColor black;
    ofColor gray;
    ofColor color(int f);
    
    int _strokeWeight;
    int strokeWeight;
    int spacingMinRatio;
    int spacingMaxRatio;
    int hideRatio;
    int splitRatio;
    int colorRatio;
            
    ofxTrueTypeFontUL2 command;
    vector<char> _command0;
    vector<char> _command1;
    vector<char> _command2;
    string command0;
    string command1;
    string command2;
    
    ofxTrueTypeFontUL2 parameter;
    ofxTrueTypeFontUL2 caption;
    vector<string> text;
    
    bool typing;
    bool operating;
    float type;
    float operate;
    
    bool W;
    bool I;
    bool A;
    bool H;
    bool S;
    bool C;
    bool D;
    bool M;
    
    int _x;
    int _y;
    float _r;
    
    int w;
    int h;
};