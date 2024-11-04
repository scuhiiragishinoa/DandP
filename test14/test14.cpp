#include <graphics.h>  
#include <conio.h>
#include <time.h>
#include "EasyXPng.h"
#include "Timer.h"
#include <vector>
#include <queue>
using namespace std;

#define  WIDTH 800  
#define  HEIGHT 600

enum PlayerStatus // Enumeration for all possible player states
{
    STAND_LEFT, STAND_RIGHT, RUN_LEFT, RUN_RIGHT, JUMP_LEFT, JUMP_RIGHT, DIE
};

class Ground  // Ground class
{
public:
    IMAGE img_ground;  // Ground image
    float left_x, right_x, top_y; // Coordinates for the ground
    float ground_width, ground_height; // Width and height of the ground image

    void initialize() // Initialization method
    {
        loadimage(&img_ground, _T("resource\\man\\land.png")); // Load ground image
        ground_width = img_ground.getwidth(); // Get ground image dimensions
        ground_height = img_ground.getheight();
        left_x = WIDTH / 2;  // Initial position in the center of the screen
        right_x = left_x + ground_width;
        top_y = HEIGHT / 2;
    }

    void draw() // Method to draw the ground
    {
        putimage(left_x, top_y, &img_ground);  // Draw the ground
    }
};

class GameScene // Game scene class
{
public:
    vector<IMAGE> imgs_background; // Background images vector
    vector<Ground> grounds; // Collection of ground objects
    int animID; // Animation ID for scrolling images
    int dx; // Background offset
    int groundSpeed; // Speed of the background movement

    void moveBackground() {
        dx -= 2; // Move background
        if (dx < -1200) {
            dx = 0;
            animID++;
        }
        if (animID > 8) animID = 0;

        // Randomly generate ground
        if (grounds[0].right_x <= 0) {
            // Shift grounds to the left
            for (int i = 0; i < grounds.size() - 1; i++) {
                grounds[i] = grounds[i + 1];
            }
            Ground newGround;
            newGround.initialize();
            newGround.left_x = 9 * newGround.ground_width;
            newGround.right_x = newGround.left_x + newGround.ground_width;
            newGround.top_y = HEIGHT / 2 + rand() % 2 * HEIGHT / 10; // Random height
            grounds[grounds.size() - 1] = newGround; // Add new ground
        }
    }

    void draw() // Method to draw the scene
    {
        moveBackground(); // Update background position
        putimage(dx, -100, &imgs_background[animID]); // Draw background

        for (int i = 0; i < grounds.size(); i++)
        {
            grounds[i].draw();  // Draw all ground pieces
        }
    }

    void initialize() // Initialization method for the scene
    {
        imgs_background.clear(); // Clear previous images
        TCHAR filename[80];
        for (int i = 1; i <= 9; i++) // Load background images
        {
            _stprintf_s(filename, _T("resource\\man\\landscape%d.png"), i);
            IMAGE img;
            loadimage(&img, filename);
            imgs_background.push_back(img);
        }
        grounds.clear(); // Clear the ground vector
        for (int i = 0; i < 10; i++) // Generate initial grounds
        {
            Ground ground;
            ground.initialize();
            ground.left_x = i * ground.ground_width;
            ground.right_x = ground.left_x + ground.ground_width;
            ground.top_y = HEIGHT / 2 + rand() % 2 * HEIGHT / 10; // Random height
            grounds.push_back(ground); // Add ground to vector
        }
        animID = 1;
        dx = 0;
        groundSpeed = 10; // Initial ground movement speed
    }
};

class Bat1 {
public:
    IMAGE img_bat; // Bat image
    float x; // Bat position X
    float y; // Bat position Y
    bool exists; // Check if bat is active

    void initialize() {
        loadimage(&img_bat, _T("resource\\man\\bat.png")); // Load bat image
        x = WIDTH; // Initial X position
        y = HEIGHT / 4; // Initial Y position
        exists = false; // Bat exists flag
    }

    void spawnBat(int playerHeight) { // Spawn bat according to player height
        if (!exists && rand() % 100 < 20) {
            exists = true; // Set existence flag
            switch (rand() % 3)
            {
            case 0: y = playerHeight + 10; break;
            case 1: y = playerHeight - 40; break;
            case 2: y = playerHeight - 170; break;
            default: y = HEIGHT / 2; break; // Default height
            }
        }
    }

    void draw() {
        if (exists) putimagePng(x, y, &img_bat); // Draw bat if exists
    }

    void move(int speed) {
        if (x < 0) {
            initialize(); // Reinitialize bat if it moves off-screen
        }
        if (exists) {
            x -= speed; // Move bat
        }
    }
};

class Bat2 {
public:
    vector<IMAGE> imgs_bat; // Bat animation frames
    float x; // Bat position X
    float y; // Bat position Y
    bool exists; // Check if bat is active
    float width; // Bat width
    float height; // Bat height
    float speed = 15; // Bat moving speed

    int index;  // Animation index
    void initialize() {
        imgs_bat.clear(); // Clear bat images
        TCHAR filename[80];
        for (int i = 0; i <= 4; i++) // Load bat animation frames
        {
            _stprintf_s(filename, _T("resource\\man\\bat%d.png"), i);
            IMAGE img;
            loadimage(&img, filename);
            imgs_bat.push_back(img); // Add each frame to the vector
        }
        width = 50; // Set bat dimensions
        height = 50;
        x = WIDTH; // Initial X position
        y = HEIGHT / 2 - height; // Initial Y position
        exists = false; // Set existence flag
        index = 0; // Reset animation index
    }

    void spawnBat(int playerHeight) { // Spawn bat with a probability
        if (!exists && rand() % 100 > 80) {
            exists = true; // Set existence flag
            switch (rand() % 3)
            {
            case 0: y = playerHeight + 10; break;
            case 1: y = playerHeight - height; break;
            case 2: y = playerHeight - 170; break;
            default: y = HEIGHT / 2; break; // Default height
            }
        }
    }

    void move() {
        if (x < 0) initialize(); // Reinitialize if off-screen
        x -= speed; // Move bat
    }

    void draw() {
        if (!exists) return; // If bat does not exist, return

        move(); // Move bat
        putimagePng(x, y, &imgs_bat[index]); // Draw bat image
        index++; // Increment animation index
        if (index > 4) index = 0; // Reset index after last frame
    }
};

class Star {
public:
    IMAGE img_star; // Star image
    float x; // Star position X
    float y; // Star position Y
    bool exists; // Check if star is active

    void initialize() {
        loadimage(&img_star, _T("resource\\man\\star.png")); // Load star image
        x = WIDTH; // Initial X position
        y = HEIGHT / 4; // Initial Y position
        exists = false; // Star exists flag
    }

    void spawnStar(int groundHeight) { // Spawn star with a probability
        if (!exists && rand() % 100 >= 30 && rand() % 100 <= 45) {
            exists = true; // Set flag to true
            int h = img_star.getheight(); // Get star height
            y = (rand() % 2 == 1) ? groundHeight - h - 10 : groundHeight - 170; // Randomize Y position
        }
    }

    void draw() {
        if (!exists) return; // If star does not exist, return
        putimagePng(x, y, &img_star); // Draw star image
    }

    void move(int speed) {
        if (x < 0) {
            initialize(); // Reinitialize if off-screen
        }
        if (exists) {
            x -= speed; // Move star
        }
    }
};

class Player  // Player-controlled character class
{
public:
    IMAGE img_current;  // Current character image
    IMAGE img_stand_right; // Image for standing right
    IMAGE img_stand_left; // Image for standing left
    IMAGE img_jump_right; // Image for jumping right
    IMAGE img_jump_left; // Image for jumping left
    vector<IMAGE> imgs_run_right; // Images for running right
    vector<IMAGE> imgs_run_left; // Images for running left
    int animIndex;  // Animation index for cycling through frames
    PlayerStatus status; // Current player status
    float x_left, y_bottom; // Player coordinates
    float vx, vy; // Speed in X and Y directions
    float gravity; // Gravity acceleration
    float width, height; // Image dimensions

    void draw() // Draw the player character
    {
        putimagePng(x_left, y_bottom - height, &img_current);  // Render the player on screen
    }

    void initialize() // Player initialization
    {
        imgs_run_left.clear(); // Clear left run images
        imgs_run_right.clear(); // Clear right run images
        loadimage(&img_stand_right, _T("resource\\man\\standright.png")); // Load standing right image
        loadimage(&img_stand_left, _T("resource\\man\\standleft.png")); // Load standing left image
        loadimage(&img_jump_right, _T("resource\\man\\jumpright.png")); // Load jumping right image
        loadimage(&img_jump_left, _T("resource\\man\\jumpleft.png")); // Load jumping left image

        status = STAND_RIGHT; // Initial status
        img_current = img_stand_right;  // Set current image
        width = img_stand_right.getwidth(); // Get the width of the standing image
        height = img_stand_right.getheight(); // Get the height

        TCHAR filename[80];
        for (int i = 0; i <= 7; i++) // Load running right images
        {
            _stprintf_s(filename, _T("resource\\man\\runright%d.png"), i);
            IMAGE img;
            loadimage(&img, filename);
            imgs_run_right.push_back(img); // Add to the run images vector
        }
        for (int i = 0; i <= 7; i++) // Load running left images
        {
            _stprintf_s(filename, _T("resource\\man\\runleft%d.png"), i);
            IMAGE img;
            loadimage(&img, filename);
            imgs_run_left.push_back(img);
        }

        animIndex = 0; // Reset animation index
        updatePosition(WIDTH / 2, HEIGHT / 2); // Start in the center of the screen
        vx = 2; // Initial horizontal speed
        vy = 0;  // Initial vertical speed
        gravity = 3;  // Set gravity acceleration
    }

    void updatePosition(float mx, float my) // Update player coordinates based on input
    {
        x_left = mx; // Update X based on input
        y_bottom = my; // Update Y based on input
    }

    void runRight(GameScene& scene) // Method for running right
    {
        for (int i = 0; i < scene.grounds.size(); i++) {
            scene.grounds[i].left_x -= scene.groundSpeed; // Move the ground
            scene.grounds[i].right_x -= scene.groundSpeed;
        }

        if (x_left < WIDTH / 2) x_left += vx; // Move character right
        else x_left += 0; // Stay in position if beyond midpoint

        if (isNotOnAllGrounds(scene.grounds, vy)) // Check if the character is in the air
        {
            img_current = img_jump_right; // Change image to jumping right
            status = JUMP_RIGHT; // Update status to jumping right
            return;
        }

        if (status == JUMP_LEFT || status == JUMP_RIGHT) // If jumping
        {
            img_current = img_jump_right; // Display jumping right image
        }
        else
        {
            if (status != RUN_RIGHT) // If not previously running right
            {
                status = RUN_RIGHT; // Change status to running right
                animIndex = 0; // Reset animation index
            }
            else // If already running right
            {
                animIndex++; // Increment animation index
                if (animIndex >= imgs_run_right.size()) // Loop through frames
                    animIndex = 0;
            }
            img_current = imgs_run_right[animIndex]; // Set the current image
        }
    }

    void runLeft(GameScene& scene) // Method for running left
    {
        x_left -= 10; // Move character left
        if (isNotOnAllGrounds(scene.grounds, vy)) // Check if the character is in the air
        {
            img_current = img_jump_left; // Change image to jumping left
            status = JUMP_LEFT; // Update status to jumping left
            return;
        }

        if (status == JUMP_LEFT || status == JUMP_RIGHT) // If jumping
        {
            img_current = img_jump_left; // Display jumping left image
        }
        else
        {
            if (status != RUN_LEFT) // If not previously running left
            {
                status = RUN_LEFT; // Change status to running left
                animIndex = 0; // Reset animation index
            }
            else // If already running left
            {
                animIndex++; // Increment animation index
                if (animIndex >= imgs_run_left.size()) // Loop through frames
                    animIndex = 0;
            }
            img_current = imgs_run_left[animIndex]; // Set the current image
        }
    }

    void standStill() // Method to handle standing still
    {
        if (status == RUN_LEFT || status == STAND_LEFT)
        {
            img_current = img_stand_left; // Set image to standing left
        }
        else if (status == RUN_RIGHT || status == STAND_RIGHT)
        {
            img_current = img_stand_right; // Set image to standing right
        }
    }

    void beginJump() // Method to initiate jump
    {
        if (status != JUMP_LEFT && status != JUMP_RIGHT) // Prevent jumping if already in air
        {
            if (status == RUN_LEFT || status == STAND_LEFT) // If standing or running left
            {
                img_current = img_jump_left; // Change image to jumping left
                status = JUMP_LEFT; // Update status to jumping left
            }
            else if (status == RUN_RIGHT || status == STAND_RIGHT) // If standing or running right
            {
                img_current = img_jump_right; // Change image to jumping right
                status = JUMP_RIGHT; // Update status to jumping right
            }
            vy = -30; // Set initial upward velocity
        }
    }

    int isOnGround(Ground& ground, float ySpeed) // Check if the character is on the ground
    {
        float x_right = x_left + width; // Calculate right X position
        if (ySpeed <= 0) ySpeed = 0; // Ignore speed if moving upwards

        if (ground.left_x - x_left <= width * 0.6 && x_right - ground.right_x <= width * 0.6 && abs(y_bottom - ground.top_y) <= 5 + ySpeed)
            return 1; // On the ground
        else
            return 0; // Not on ground
    }

    int isNotOnAllGrounds(vector<Ground>& grounds, float speed) // Check if the player is not on any ground
    {
        for (int i = 0; i < grounds.size(); i++)
        {
            if (isOnGround(grounds[i], speed)) // If on any ground
                return 0; // Return false
        }
        return 1; // Not on any ground, return true
    }

    void updateYCoordinate(GameScene& scene) // Update Y coordinate
    {
        if (isNotOnAllGrounds(scene.grounds, vy) || status == JUMP_LEFT || status == JUMP_RIGHT) // If in air
        {
            vy += gravity; // Update Y speed with gravity
            y_bottom += vy;  // Update Y position

            for (int i = 0; i < scene.grounds.size(); i++) // Check all grounds
            {
                if (isOnGround(scene.grounds[i], vy)) // If on a ground
                {
                    y_bottom = scene.grounds[i].top_y; // Align to ground
                    if (status == JUMP_LEFT) status = STAND_LEFT; // Switch to standing left
                    if (status == JUMP_RIGHT) status = STAND_RIGHT; // Switch to standing right
                    break; // Exit loop after landing
                }
            }
        }
    }
};

// Global variables
Player player;  // Player object
GameScene scene;  // Game scene object
Bat1 bat1; // Bat object 1
Bat2 bat2; // Bat object 2
Star star; // Star object
Timer timer;  // Timer for precise delays

int score = 0; // Current score
int life = 6; // Player life count
IMAGE img_life; // Image for life display

int isHit(int x, int y, int w, int h) { // Check for collisions
    if (x >= player.x_left + 20 - w && x <= player.x_left + player.width - 20 &&
        y >= player.y_bottom - h - player.height && y <= player.y_bottom) {
        return 1; // Collision detected
    }
    return 0; // No collision
}

void showLife() { // Method to display life count
    loadimage(&img_life, _T("resource\\man\\heart.png")); // Load life image
    for (int i = 0; i < life; i++) {
        putimagePng(i * 35, 30, &img_life); // Draw life images
    }
}

void showScore() { // Method to display score
    settextstyle(30, 0, _T("微软雅黑")); // Set text style
    settextcolor(RED); // Set text color
    setbkmode(TRANSPARENT);   // Set transparent background
    outtextxy(10, 80, "Score:"); // Display score label
    char str[20];
    sprintf_s(str, "%d", score); // Convert score to string
    outtextxy(90, 80, str); // Display score
}

void startup() // Method for the initial setup
{
    srand(time(0)); // Seed random number generator
    scene.initialize();  // Initialize the game scene
    player.initialize(); // Initialize player character
    bat1.initialize(); // Initialize bat 1
    star.initialize(); // Initialize star
    bat2.initialize(); // Initialize bat 2

    initgraph(WIDTH, HEIGHT); // Create a new graphics window
    BeginBatchDraw(); // Start batch drawing mode
}

void show() // Method to render everything on the screen
{
    scene.draw();   // Draw the scene
    player.draw();  // Draw the player character
    bat1.draw();    // Draw bat 1
    star.draw();    // Draw star
    bat2.draw();    // Draw bat 2

    showLife();     // Show life count
    showScore();    // Show score

    FlushBatchDraw(); // Flush the drawing buffer
    timer.Sleep(50); // Pause for a short interval
}

void updateWithoutInput() // Update logic independent of user input
{
    player.updateYCoordinate(scene); // Update player Y coordinate

    if (scene.grounds[0].right_x <= 10) {
        int groundHeight = int(scene.grounds[scene.grounds.size() - 1].top_y); // Get height of last ground
        bat1.spawnBat(player.y_bottom); // Spawn bat 1
        star.spawnStar(groundHeight); // Spawn star
        bat2.spawnBat(player.y_bottom); // Spawn bat 2
    }

    // Check collisions with bats
    if (isHit(bat1.x, bat1.y, bat1.img_bat.getwidth() - 20, bat1.img_bat.getheight())) {
        bat1.initialize(); // Reinitialize bat 1
        life--; // Decrease player life
    }

    if (isHit(bat2.x, bat2.y, bat2.width, bat2.height)) {
        bat2.initialize(); // Reinitialize bat 2
        life--; // Decrease player life
    }

    // Check collision with star
    if (isHit(star.x, star.y, star.img_star.getwidth(), star.img_star.getheight())) {
        printf("Hit Star\n"); // Log collision
        star.initialize(); // Reinitialize star
        score++; // Increment score
    }
}

void updateWithInput() // Update logic that depends on user input
{
    player.standStill(); // Default to standing still

    if (_kbhit()) // Check if a key has been pressed
    {
        if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) // Move right
        {
            player.runRight(scene); // Run the player character right
            bat1.move(scene.groundSpeed); // Move bat 1
            star.move(scene.groundSpeed); // Move star
        }
        else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) // Move left
        {
            player.runLeft(scene); // Run the player character left
        }
        if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) // Jump
        {
            player.beginJump(); // Initiate jump
        }
    }
}

void gameLost() { // Handle game over state
    settextstyle(50, 0, _T("微软雅黑")); // Set font style and size
    settextcolor(BLACK); // Set text color
    setbkmode(TRANSPARENT); // Set transparent background for text
    outtextxy(WIDTH / 2 - textwidth("Game Over") / 2, 250, "Game Over!"); // Center message
    settextstyle(30, 0, _T("微软雅黑")); // Change font size
    outtextxy(WIDTH / 2 - textwidth("Press Space to Exit") / 2, 350, "Press Space to Exit"); // Prompt for exit
    while (_getch() != ' ') {}; // Wait for space key to exit
}

void gameNext() { // Handle transition to the next level
    settextstyle(50, 0, _T("微软雅黑")); // Set font style and size
    settextcolor(BLACK); // Set text color
    setbkmode(TRANSPARENT); // Set transparent background for text
    outtextxy(WIDTH / 2 - textwidth("Level Complete") / 2, 250, "Level Complete"); // Display level complete message
    settextstyle(30, 0, _T("微软雅黑")); // Change font size
    outtextxy(WIDTH / 2 - textwidth("Press Space to Start Next Level") / 2, 350, "Press Space to Start Next Level"); // Prompt for next level
    while (_getch() != ' ') {}; // Wait for space key to continue
}

int main() // Main function
{
    startup();   // Initial setup
    while (1) // Game loop
    {
        show(); // Render graphics
        updateWithoutInput(); // Update logic without user input
        updateWithInput(); // Update logic that reacts to user input

        EndBatchDraw(); // End the batch drawing
        if (life <= 0 || player.y_bottom >= HEIGHT) { // Check for game over conditions
            gameLost(); // Handle game loss
            break; // Exit the loop
        }

        if (score >= 10 && scene.groundSpeed == 10) { // Check for level advancement
            gameNext(); // Advance to next level
            life += 3; // Increase life count
            scene.groundSpeed = 15; // Increase ground speed
            bat2.speed = 20; // Increase bat 2 speed
        }
    }
    closegraph(); // Close the graphics window
    return 0; // End of the program
}