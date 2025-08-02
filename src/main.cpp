#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "sound.h"
#include <math.h>
const int WIDTH=400;      // Window Width
const int HEIGHT=400;     // Window Height
const int CELL_SIZE = 20; // Size of each grid cell
const int EYE_SIZE = 5;   // Control the eye's size
const double DELAY = 0.08;// Control game speed
int cols = WIDTH / CELL_SIZE;  // Number of columns in the grid
int rows = HEIGHT / CELL_SIZE; // Number of rows in the grid
struct Vec2{
    int x,y; // coordinates of vector
    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
};

std::vector<Vec2> snake = { {cols / 2, rows / 2}}; // Snake will initially be at the center of grid
Vec2 fruit;

int dirX = 1, dirY = 0; // Directions, initially moving to the right (+1 = right, -1 = left)
double lastTime = 0.0;
double delay = DELAY;
bool alive=true;
bool pause=false;
void drawCell(int x, int y,float r, float g, float b){
    glColor3f(r,g,b);                                 // Set drawing color
    glBegin(GL_TRIANGLES);                            // Begin drawing
        // First triangle
        glVertex2i(x*CELL_SIZE, (y+1)*CELL_SIZE);     // Bottom left corner
        glVertex2i((x+1)*CELL_SIZE, (y+1)*CELL_SIZE); // Bottom right corner
        glVertex2i((x+1)*CELL_SIZE, y*CELL_SIZE);     // Top right corner
        // Second triangle
        glVertex2i(x*CELL_SIZE, (y+1)*CELL_SIZE);     // Bottom left corner
        glVertex2i((x+1)*CELL_SIZE, y*CELL_SIZE);     // Top right corner
        glVertex2i(x*CELL_SIZE, y*CELL_SIZE);         // Top left corner
    glEnd();                                          // Finish drawing
}
void drawWhite(){
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(0,0);glVertex2f(0,WIDTH);glVertex2f(HEIGHT,0);glVertex2f(HEIGHT,WIDTH);
    glEnd();
}
// Spawn the fruit at a random position that is not on the snake
void spawnFruit() {
    while (true) {
        fruit.x = rand() % cols;                // Keep fruit in the map
        fruit.y = rand() % rows;                

        bool onSnake = false;                   // To check if fruit spawned on the snake
        for (const auto& segment : snake) {
            if (segment == fruit) {
                onSnake = true;
                break;
            }
        }
        if (!onSnake) break;              // Keep changing fruit location till it isnt on the snake
    }
}
bool checkSelfCollision() {
    const Vec2& head = snake[0];
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i] == head)
            
            return true;
    }
    return false;
}
void update(){
    if(!alive) return;
    if (snake[0] == fruit) {
        // Add a new segment to the tail (copy last segment)
        snake.push_back(snake.back());
        spawnFruit();  // Respawn fruit
        playEatSound();
    }
    // Move the snake's body cells (tail->head)
    for(int i = snake.size()-1;i>0;--i){
        snake[i] = snake[i-1]; //Each segment takes position of segment before
    }
    //Update the head cell based on direction
    
    snake[0].x += dirX;
    snake[0].y += dirY;
    
    // Wrap the snake around the screen if it goes out of bounds
    if(snake[0].x >= cols) snake[0].x =0;    // Wrap right edge -> left edge
    if(snake[0].x < 0) snake[0].x =cols-1;   // Wrap left edge -> right edge
    if(snake[0].y >= rows) snake[0].y =0;    // Wrap bottom edge -> top edge
    if(snake[0].y < 0) snake[0].y =rows-1;   // Wrap top ledge -> bottom edge
    
    if (checkSelfCollision()) {
        std::cout << "Game Over! You collided with yourself\n"<<std::endl;
        alive = false;
        playGameOverSound();
    }
    
}
void drawEye(int x, int y) {
    glColor3f(0.2f,0.4f,0.2f);
    glBegin(GL_QUADS);
        glVertex2i(x, y);                      // Top-left corner of the eye
        glVertex2i(x + EYE_SIZE, y);           // Top-right corner
        glVertex2i(x + EYE_SIZE, y + EYE_SIZE);// Bottom-right corner
        glVertex2i(x, y + EYE_SIZE);           // Bottom-left corner
    glEnd();
}

void drawEyesInCorners(int x, int y, int size) {
    Vec2 EyeL,EyeR;

    if(dirY == 0){ // If moving horizontally
        EyeL.x = dirX>0?size-(EYE_SIZE+1):1; EyeL.y = dirX>0?1:size-(EYE_SIZE+1);
        EyeR.x = dirX>0?size-(EYE_SIZE+1):1; EyeR.y = dirX<0?1:size-(EYE_SIZE+1);
    }
    if(dirX == 0){ // If moving vertically
        EyeL.x = dirY<0?1:size-(EYE_SIZE+1); EyeL.y = dirY<0?1:size-(EYE_SIZE+1);
        EyeR.x = dirY<0?size-(EYE_SIZE+1):1; EyeR.y = dirY<0?1:size-(EYE_SIZE+1);
    }

    drawEye(x+EyeL.x,y+EyeL.y);
    drawEye(x+EyeR.x,y+EyeR.y);
}
void togglePause(){
    pause = !pause;
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        togglePause();
    }
}
int main(){
    if(!glfwInit()){
        return -1;
    }
    srand(static_cast<unsigned int>(time(0))); //Seed random number generator

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
    
    printf("Window size: %d x %d\n", windowWidth, windowHeight);
    printf("Framebuffer size: %d x %d\n", fbWidth, fbHeight);
    printf("Rows : %d, Columns : %d\n", rows,cols);
    spawnFruit();                         // Place the first fruit
    glfwMakeContextCurrent(window);       // Make the OpenGL context of the created window current
    glewInit();                           // Initialize GLEW (to allow access to modern OpenGL functions)

    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);  // Set orthogonal 2D projection for OpenGL rendering
    
    lastTime = glfwGetTime();
    initAudio();  // Initialize audio system
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT); // Clear Screen
        
        for(int i=0;i<snake.size();i++){
            drawCell(snake[i].x, snake[i].y, alive?0.0f:((i%2==0)?0.8f:1.0f), alive?((i%2==0)?1.0f:0.8f):0.0f, 0.0f); //Draw the snake
        }
        drawEyesInCorners(snake[0].x*CELL_SIZE,snake[0].y*CELL_SIZE, 20);
        
        drawCell(fruit.x, fruit.y, 1.0f, 1.0f, 0.0f);
        // Input handling :
        glfwSetKeyCallback(window, keyCallback);
        if(glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS && dirY!=1 && !pause){ // UP KEY
            dirX = 0; dirY = -1; // Move Up (-y direction)
        }
        else if(glfwGetKey(window, GLFW_KEY_DOWN)==GLFW_PRESS && dirY!=-1 && !pause){ // DOWN KEY
            dirX = 0; dirY = 1; // Move Down (+y direction)
        }
        else if(glfwGetKey(window, GLFW_KEY_LEFT)==GLFW_PRESS && dirX!=1 && !pause){ // LEFT KEY
            dirX = -1; dirY = 0; // Move Left (-x direction)
        }
        else if(glfwGetKey(window, GLFW_KEY_RIGHT)==GLFW_PRESS && dirX!=-1 && !pause){ // RIGHT KEY
            dirX = 1; dirY = 0; // Move Right (+x direction)
        }
        
        double currentTime = glfwGetTime();
        
        if (currentTime - lastTime >= delay && alive && !pause) {
            update();
            lastTime = currentTime;
        }
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    closeAudio();
    glfwTerminate();
    return 0;

}
