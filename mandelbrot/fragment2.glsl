#version 330 core

in vec2 xy;

out vec4 color;

//uniform int max_iter;
uniform vec2 c;
//uniform float scale;
uniform float scale;
uniform vec2 center;

void main(){
    int iter = 0;
    float x = 0.0f;
    float y = 0.0f;
    //    float limit = 4.0f * scale * scale;
    float cx = scale * xy.x + center.x;
    float cy = scale * xy.y + center.y;
    while (iter < 96 && x * x + y * y < 4.0f) {
        float x0 = x * x - y * y + cx;
        float y0 = 2.0f * x * y + cy;
        x = x0;
        y = y0;
        iter++;
    }
    float rate = float(iter) / float(96);
    color = vec4(0.0f, rate, 1.0f - rate, 1.0f);
}

