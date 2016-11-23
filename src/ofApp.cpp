#include "ofApp.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::setup(){
    ofHideCursor();
    //ofSetWindowPosition(ofGetWidth()+1,0);
    ofSetEscapeQuitsApp(false);
    
    fbo.allocate(RATIO*WINDOW_WIDTH,RATIO*WINDOW_HEIGHT,GL_RGBA,4);
    glitch.setup(&fbo);
    ofBackground(255);
    ofDisableAntiAliasing();
    
    ofSetFrameRate(FRAME_RATE);
    ofSetVerticalSync(true);
    
    sender.setup(HOST,PORT);
    screenShotNum=-1;
    pnGlitchNum=-1;
    
    frameCount=0;
    bpm=60.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::update(){
    if(screenShoted){
        image.grabScreen(0,0,RATIO*WINDOW_WIDTH,RATIO*WINDOW_HEIGHT);
        image.saveImage("screenshot.jpg");
        image.saveImage("screenshot.png");
        screenShoted=false;
    }
    
    ofxOscMessage m;
    m.setAddress("/test");
    m.addIntArg(screenShotNum);
    m.addIntArg(pnGlitchNum);
    sender.sendMessage(m);
    
    if(0<click.size()&&ofGetElapsedTimeMillis()-click[click.size()-1]>2000.0){
        click.clear();
    }
    
    if(0<mondrian.type&&ofGetElapsedTimeMillis()-mondrian.type>2000.0){
        mondrian.typing=false;
        mondrian.command0="";
        mondrian.command1="";
        mondrian.command2="";
        mondrian._command0.clear();
        mondrian._command1.clear();
        mondrian._command2.clear();
    }
    
    if(0<mondrian.operate&&ofGetElapsedTimeMillis()-mondrian.operate>2000.0){
        if(!mondrian.W&&!mondrian.I&&!mondrian.A&&
           !mondrian.H&&!mondrian.S&&!mondrian.C&&
           !mondrian.D&&!mondrian.M){
            mondrian.operating=false;
        }
    }
    
    if(mondrian.drawModeChanged){
        if(mondrian.drawMode==mondrian.SYNC)time=16;
        else time=1;
        setBpm(bpm,time);
        mondrian.drawModeChanged=false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::draw(){
    fbo.begin();
        ofClear(255,255);
        if(pnGlitched||jpGlitched||pieted){
            if(mondrian.mondrianMode==mondrian.COMPOSITION){
                frameCount+=1.0;
                if(FRAME_RATE/fps<frameCount){
                    frameCount-=FRAME_RATE/fps;
                }
            }
            if(pnGlitched)image.loadImage("pnglitch.png");
            if(jpGlitched)image.loadImage("jpglitch.jpg");
            if(pieted){
                if(pietNum==1)image.loadImage("piet01.jpg");
                else if(pietNum==2)image.loadImage("piet02.jpg");
                else image.loadImage("piet03.jpg");
            }
            image.draw(0,0);
        }else{
            if(!mondrian.stop){
                if(mondrian.mondrianMode==mondrian.COMPOSITION){
                    if(mondrian.drawMode==mondrian.SYNC){
                        frameCount+=1.0;
                        if(FRAME_RATE/fps<frameCount){
                            frameCount-=FRAME_RATE/fps;
                            mondrian.update();
                        }
                    }else{
                        frameCount+=1.0;
                        if(frameCount<FRAME_RATE/fps/2.0){
                            mondrian.update();
                        }else if(FRAME_RATE/fps<frameCount){
                            frameCount-=FRAME_RATE/fps;
                        }
                    }
                }else if(mondrian.mondrianMode==mondrian.BOOGIE_WOOGIE){
                    if(mondrian.drawMode==mondrian.SYNC){
                        frameCount+=1.0;
                        if(FRAME_RATE/fps<frameCount){
                            if(!mondrian.moving)mondrian.moving=true;
                            frameCount-=FRAME_RATE/fps;
                            for(int i=0;i<ceil(sqrt(sqrt(sqrt(mondrian.c.size()))));i++){
                                mondrian.changeColor();
                            }
                            mondrian.update();
                        }
                    }else{
                        frameCount+=1.0;
                        if(frameCount<FRAME_RATE/fps/2.0){
                            if(!mondrian.moving)mondrian.moving=true;
                            for(int i=0;i<ceil(sqrt(sqrt(sqrt(mondrian.c.size()))));i++){
                                mondrian.changeColor();
                                mondrian.update();
                            }
                        }else if(frameCount<FRAME_RATE/fps){
                            if(mondrian.moving)mondrian.moving=false;
                        }else{
                            frameCount-=FRAME_RATE/fps;
                        }
                    }
                }
            }
            mondrian.draw();
        }
    fbo.end();
    glitch.generateFx();
    
    fbo.draw(0,0);
    //ofSetColor(0);
    //string msg=ofToString(ofGetFrameRate())+string(" fps");
    //ofDrawBitmapString(msg,10,20);
    
    ofSetColor(0);
    ofRect(RATIO*MARGIN,RATIO*PADDING,WINDOW_WIDTH-RATIO*MARGIN*2,RATIO*11.0*3.0+4.75);
    ofSetColor(255);
    mondrian.caption.drawString(mondrian.text[0],
                                RATIO*MARGIN,RATIO*PADDING,
                                mondrian.w,WINDOW_HEIGHT,UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_LEFT);
    mondrian.caption.drawString(mondrian.text[1],
                                RATIO*MARGIN+RATIO*PADDING+mondrian.w,RATIO*PADDING,
                                mondrian.w,WINDOW_HEIGHT,UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_LEFT);
    mondrian.caption.drawString(mondrian.text[2],
                                RATIO*MARGIN+RATIO*(PADDING+mondrian.w)*2,RATIO*PADDING,
                                mondrian.w,WINDOW_HEIGHT,UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_LEFT);
    mondrian.caption.drawString(mondrian.text[3],
                                RATIO*MARGIN+RATIO*(PADDING+mondrian.w)*3,RATIO*PADDING,
                                mondrian.w,WINDOW_HEIGHT,UL2_TEXT_ALIGN_V_TOP|UL2_TEXT_ALIGN_LEFT);
    
    if(mondrian.typing){
        mondrian._command0.push_back('_');
        mondrian.command0=string(mondrian._command0.begin(),mondrian._command0.end());
        mondrian._command0.pop_back();
        
        ofSetColor(255);
        ofRect(mondrian.command.getStringBoundingBox(mondrian.command0,RATIO*PADDING,
                                                     WINDOW_HEIGHT-RATIO*PADDING));
        ofSetColor(0);
        mondrian.command.drawString(mondrian.command0,RATIO*PADDING,
                                    WINDOW_HEIGHT-RATIO*PADDING);
    }
    
    if(white)ofBackground(255);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::keyPressed(int key){
    if(mondrian.typing){
        mondrian.typed(key);
    }else if(mondrian.operating){
        mondrian.pressed(key);
    }else{
        if(key=='i')glitch.setFx(OFXPOSTGLITCH_CONVERGENCE,true);
        if(key=='c')glitch.setFx(OFXPOSTGLITCH_CUTSLIDER,true);
        if(key=='t')glitch.setFx(OFXPOSTGLITCH_TWIST,true);
        if(key=='m')glitch.setFx(OFXPOSTGLITCH_SLITSCAN,true);
        if(key=='h')glitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST,true);
        if(key=='1')glitch.setFx(OFXPOSTGLITCH_CR_REDINVERT,true);
        if(key=='2')glitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT,true);
        if(key=='3')glitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT,true);
    
        if(key=='s'){
            screenShoted=true;
            screenShotNum++;
        }
    
        if(key=='j'&&!jpGlitched&&!pnGlitched&&!pieted){
            ofBuffer file=ofBufferFromFile("screenshot.jpg");
            char *buffer=file.getBinaryBuffer();
            char byteCheck=(char)round(ofRandom(0,255));
            char byteCount=(char)round(ofRandom(0,255));
            for(int i=0;i<file.size();i++){
                if(buffer[i]==byteCheck){
                    buffer[i]=byteCount;
                }
            }
            ofBufferToFile("jpglitch.jpg",file);
            jpGlitched=true;
            if(mondrian.colorMode==mondrian.RYB)glitch.setFx(OFXPOSTGLITCH_RYB,true);
            else if(mondrian.colorMode==mondrian.CMY)glitch.setFx(OFXPOSTGLITCH_CMY,true);
            else glitch.setFx(OFXPOSTGLITCH_K,true);
        }
    
        if(key=='p'&&!pnGlitched&&!jpGlitched&&!pieted){
            pnGlitched=true;
            if(mondrian.colorMode==mondrian.RYB)glitch.setFx(OFXPOSTGLITCH_RYB,true);
            else if(mondrian.colorMode==mondrian.CMY)glitch.setFx(OFXPOSTGLITCH_CMY,true);
            else glitch.setFx(OFXPOSTGLITCH_K,true);
            pnGlitchNum++;
        }
        
        if(key=='a'&&!pieted&&!jpGlitched&&!pnGlitched){
            float _r=ofRandom(0,100);
            if(_r<100.0/3.0)pietNum=1;
            else if(_r<100.0/3.0*2.0)pietNum=2;
            else pietNum=3;
            pieted=true;
            
        }
    
        if(key==32){
            mondrian.typing=true;
            mondrian.type=ofGetElapsedTimeMillis();
        }
        
        if(key==OF_KEY_COMMAND){
            mondrian.operating=true;
            mondrian.operate=ofGetElapsedTimeMillis();
        }
        
        if(key=='-')mondrian.stop=true;
        if(key==OF_KEY_BACKSPACE)white=true;
        if(key==OF_KEY_TAB);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::keyReleased(int key){
    if(!mondrian.typing){
        if(key=='i')glitch.setFx(OFXPOSTGLITCH_CONVERGENCE,false);
        if(key=='c')glitch.setFx(OFXPOSTGLITCH_CUTSLIDER,false);
        if(key=='t')glitch.setFx(OFXPOSTGLITCH_TWIST,false);
        if(key=='m')glitch.setFx(OFXPOSTGLITCH_SLITSCAN,false);
        if(key=='h')glitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST,false);
        if(key=='1')glitch.setFx(OFXPOSTGLITCH_CR_REDINVERT,false);
        if(key=='2')glitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT,false);
        if(key=='3')glitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT,false);

        if(key=='j'&&!pnGlitched){
            jpGlitched=false;
            glitch.setFx(OFXPOSTGLITCH_RYB,false);
            glitch.setFx(OFXPOSTGLITCH_CMY,false);
            glitch.setFx(OFXPOSTGLITCH_K,false);
        }
        if(key=='p'&&!jpGlitched){
            pnGlitched=false;
            glitch.setFx(OFXPOSTGLITCH_RYB,false);
            glitch.setFx(OFXPOSTGLITCH_CMY,false);
            glitch.setFx(OFXPOSTGLITCH_K,false);
        }
        if(key=='a')pieted=false;
        
        if(key==OF_KEY_BACKSPACE)white=false;
        
        if(key=='-')mondrian.stop=false;
        mondrian.released(key);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::setBpm(float b,float t){
    bpm=b;
    time=t;
    spb=1000.0/(bpm/60.0);
    fps=time*(bpm/60.0);
    frameCount=0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::mousePressed(int x,int y,int button){
    click.push_back(ofGetElapsedTimeMillis());
    if(3<click.size()){
        setBpm(time/((click[3]-click[0])/3)*1000.0*60.0/time,time);
        click.clear();
        frameCount=0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::mouseReleased(int x,int y,int button){

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::mouseMoved(int x,int y){
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::mouseDragged(int x,int y,int button){
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::windowResized(int w,int h){
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::gotMessage(ofMessage msg){

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ofApp::dragEvent(ofDragInfo dragInfo){

}