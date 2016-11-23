uniform sampler2DRect image;
uniform float rand;
uniform int range;

varying vec3 pos;

void main(void)
{
    vec2 texCoord=vec2(pos.x,pos.y);
    vec4 col=texture2DRect(image,texCoord);
    
    int index=1;

    if(col.r==col.g&&col.g==col.b&&col.b==col.r){
        if(col.b<0.5)index=0;
        else index=4;
    }else{
        if(col.r<col.g)index=2;
        if(col.g<col.b)index=3;
        if(col.r==col.g)if(index==1)index=2;
        else if(col.g==col.b)if(index==2)index=3;
    }
    
    if(index==0){
        col.r=0.0;
        col.g=0.0;
        col.b=0.0;
    }else if(index==1){
        col.r=col.r;
        col.g=0.0;
        col.b=col.r;
    }else if(index==2){
        col.r=col.g;
        col.g=col.g;
        col.b=0.0;
    }else if(index==3){
        col.r=0.0;
        col.g=col.b;
        col.b=col.b;
    }else{
        col.r=1.0;
        col.g=1.0;
        col.b=1.0;
    }
    
    gl_FragColor.rgba = col.rgba;
}