#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
using namespace glm;

GLFWwindow* window;
int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
double scaleTransformSpeed = 0.2f;
double displacementSpeed = 0.5f;
double shapeTransformSpeed = 0.1f;

GLfloat c[2] = {0.0f, 0.0f};
GLfloat center[2] = {0.0f, 0.0f};
GLfloat scaleXY[2];

GLuint cID, centerID, scaleID;

std::vector<GLfloat> vertices = {
    -1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f
};

void WindowSizeCallback(GLFWwindow * window, int width, int height);

int main(int args, const char* argv[])
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Mandelbrot", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    glfwSetWindowSizeCallback(window, WindowSizeCallback);
    
    glfwPollEvents();
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // Create and compile GLSL program
    // Use you own path!!!
    GLuint programID = LoadShaders( "vertices.glsl", "fragment.glsl");
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                          0,                  // attribute
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );
    
    cID = glGetUniformLocation(programID, "c");
    centerID = glGetUniformLocation(programID, "center");
    scaleID = glGetUniformLocation(programID, "scale");
    
    if (WINDOW_WIDTH > WINDOW_HEIGHT) {
        scaleXY[1] = 1.0;
        scaleXY[0] = WINDOW_WIDTH / WINDOW_HEIGHT;
    }else {
        scaleXY[0] = 1.0;
        scaleXY[1] = WINDOW_HEIGHT / WINDOW_WIDTH;
    }
    
    glUseProgram(programID);
    
    glUniform2fv(scaleID, 1, scaleXY);
    glUniform2fv(centerID, 1, center);
    glUniform2fv(cID, 1, c);
    
    double lastCommit = 0.0, currentTime = 0.0;
    
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           !glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        lastCommit = currentTime;
        currentTime = glfwGetTime();
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        // handle events. a better way is to register a callback function.
        if (glfwGetKey(window, GLFW_KEY_EQUAL ) == GLFW_PRESS) {
            scaleXY[0] /= 1 + (currentTime - lastCommit) * scaleTransformSpeed;
            scaleXY[1] /= 1 + (currentTime - lastCommit) * scaleTransformSpeed;
            glUniform2fv(scaleID, 1, scaleXY);
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS ) == GLFW_PRESS) {
            scaleXY[0] *= 1 + (currentTime - lastCommit) * scaleTransformSpeed;
            scaleXY[1] *= 1 + (currentTime - lastCommit) * scaleTransformSpeed;
            glUniform2fv(scaleID, 1, scaleXY);
        }
        if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS) {
            center[1] += (currentTime - lastCommit) * displacementSpeed * scaleXY[1];
            glUniform2fv(centerID, 1, center);
        }
        if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS) {
            center[1] -= (currentTime - lastCommit) * displacementSpeed * scaleXY[1];
            glUniform2fv(centerID, 1, center);
        }
        if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS) {
            center[0] += (currentTime - lastCommit) * displacementSpeed * scaleXY[0];
            glUniform2fv(centerID, 1, center);
        }
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS) {
            center[0] -= (currentTime - lastCommit) * displacementSpeed * scaleXY[0];
            glUniform2fv(centerID, 1, center);
        }
        if (glfwGetKey(window, GLFW_KEY_J ) == GLFW_PRESS) {
            c[0] += (currentTime - lastCommit) * shapeTransformSpeed;
            glUniform2fv(cID, 1, c);
        }
        if (glfwGetKey(window, GLFW_KEY_K ) == GLFW_PRESS) {
            c[0] -= (currentTime - lastCommit) * shapeTransformSpeed;
            glUniform2fv(cID, 1, c);
        }
        if (glfwGetKey(window, GLFW_KEY_N ) == GLFW_PRESS) {
            c[1] += (currentTime - lastCommit) * shapeTransformSpeed;
            glUniform2fv(cID, 1, c);
        }
        if (glfwGetKey(window, GLFW_KEY_M ) == GLFW_PRESS) {
            c[1] -= (currentTime - lastCommit) * shapeTransformSpeed;
            glUniform2fv(cID, 1, c);
        }
        
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
        glfwSwapBuffers(window);
    }
    
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

void WindowSizeCallback(GLFWwindow * window, int width, int height) {
    scaleXY[0] *= (GLfloat)width / (GLfloat)WINDOW_WIDTH;
    scaleXY[1] *= (GLfloat)height / (GLfloat)WINDOW_HEIGHT;
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUniform2fv(scaleID, 1, scaleXY);
    
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
    glfwSwapBuffers(window);
}
