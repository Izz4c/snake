#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int WIDTH=800;
const int HEIGHT=600;

int main(){
    if(!glfwInit()){
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT, "Snake Game",NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    glOrtho(0,WIDTH,HEIGHT,0,-1,1); 

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT); //Clear Screen
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
    
}