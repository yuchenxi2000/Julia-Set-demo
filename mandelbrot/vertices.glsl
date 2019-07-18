#version 410 core

layout(location = 0) in vec3 xyz;

out vec2 xy;

void main(){
    
    gl_Position =  vec4(xyz, 1.0f);
    
    xy = vec2(xyz.x, xyz.y);
}



