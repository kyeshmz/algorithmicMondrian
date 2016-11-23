#include "mondrian.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
mondrian::mondrian(){
    fft.setup();
    
    drawModeChanged=true;
    
    drawMode=BLAST;
    mondrianMode=COMPOSITION;
    colorMode=RYB;
    setColorMode(colorMode);
    
    strokeWeight=2;
    spacingMinRatio=1;
    spacingMaxRatio=5;
    hideRatio=50;
    splitRatio=50;
    colorRatio=10;
    
    caption.loadFont("helveticaNeueLight.otf",RATIO*4.75,false,false,0.3f,0,true);
    command.loadFont("helveticaNeueBold.otf",RATIO*75.0,false,true,0.3f,0,true);
    caption.setLetterSpacing(0.175);
    caption.setLineHeight(RATIO*11.0);
    command.setLetterSpacing(-0.175);
    command.setLineHeight(RATIO*150.0);
    parameter.useProportional(true);
    caption.useProportional(true);
    command.useProportional(true);
    
    w=RATIO*(WINDOW_WIDTH-MARGIN*2-PADDING*3)/4;
    h=WINDOW_HEIGHT;
    
    text.push_back("        Piet Mondrian, born in 1872 in the Netherlands, studied at Amsterdam's Royal Academy and painted traditional Dutch landscapes until 1908 when he began to experiment in newer styles. Attracted by Cubism, he moved to Paris from 1912 to 1914 and painted realistic themes in abstract terms.");
    text.push_back("        Back in the Netherlands, Mondrian's works reflected his years in Paris with a new simplicity of line and color. In 1917 he co-founded the journal De Stijl which published his essays about his theory of Neoplasticism. Mondrian, then 47, returned to Paris in 1919.");
    text.push_back("        There his work showed a radical transformation. His paintings had a dynamic balance with blocks of primary colors and planes of white and grey divided by grids of black lines. His recognition grew and his paintings were purchased by collecters and museums in Europe and America.");
    text.push_back("        As World War II approached, Mondrian moved to London and in 1940 to New York where in paintings like Broadway Boogie Woogie, he replaced the black grid lines with colored lines leaving a creative legacy that affects the way we view art today.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::reset(){
    _strokeWeight=strokeWeight;
    
    x.clear();
    y.clear();
    xy.clear();
    
    c.clear();
    a.clear();
    
    x.push_back(0);
    y.push_back(0);
    
    for(float _x=ofRandom(-2.0*round(ofRandom(spacingMinRatio,spacingMaxRatio))*strokeWeight,0);
        _x<WINDOW_WIDTH;
        _x+=2.0*round(ofRandom(spacingMinRatio,spacingMaxRatio))*strokeWeight){
        if(strokeWeight/2.0<_x&&_x<WINDOW_WIDTH-strokeWeight/2.0){
            x.push_back(_x);
        }
    }
    
    for(float _y=ofRandom(-2.0*round(ofRandom(spacingMinRatio,spacingMaxRatio))*strokeWeight,0);
        _y<WINDOW_HEIGHT;
        _y+=2.0*round(ofRandom(spacingMinRatio,spacingMaxRatio))*strokeWeight){
        if(strokeWeight/2.0<_y&&_y<WINDOW_HEIGHT-strokeWeight/2.0){
            y.push_back(_y);
        }
    }
    
    x.push_back(WINDOW_WIDTH+1);
    y.push_back(WINDOW_HEIGHT+1);
    
    for(_x=0;_x<x.size();_x++){
        vector<mondrian2f> v;
        for(_y=0;_y<y.size();_y++){
            bool b1=true;
            bool b2=true;
            _r=ofRandom(0,100);
            if(_r<hideRatio)b1=false;
            _r=ofRandom(0,100);
            if(_r<hideRatio)b2=false;
            mondrian2f m(x[_x],y[_y],b1,b2);
            v.push_back(m);
        }
        xy.push_back(v);
    }
    
    for(_x=0;_x+1<x.size();_x++){
        for(_y=0;_y+1<y.size();_y++){
            if(_x==0)xy[_x][_y].drawY=true;
            else xy[_x][_y].drawX=xy[_x-1][_y].drawX;
            if(_y==0)xy[_x][_y].drawX=true;
            else xy[_x][_y].drawY=xy[_x][_y-1].drawY;
            
            _r=ofRandom(0,100);
            if(0<_y&&_r<splitRatio/2){
                xy[_x][_y].drawX=!xy[_x][_y].drawX;
                if(xy[_x][_y].drawX){
                    if(xy[_x][_y].drawX!=xy[_x][_y-1].drawY)xy[_x][_y].drawX=!xy[_x][_y].drawX;
                }else{
                    if(xy[_x][_y].drawX==xy[_x][_y-1].drawY)xy[_x][_y].drawX=!xy[_x][_y].drawX;
                }
            }else if(0<_x&&_r<splitRatio){
                xy[_x][_y].drawY=!xy[_x][_y].drawY;
                if(xy[_x][_y].drawY){
                    if(xy[_x][_y].drawY!=xy[_x-1][_y].drawX)xy[_x][_y].drawY=!xy[_x][_y].drawY;
                }else{
                    if(xy[_x][_y].drawY==xy[_x-1][_y].drawX)xy[_x][_y].drawY=!xy[_x][_y].drawY;
                }
            }
            
            xy[_x][_y].point=color(POINT);
            if(xy[_x][_y].drawX&&xy[_x][_y].drawY){
                xy[_x][_y].plane=color(PLANE);
                xy[_x][_y].lineX=color(LINE);
                xy[_x][_y].lineY=color(LINE);
            }else if(xy[_x][_y].drawX&&0<_x){
                xy[_x][_y].plane=xy[_x-1][_y].plane;
                xy[_x][_y].lineX=xy[_x-1][_y].lineX;
            }else if(xy[_x][_y].drawY&&0<_y){
                xy[_x][_y].plane=xy[_x][_y-1].plane;
                xy[_x][_y].lineY=xy[_x][_y-1].lineY;
            }else if(0<_x&&0<_y){
                xy[_x][_y].plane=xy[_x-1][_y-1].plane;
            }
            
            if(mondrianMode==BOOGIE_WOOGIE){
                if(0<_x&&0<_y){
                    if((xy[_x][_y].drawX&&xy[_x][_y].drawY)||
                       (xy[_x][_y].drawX&&xy[_x][_y-1].drawY)||
                       (xy[_x-1][_y].drawX&&xy[_x][_y].drawY)){
                        xy[_x][_y].section=true;
                        car _c(xy[_x][_y].x,xy[_x][_y].y,_x,_y,UP);
                        _c.length=round(ofRandom(strokeWeight,strokeWeight*2));
                        _c.speed=round(ofRandom(1,3));
                        _c.body=color(CAR);
                        if(_c.speed==3)_c.speed=4;
                        c.push_back(_c);
                    }
                }
                
                if(xy[_x][_y].drawX||xy[_x][_y].drawY){
                    _r=ofRandom(0,100);
                    if(_r<colorRatio){
                        int _xNum=_x+1;
                        int _yNum=_y+1;
                        if(xy[_x][_y].drawX&&xy[_x][_y].drawY){
                            if(_r<colorRatio/2.0){
                                while(!xy[_x][_yNum].drawY&&_yNum+1<y.size()-1)_yNum++;
                            }else{
                                while(!xy[_xNum][_y].drawX&&_xNum+1<x.size()-1)_xNum++;
                            }
                        }else if(xy[_x][_y].drawX){
                            while(!xy[_x][_yNum].drawY&&_yNum+1<y.size()-1)_yNum++;
                        }else{
                            while(!xy[_xNum][_y].drawX&&_xNum+1<x.size()-1)_xNum++;
                        }
                        architecture _a(_x,_y,_xNum,_yNum);
                        _a.exterior=color(PLANE);
                        _a.interior=color(POINT);
                        a.push_back(_a);
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::update(){
    if(mondrianMode==COMPOSITION){
        reset();
    }else if(mondrianMode==BOOGIE_WOOGIE){
        for(int _c=0;_c<c.size();_c++){
            _r=ofRandom(0,100);
            if(((c[_c].direction==UP||c[_c].direction==DOWN)&&
                (c[_c].y==xy[c[_c].xNum][c[_c].yNum].y))||
               ((c[_c].direction==LEFT||c[_c].direction==RIGHT)&&
                (c[_c].x==xy[c[_c].xNum][c[_c].yNum].x))){
                   if(!xy[c[_c].xNum][c[_c].yNum-1].drawY){
                       if(_r<100.0/3.0){
                           c[_c].direction=DOWN;
                           c[_c].yNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum++;
                               if(y.size()-1<c[_c].yNum)c[_c].yNum=0;
                           }
                       }else if(_r<100.0/3.0*2.0){
                           c[_c].direction=LEFT;
                           c[_c].xNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum--;
                               if(c[_c].xNum<0)c[_c].xNum=x.size()-1;
                           }
                       }else{
                           c[_c].direction=RIGHT;
                           c[_c].xNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum++;
                               if(x.size()-1<c[_c].xNum)c[_c].xNum=0;
                           }
                       }
                   }else if(!xy[c[_c].xNum][c[_c].yNum].drawY){
                       if(_r<100.0/3.0){
                           c[_c].direction=UP;
                           c[_c].yNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum--;
                               if(c[_c].yNum<0)c[_c].yNum=y.size()-1;
                           }
                       }else if(_r<100.0/3.0*2.0){
                           c[_c].direction=LEFT;
                           c[_c].xNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum--;
                               if(c[_c].xNum<0)c[_c].xNum=x.size()-1;
                           }
                       }else{
                           c[_c].direction=RIGHT;
                           c[_c].xNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum++;
                               if(x.size()-1<c[_c].xNum)c[_c].xNum=0;
                           }
                       }
                   }else if(!xy[c[_c].xNum-1][c[_c].yNum].drawX){
                       if(_r<100.0/3.0){
                           c[_c].direction=UP;
                           c[_c].yNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum--;
                               if(c[_c].yNum<0)c[_c].yNum=y.size()-1;
                           }
                       }else if(_r<100.0/3.0*2.0){
                           c[_c].direction=DOWN;
                           c[_c].yNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum++;
                               if(y.size()-1<c[_c].yNum)c[_c].yNum=0;
                           }
                       }else{
                           c[_c].direction=RIGHT;
                           c[_c].xNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum++;
                               if(x.size()-1<c[_c].xNum)c[_c].xNum=0;
                           }
                       }
                   }else if(!xy[c[_c].xNum][c[_c].yNum].drawX){
                       if(_r<100.0/3.0){
                           c[_c].direction=UP;
                           c[_c].yNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum--;
                               if(c[_c].yNum<0)c[_c].yNum=y.size()-1;
                           }
                       }else if(_r<100.0/3.0*2.0){
                           c[_c].direction=DOWN;
                           c[_c].yNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum++;
                               if(y.size()-1<c[_c].yNum)c[_c].yNum=0;
                           }
                       }else{
                           c[_c].direction=LEFT;
                           c[_c].xNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum--;
                               if(c[_c].xNum<0)c[_c].xNum=x.size()-1;
                           }
                       }
                   }else{
                       if(_r<100.0/4.0){
                           c[_c].direction=UP;
                           c[_c].yNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum--;
                               if(c[_c].yNum<0)c[_c].yNum=y.size()-1;
                           }
                       }else if(_r<100.0/4.0*2.0){
                           c[_c].direction=DOWN;
                           c[_c].yNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].yNum++;
                               if(y.size()-1<c[_c].yNum)c[_c].yNum=0;
                           }
                       }else if(_r<100.0/4.0*3.0){
                           c[_c].direction=LEFT;
                           c[_c].xNum--;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum--;
                               if(c[_c].xNum<0)c[_c].xNum=x.size()-1;
                           }
                       }else{
                           c[_c].direction=RIGHT;
                           c[_c].xNum++;
                           while(!xy[c[_c].xNum][c[_c].yNum].section){
                               c[_c].xNum++;
                               if(x.size()-1<c[_c].xNum)c[_c].xNum=0;
                           }
                       }
                   }
               }
            
            switch(c[_c].direction){
                case UP:
                    c[_c].y-=strokeWeight/16.0*c[_c].speed;
                    //if(c[_c].y<0)c[_c].y=WINDOW_HEIGHT;
                    break;
                case DOWN:
                    c[_c].y+=strokeWeight/16.0*c[_c].speed;
                    //if(WINDOW_HEIGHT<c[_c].y)c[_c].y=0;
                    break;
                case LEFT:
                    c[_c].x-=strokeWeight/16.0*c[_c].speed;
                    //if(c[_c].x<0)c[_c].x=WINDOW_WIDTH;
                    break;
                case RIGHT:
                    c[_c].x+=strokeWeight/16.0*c[_c].speed;
                    //if(WINDOW_WIDTH<c[_c].x)c[_c].x=0;
                    break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::draw(){
    if(mondrianMode==COMPOSITION)composition();
    else if(mondrianMode==BOOGIE_WOOGIE)boogieWoogie();
    else ryojiikeda();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::composition(){
    ofSetRectMode(OF_RECTMODE_CORNER);
    for(_x=0;_x+1<x.size();_x++){
        for(_y=0;_y+1<y.size();_y++){
            ofSetColor(xy[_x][_y].plane);
            ofRect(xy[_x][_y].x,xy[_x][_y].y,
                   xy[_x+1][_y].x-xy[_x][_y].x,xy[_x][_y+1].y-xy[_x][_y].y);
        }
    }
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    for(_x=0;_x+1<x.size();_x++){
        for(_y=0;_y+1<y.size();_y++){
            ofSetColor(LINE);
            if(0<_x&&xy[_x][_y].drawY){
                ofRect(xy[_x][_y].x,(xy[_x][_y].y+xy[_x][_y+1].y)/2,
                       _strokeWeight,xy[_x][_y+1].y-xy[_x][_y].y);
            }
            if(0<_y&&xy[_x][_y].drawX){
                ofRect((xy[_x][_y].x+xy[_x+1][_y].x)/2,xy[_x][_y].y,
                       xy[_x+1][_y].x-xy[_x][_y].x,_strokeWeight);
            }
            if(0<_x&&0<_y){
                if(xy[_x][_y].section){
                    ofSetColor(xy[_x][_y].point);
                    ofRect(xy[_x][_y].x,xy[_x][_y].y,_strokeWeight,_strokeWeight);
                }
            }
        }
    }
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::boogieWoogie(){
    ofSetRectMode(OF_RECTMODE_CORNER);
    for(int _a=0;_a+1<a.size();_a++){
        ofSetColor(a[_a].exterior);
        ofRect(xy[a[_a].xNum1][a[_a].yNum1].x,xy[a[_a].xNum1][a[_a].yNum1].y,
               xy[a[_a].xNum2][a[_a].yNum2].x-xy[a[_a].xNum1][a[_a].yNum1].x,
               xy[a[_a].xNum2][a[_a].yNum2].y-xy[a[_a].xNum1][a[_a].yNum1].y);
        
        ofSetColor(a[_a].interior);
        ofRect((xy[a[_a].xNum1][a[_a].yNum1].x)+
               (xy[a[_a].xNum2][a[_a].yNum2].x-xy[a[_a].xNum1][a[_a].yNum1].x)/4.0,
               (xy[a[_a].xNum1][a[_a].yNum1].y)+
               (xy[a[_a].xNum2][a[_a].yNum2].y-xy[a[_a].xNum1][a[_a].yNum1].y)/4.0,
               (xy[a[_a].xNum2][a[_a].yNum2].x-xy[a[_a].xNum1][a[_a].yNum1].x)/2.0,
               (xy[a[_a].xNum2][a[_a].yNum2].y-xy[a[_a].xNum1][a[_a].yNum1].y)/2.0);
    }
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    for(_x=0;_x+1<x.size();_x++){
        for(_y=0;_y+1<y.size();_y++){
            ofSetColor(xy[_x][_y].lineY);
            if(0<_x&&xy[_x][_y].drawY){
                ofRect(xy[_x][_y].x,(xy[_x][_y].y+xy[_x][_y+1].y)/2,
                       _strokeWeight,xy[_x][_y+1].y-xy[_x][_y].y);
            }
            ofSetColor(xy[_x][_y].lineX);
            if(0<_y&&xy[_x][_y].drawX){
                ofRect((xy[_x][_y].x+xy[_x+1][_y].x)/2,xy[_x][_y].y,
                       xy[_x+1][_y].x-xy[_x][_y].x,_strokeWeight);
            }
            if(0<_x&&0<_y){
                if(xy[_x][_y].section){
                    ofSetColor(xy[_x][_y].point);
                    ofRect(xy[_x][_y].x,xy[_x][_y].y,_strokeWeight,_strokeWeight);
                }
            }
        }
    }
    
    
    for(int _c=0;_c<c.size();_c++){
        ofSetColor(c[_c].body);
        if(c[_c].direction==UP||c[_c].direction==DOWN){
            ofRect(c[_c].x,c[_c].y,strokeWeight,c[_c].length);
        }else{
            ofRect(c[_c].x,c[_c].y,c[_c].length,strokeWeight);
        }
    }
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::ryojiikeda(){
    fft.drawDebug();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::changeColor(){
    _r=ofRandom(0,100);
    _x=round(ofRandom(1,x.size()-1));
    _y=round(ofRandom(1,y.size()-1));
    if(xy[_x][_y].drawX&&xy[_x][_y].drawY){
        bool b=true;
        if(_r<100.0/2.0){
            xy[_x][_y].lineX=color(LINE);
            while(b){
                _x++;
                if(x.size()-1<_x+1||(xy[_x][_y].drawX&&xy[_x][_y].drawY))b=false;
                else xy[_x][_y].lineX=xy[_x-1][_y].lineX;
            }
        }else{
            xy[_x][_y].lineY=color(LINE);
            while(b){
                _y++;
                if(y.size()-1<_y+1||(xy[_x][_y].drawX&&xy[_x][_y].drawY))b=false;
                else xy[_x][_y].lineY=xy[_x][_y-1].lineY;
            }
        }
    }
    
    if(0<a.size()){
        _r=ofRandom(0,100);
        int _a=round(ofRandom(0,a.size()-1));
        if(_r<100.0/4.0/2.0)a[_a].exterior=color(PLANE);
        else if(_r<100.0/4.0) a[_a].interior=color(POINT);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ofColor mondrian::color(int f){
    ofColor c;
    _r=ofRandom(0,100);
    
    if(f==LINE){
        if(mondrianMode==BOOGIE_WOOGIE){
            if(_r<colorRatio/2.0){
                c=red;
            }else if(_r<colorRatio){
                c=blue;
            }else{
                c=yellow;
            }
        }else if(mondrianMode==COMPOSITION){
            c=black;
        }
    }else if(f==PLANE){
        if(mondrianMode==BOOGIE_WOOGIE){
            if(_r<100.0/3.0){
                c=red;
            }else if(_r<100.0/3.0*2.0){
                c=blue;
            }else{
                c=yellow;
            }
        }else{
            if(mondrianMode==COMPOSITION){
                if(_r<colorRatio/3.0){
                    c=red;
                }else if(_r<colorRatio/3.0*2.0){
                    c=blue;
                }else if(_r<colorRatio){
                    c=yellow;
                }else{
                    c=white;
                }
            }
        }
    }else if(f==POINT){
        if(mondrianMode==BOOGIE_WOOGIE){
            if(_r<100.0/4.0){
                c=red;
            }else if(_r<100.0/4.0*2.0){
                c=blue;
            }else if(_r<100.0/4.0*3.0){
                c=yellow;
            }else{
                c=gray;
            }
        }else{
            c=black;
        }
    }else if(f==CAR){
        if(_r<100.0/3.0){
            c=red;
        }else if(_r<100.0/3.0*2.0){
            c=blue;
        }else{
            c=gray;
        }
    }
    
    return c;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::setColorMode(int c){
    if(colorMode==RYB){
        red=ofColor(255,32,0);
        blue=ofColor(64,0,255);
        yellow=ofColor(255,255,32);
        black=ofColor(0);
        gray=ofColor(192);
    }else if(colorMode==CMY){
        red=ofColor(0,255,255);
        blue=ofColor(255,0,255);
        yellow=ofColor(255,255,0);
        black=ofColor(0);
        gray=ofColor(192);
    }else if(colorMode==K){
        red=ofColor(0);
        blue=ofColor(0);
        yellow=ofColor(0);
        black=ofColor(0);
        gray=ofColor(192);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::typed(int key){
    if(key!=27&&key!=165&&key!=768&&key!=769){
        if(key==OF_KEY_RETURN){
            int num;
            bool b=false;
            
            for(int i=0;i<_command0.size();i++){
                if(_command0[i]==' ')b=true;
                else if(b)_command2.push_back(_command0[i]);
                else _command1.push_back(_command0[i]);
            }
            
            command1=string(_command1.begin(),_command1.end());
            command2=string(_command2.begin(),_command2.end());
            
            if(command2=="SYNC"){
                num=SYNC;
            }else if(command2=="BLAST"){
                num=BLAST;
            }else if(command2=="COMPOSITION"){
                num=COMPOSITION;
            }else if(command2=="BOOGIE_WOOGIE"){
                num=BOOGIE_WOOGIE;
            }else if(command2=="IKEDA"){
                num=IKEDA;
            }else if(command2=="RYB"){
                num=RYB;
            }else if(command2=="CMY"){
                num=CMY;
            }else if(command2=="K"){
                num=K;
            }else{
                istringstream i(command2);
                i>>num;
            }
            
            if(0<=num){
                if(command1=="drawMode"||command1=="dm"){
                    if(command2=="SYNC"||command2=="BLAST"){
                        drawMode=num;
                        drawModeChanged=true;
                        if(mondrianMode==BOOGIE_WOOGIE)update();
                    }
                }else if(command1=="mondrianMode"||command1=="mm"){
                    if(command2=="COMPOSITION"||
                       command2=="BOOGIE_WOOGIE"||
                       command2=="IKEDA"){
                        mondrianMode=num;
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }else if(command1=="colorMode"||command1=="cm"){
                    if(command2=="RYB"||command2=="CMY"||command2=="K"){
                        colorMode=num;
                        setColorMode(colorMode);
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }else if(command1=="strokeWeight"||command1=="sw"){
                    if(2<=num&&num<=20){
                        strokeWeight=num;
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }else if(command1=="spacingMinRatio"||command1=="snr"){
                    if(1<=num&&num<=spacingMaxRatio){
                        spacingMinRatio=num;
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }else if(command1=="spacingMaxRatio"||command1=="sxr"){
                    if(spacingMinRatio<=num&&num<=20){
                        spacingMaxRatio=num;
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }else if(command1=="hideRatio"||command1=="hr"){
                    if(0<=num&&num<=100){
                        hideRatio=num;
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }else if(command1=="splitRatio"||command1=="sr"){
                    if(0<=num&&num<=100){
                        splitRatio=num;
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }else if(command1=="colorRatio"||command1=="cr"){
                    if(0<=num&&num<=100){
                        colorRatio=num;
                        if(mondrianMode==BOOGIE_WOOGIE)reset();
                    }
                }
            }
            typing=false;
            command0="";
            command1="";
            command2="";
            _command0.clear();
            _command1.clear();
            _command2.clear();
        }else if(key==OF_KEY_BACKSPACE){
            if(0<_command0.size())_command0.pop_back();
        }else if(key<1000){
            _command0.push_back(key);
        }
        type=ofGetElapsedTimeMillis();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::pressed(int key){
    switch(key){
        case 'w'|'W':
            W=true;
            break;
        case 'i'|'I':
            I=true;
            break;
        case 'a'|'A':
            A=true;
            break;
        case 'h'|'H':
            H=true;
            break;
        case 's'|'S':
            S=true;
            break;
        case 'c'|'C':
            C=true;
            break;
        case 'd'|'D':
            D=true;
            break;
        case 'm'|'M':
            M=true;
            break;
    }
    
    switch(key){
        case OF_KEY_UP:
            if(W){
                strokeWeight+=2;
                if(20<strokeWeight)strokeWeight=20;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(I){
                spacingMinRatio+=1;
                if(spacingMaxRatio<spacingMinRatio)spacingMinRatio=spacingMaxRatio;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(A){
                spacingMaxRatio+=1;
                if(20<spacingMaxRatio)spacingMaxRatio=20;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(H){
                hideRatio+=10;
                if(100<hideRatio)hideRatio=100;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(S){
                splitRatio+=10;
                if(100<splitRatio)splitRatio=100;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(C){
                colorRatio+=10;
                if(100<colorRatio)colorRatio=100;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            break;
        case OF_KEY_DOWN:
            if(W){
                strokeWeight-=2;
                if(strokeWeight<2)strokeWeight=2;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(I){
                spacingMinRatio-=1;
                if(spacingMinRatio<1)spacingMinRatio=1;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(A){
                spacingMaxRatio-=1;
                if(spacingMaxRatio<spacingMinRatio)spacingMaxRatio=spacingMinRatio;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(H){
                hideRatio-=10;
                if(hideRatio<0)hideRatio=0;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(S){
                splitRatio-=10;
                if(splitRatio<0)splitRatio=0;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(C){
                colorRatio-=10;
                if(colorRatio<0)colorRatio=0;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            break;
        case OF_KEY_LEFT:
            if(W){
                strokeWeight=2;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(I){
                spacingMinRatio=1;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(A){
                spacingMaxRatio=spacingMinRatio;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(H){
                hideRatio=0;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(S){
                splitRatio=0;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(C){
                colorRatio=0;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            break;
        case OF_KEY_RIGHT:
            if(W){
                strokeWeight=20;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(I){
                spacingMinRatio=spacingMaxRatio;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(A){
                spacingMaxRatio=20;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(H){
                hideRatio=100;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(S){
                splitRatio=100;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(C){
                colorRatio=100;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            break;
        case OF_KEY_RETURN:
            if(C){
                _r=ofRandom(0,100);
                if(colorMode==RYB){
                    if(_r<100.0/2.0)colorMode=CMY;
                    else colorMode=K;
                }else if(colorMode==CMY){
                    if(_r<100.0/2.0)colorMode=RYB;
                    else colorMode=K;
                }else{
                    if(_r<100.0/2.0)colorMode=RYB;
                    else colorMode=CMY;
                }
                setColorMode(colorMode);
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(D){
                if(drawMode==SYNC)drawMode=BLAST;
                else drawMode=SYNC;
                drawModeChanged=true;
            }
            if(M){
                if(mondrianMode==COMPOSITION)mondrianMode=BOOGIE_WOOGIE;
                else mondrianMode=COMPOSITION;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            break;
        case '0':
            if(W){
                strokeWeight=2;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(I){
                spacingMinRatio=1;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(A){
                spacingMaxRatio=5;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(H){
                hideRatio=50;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(S){
                splitRatio=50;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            if(C){
                colorRatio=10;
                if(mondrianMode==BOOGIE_WOOGIE)reset();
            }
            break;
    }
    
    operate=ofGetElapsedTimeMillis();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mondrian::released(int key){
    switch(key){
        case 'w'|'W':
            W=false;
            break;
        case 'i'|'I':
            I=false;
            break;
        case 'a'|'A':
            A=false;
            break;
        case 'h'|'H':
            H=false;
            break;
        case 's'|'S':
            S=false;
            break;
        case 'c'|'C':
            C=false;
            break;
        case 'd'|'D':
            D=false;
            break;
        case 'm'|'M':
            M=false;
            break;
    }
}
