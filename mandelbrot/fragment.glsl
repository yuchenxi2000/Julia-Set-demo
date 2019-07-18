#version 410 core
const int MAX_ITER = 96; // adjustable constant

in vec2 xy;
out vec4 color;

//uniform int max_iter;
uniform vec2 c;
//uniform float scale;
uniform vec2 scale;
uniform vec2 center;

void main(){
    int iter = 0;
    float x = scale.x * xy.x + center.x;
    float y = scale.y * xy.y + center.y;
//    float limit = 4.0f * scale * scale;
    while (iter < MAX_ITER && x * x + y * y < 4.0f) {
        float x0 = x * x - y * y + c.x;
        float y0 = 2.0f * x * y + c.y;
        x = x0;
        y = y0;
        iter++;
    }
    float rate = float(iter) / float(MAX_ITER);
    color = vec4(0.0f, rate, 1.0f - rate, 1.0f);
}
