uniform sampler2DRect image;
uniform float rand;
uniform int range;

varying vec3 pos;

void main(void)
{
    vec2 texCoord=vec2(pos.x,pos.y);
    vec4 col=texture2DRect(image,texCoord);
    
    int index=1;

    if(col.r<col.g)index=2;
    if(col.g<col.b)index=3;
    
    if(index==1){
        col.r=col.r;
        col.g=col.r;
        col.b=col.r;
    }else if(index==2){
        col.r=col.g;
        col.g=col.g;
        col.b=col.g;
    }else{
        col.r=col.b;
        col.g=col.b;
        col.b=col.b;
    }
    
    gl_FragColor.rgba = col.rgba;
}