#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
const int WIDTH=1000;      // Window Width
const int HEIGHT=1000;     // Window Height
const int CELL_SIZE = 20; // Size of each grid cell

int cols = WIDTH / CELL_SIZE;  // Number of columns in the grid
int rows = HEIGHT / CELL_SIZE; // Number of rows in the grid
int tick = 0;
struct Vec2{
    int x,y; // coordinates of vector
};

std::vector<Vec2> snake = { {cols / 2, rows / 2}}; // Snake will initially be at the center of grid
int dirX = 1, dirY = 0; // Directions, initially moving to the right (+1 = right, -1 = left)

void drawCell(int x, int y,float r, float g, float b){
    glColor3f(r,g,b);                             // Set drawing color
    glBegin(GL_QUADS);                            // Start drawing
    glVertex2f(x*CELL_SIZE, y*CELL_SIZE);         // Bottom left corner
    glVertex2f((x+1)*CELL_SIZE, y*CELL_SIZE);     // Bottom right corner
    glVertex2f((x+1)*CELL_SIZE, (y+1)*CELL_SIZE);     // Top left corner
    glVertex2f(x*CELL_SIZE,(y+1)*CELL_SIZE);  // Top right corner
    glEnd();                                      // Finish drawing the box
    //std::cout<<"Drawing at "<< x<< ","<<y << std::endl;
}
void drawWhite(){
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(0,0);glVertex2f(0,WIDTH);glVertex2f(HEIGHT,0);glVertex2f(HEIGHT,WIDTH);
    glEnd();
}

void update(){
    tick++;
    if (tick%6!=0) return;
    // Move the snake's body cells (tail->head)
    for(int i = snake.size()-1;i>0;--i){
        snake[i] = snake[i-1]; //Each segment takes position of segment before
    }
    //Update the head cell based on direction
    snake[0].x += dirX;
    snake[0].y += dirY;
    
    // Wrap the snake around the screen if it goes out of bounds
    if(snake[0].x >= cols) snake[0].x =0;    // Wrap right edge -> left edge
    if(snake[0].x < 0) snake[0].x =cols-1;  // Wrap left edge -> right edge
    if(snake[0].y >= rows) snake[0].y =0;    // Wrap bottom edge -> top edge
    if(snake[0].y < 0) snake[0].y =rows-1;  // Wrap top ledge -> bottom edge
    printf("Snake is at %d,%d\n",snake[0].x,snake[0].y);
    
}
int main(){
    if(!glfwInit()){
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT, "Snake Game",NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight); // logical size (what you requested)

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight); // actual rendering area
    
    glViewport(0, 0, fbWidth, fbHeight); // correct rendering dimensions
    
    // Optional: print sizes to debug
    printf("Window size: %d x %d\n", windowWidth, windowHeight);
    printf("Framebuffer size: %d x %d\n", fbWidth, fbHeight);
    printf("Rows : %d, Columns : %d\n", rows,cols);
    glfwMakeContextCurrent(window);  // Make the OpenGL context of the created window current
    glewInit();                    // Initialize GLEW (to allow access to modern OpenGL functions)

    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);  // Set orthogonal 2D projection for OpenGL rendering
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT); // Clear Screen

        // Input handling :
        if(glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS && dirY!=1){ // UP KEY
            dirX = 0; dirY = -1; // Move Up (-y direction)
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN)==GLFW_PRESS && dirY!=-1){ // DOWN KEY
            dirX = 0; dirY = 1; // Move Down (+y direction)
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT)==GLFW_PRESS && dirX!=1){ // LEFT KEY
            dirX = -1; dirY = 0; // Move Left (-x direction)
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT)==GLFW_PRESS && dirX!=-1){ // RIGHT KEY
            dirX = 1; dirY = 0; // Move Right (+x direction)
        }
        update();
        for (auto& s : snake) { 
            drawCell(s.x, s.y, 0.0f, 1.0f, 0.0f);  // Draw each snake segment (green color)
        }
        drawWhite();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}