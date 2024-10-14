#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <iostream>
#include <time.h>
#include <list>

using namespace std;
#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996);

// Screen size
#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 600

// Maximum number of enemy planes and bullets
#define MAX_ENEMY_PLANES  50
#define MAX_BULLETS       100

// Bullet structure
class BulletNode {
public:
    int x; // x-coordinate
    int y; // y-coordinate
};

// Represents the player plane and its properties
BulletNode playerPlane = { 200, 590 };
int playerSpeed = 8; // Movement speed

// Store enemy planes and bullets using arrays
BulletNode enemyPlanes[MAX_ENEMY_PLANES];
BulletNode bullets[MAX_BULLETS];
bool enemyActive[MAX_ENEMY_PLANES] = { false }; // Enemy plane active state
bool bulletActive[MAX_BULLETS] = { false }; // Bullet active state

// Enemy bullets
BulletNode enemyBullets[MAX_BULLETS];
bool enemyBulletActive[MAX_BULLETS] = { false };
time_t bulletFireGap = time(NULL); // Time interval for bullet firing

// Player health
int playerHealthWidth = 60;

// Weapons represented as a structure
struct Weapon {
    int x; // x-coordinate
    int y; // y-coordinate
    int type; // 0 means nonexistent, 1, 2, 3 denotes weapon types
};
Weapon currentWeapon = { 0, 0, 0 };
bool hasNewWeapon = false;
time_t weaponStartTime; // Timer for weapon usage

// Boss properties
struct Boss {
    int x;
    int y;
    int healthWidth;
};
Boss gameBoss = { 150, 100, 100 };
int bossSpawnTime = 100;   // Boss appears after 100 seconds
time_t bossStartTime = time(NULL);
time_t bossBulletGap;
BulletNode bossBullets[MAX_BULLETS];
bool bossBulletActive[MAX_BULLETS];

// Detect keyboard input
void detectKeyPress() {
    if (_kbhit()) { // Check if any key is pressed
        char key = getch(); // Capture keyboard input

        switch (key) {
        case 72: // Up key
            playerPlane.y -= playerSpeed;
            break;
        case 80: // Down key
            playerPlane.y += playerSpeed;
        break;
        case 75: // Left key
            playerPlane.x -= playerSpeed;
        break;
        case 77: // Right key
            playerPlane.x += playerSpeed;
            break;
        default:
            break;
        }
    }

    // Ensure the player plane stays within screen bounds
    if (playerPlane.x < 0) playerPlane.x = 0;
    if (playerPlane.x >= WINDOW_WIDTH) playerPlane.x = WINDOW_WIDTH - 32;
    if (playerPlane.y >= WINDOW_HEIGHT) playerPlane.y = WINDOW_HEIGHT - 18;
    if (playerPlane.y < 0) playerPlane.y = 0;
}

// Check if bullets and enemy planes are off-screen
void checkBulletAndEnemyOutOfBounds() {
    // Deactivate enemy planes that move off the screen
    for (int i = 0; i < MAX_ENEMY_PLANES; ++i) {
        if (enemyActive[i] && enemyPlanes[i].y >= WINDOW_HEIGHT) {
            enemyActive[i] = false;
        }
    }

    // Deactivate bullets that move off the screen
    for (int j = 0; j < MAX_BULLETS; ++j) {
        if (bulletActive[j] && bullets[j].y <= -10) {
            bulletActive[j] = false;
        }
    }
}

// Create new enemy planes
void addEnemyPlane() {
    for (int i = 0; i < MAX_ENEMY_PLANES; ++i) {
        if (!enemyActive[i]) { // Find inactive enemy planes
            enemyPlanes[i] = { rand() % (WINDOW_WIDTH - 50), 0 };
            enemyActive[i] = true;
            break;
        }
    }
}

// Shoot new bullets from the player plane
void addBullet() {
    if (hasNewWeapon && currentWeapon.type == 2) // Prevent shooting if using laser weapon
        return;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bulletActive[i]) { // Find inactive bullets
            bullets[i] = { playerPlane.x + 16, playerPlane.y - 20 };
            bulletActive[i] = true;
            break;
        }
    }
}

// Enemy planes shooting bullets at the player
void addEnemyBullets() {
    int j = 0;
    // Automatically shoot bullets from every enemy plane every 2 seconds
    if (time(NULL) - 2 >= bulletFireGap) {
        bulletFireGap = time(NULL);
        for (int i = 0; i < MAX_ENEMY_PLANES; i++) {
            if (enemyActive[i]) {
                while (enemyBulletActive[j]) {
                    j++;
                    if (j >= MAX_BULLETS) j = 0; // Reset to start if over limit
                }
                enemyBulletActive[j] = true;
                enemyBullets[j] = { enemyPlanes[i].x + 25, enemyPlanes[i].y + 30 };
            }
        }
    }
}

// Draw enemy bullets and update their position
void drawEnemyBullets() {
    for (int j = 0; j < MAX_BULLETS; j++) {
        if (enemyBulletActive[j]) {
            setfillcolor(BLUE);
            fillcircle(enemyBullets[j].x, enemyBullets[j].y, 4);
            enemyBullets[j].y += 2; // Move bullets down
            if (enemyBullets[j].y > WINDOW_HEIGHT) {
                enemyBulletActive[j] = false; // Deactivate if off screen
            }
        }
    }
}

// Check if enemy bullets hit the player plane
void checkEnemyBulletHit() {
    for (int j = 0; j < MAX_BULLETS; j++) {
        if (!enemyBulletActive[j]) continue;

        // Collision detection
        if (enemyBullets[j].x >= (playerPlane.x - 5) &&
            enemyBullets[j].x <= (playerPlane.x + 55) &&
            enemyBullets[j].y >= (playerPlane.y - 5) &&
            enemyBullets[j].y <= (playerPlane.y + 85)) {
            enemyBulletActive[j] = false; // Bullet disappears
            playerHealthWidth -= 5; // Decrease player health
            break;
        }
    }
}

// Display player health on the screen
void showPlayerHealth() {
    settextstyle(30, 0, "黑体");
    settextcolor(RED);
    outtextxy(WINDOW_WIDTH - textwidth("LIFE:") - 80, 10, "LIFE:");

    setfillcolor(RED);
    fillrectangle(WINDOW_WIDTH - textwidth("LIFE:"), 10,
        WINDOW_WIDTH - textwidth("LIFE:") + playerHealthWidth, 30);
    rectangle(WINDOW_WIDTH - textwidth("LIFE:"), 10,
        WINDOW_WIDTH - textwidth("LIFE:") + 60, 30);
}

// Randomly generate weapons based on a chance
void generateWeapons(int x, int y) {
    if (currentWeapon.type > 0) return; // No additional weapon if one exists

    int randomChance = rand() % 100;
    if (randomChance <= 5) { // 1. Speed weapon (yellow block)
        currentWeapon.x = x;
        currentWeapon.y = y;
        currentWeapon.type = 1;
    }
    else if (randomChance > 5 && randomChance <= 10) { // 2. Laser weapon (light blue)
        currentWeapon.x = x;
        currentWeapon.y = y;
        currentWeapon.type = 2;
    }
    else if (randomChance > 10 && randomChance <= 15) { // 3. Shotgun weapon (red)
        currentWeapon.x = x;
        currentWeapon.y = y;
        currentWeapon.type = 3;
    }
}

// Collision detection for shooting enemy planes
void shootAtEnemy() {
    for (int i = 0; i < MAX_ENEMY_PLANES; ++i) {
        if (!enemyActive[i]) continue; // Skip inactive enemy planes
        for (int j = 0; j < MAX_BULLETS; ++j) {
            if (!bulletActive[j]) continue; // Skip inactive bullets
            // Collision detection
            if (bullets[j].x >= (enemyPlanes[i].x - 10) &&
                bullets[j].x <= (enemyPlanes[i].x + 50) &&
                bullets[j].y >= (enemyPlanes[i].y - 15) &&
                bullets[j].y <= (enemyPlanes[i].y + 30)) {
                enemyActive[i] = false; // Enemy plane disappears
                bulletActive[j] = false; // Bullet disappears
                generateWeapons(bullets[j].x, bullets[j].y); // Possibly drop a weapon
                break;
            }
        }
    }
}

// Draw the weapon when dropped
void showDroppedWeapon() {
    if (!hasNewWeapon && currentWeapon.type > 0) {
        if (currentWeapon.type == 1) {
            setfillcolor(GREEN);
        }
        else if (currentWeapon.type == 2) {
            setfillcolor(BLUE);
        }
        else if (currentWeapon.type == 3) {
            setfillcolor(RED);
        }
        fillrectangle(currentWeapon.x, currentWeapon.y,currentWeapon.x + 15, currentWeapon.y + 15);
        currentWeapon.y += 1; // Move the weapon down

        // Remove the weapon if it falls off the screen
        if (currentWeapon.y >= WINDOW_HEIGHT) {
            currentWeapon.type = 0; // Mark as nonexistent
        }
    }
}

// Handle shotgun bullet collisions with enemy planes
void handleShotgunHit(int x, int y) {
    for (int i = 0; i < MAX_ENEMY_PLANES; i++) { // Normal enemy planes
        if (enemyActive[i] &&
            x >= (enemyPlanes[i].x - 10) &&
            x <= (enemyPlanes[i].x + 50) &&
            y >= (enemyPlanes[i].y - 15) &&
            y <= (enemyPlanes[i].y + 30)) {
            enemyActive[i] = false; // Enemy plane destroyed
        }
    }

    // Check if the boss is present and hit
    if (time(NULL) >= bossSpawnTime + bossStartTime) {
        if (x >= (gameBoss.x - 10) &&
            x <= (gameBoss.x + 85) &&
            y >= (gameBoss.y - 5) &&
            y <= (gameBoss.y + 35))
            gameBoss.healthWidth -= 5; // Damage the boss
    }
}

// Handle laser weapon hits
void handleLaserHit(int x1, int y1, int x2, int y2) {
    for (int i = 0; i < MAX_ENEMY_PLANES; i++) {
        if (enemyActive[i] &&
            enemyPlanes[i].x >= x1 - 30 && enemyPlanes[i].x <= x2 &&
            enemyPlanes[i].y >= y1 && enemyPlanes[i].y <= y2) {
            enemyActive[i] = false; // Destroy enemy plane
        }
    }
    // Laser cannot hit the boss
}

// Effect of picking up a new weapon
void checkForNewWeaponPickup() {
    // Collision detection for weapon pickup
    if (!hasNewWeapon && currentWeapon.type > 0 &&
        currentWeapon.x >= playerPlane.x - 15 && currentWeapon.x <= playerPlane.x + 32 + 15 && currentWeapon.y >= playerPlane.y - 15 && currentWeapon.y <= playerPlane.y + 18 + 15) {
        hasNewWeapon = true;
        weaponStartTime = time(NULL);
    }

    // Weapon goes off-screen
    if (currentWeapon.y >= WINDOW_HEIGHT) {
        currentWeapon.type = 0; // Mark as nonexistent
    }

    // Apply weapon effects
    if (hasNewWeapon) {
        if (currentWeapon.type == 1 && playerHealthWidth <= 60) {  // Increase movement speed for 10 seconds
            playerHealthWidth += 5;
        }
        else if (currentWeapon.type == 2) {  // Laser weapon - stop normal bullets
            setfillcolor(RGB(0, 255, 255));
            fillrectangle(playerPlane.x + 11, 0, playerPlane.x + 21, playerPlane.y);
            handleLaserHit(playerPlane.x + 11, 0, playerPlane.x + 21, playerPlane.y);
            if (playerHealthWidth <= 60) {
                playerHealthWidth += 5;
            }
        }
        else if (currentWeapon.type == 3) {  // Shotgun for 10 seconds
            for (int i = 0; i < MAX_BULLETS; ++i) {
                if (bulletActive[i]) {
                    int spread = (playerPlane.y - bullets[i].y) / 2;
                    circle(bullets[i].x + spread, bullets[i].y, 5);
                    circle(bullets[i].x - spread, bullets[i].y, 5);
                    handleShotgunHit(bullets[i].x + spread, bullets[i].y);
                    handleShotgunHit(bullets[i].x - spread, bullets[i].y);
                }
            }
            if (playerHealthWidth <= 60) {
                playerHealthWidth += 5;
            }
        }

        // Reset effects after 8 seconds
        if (time(NULL) >= weaponStartTime + 8) {
            hasNewWeapon = false;
            currentWeapon.type = 0;
        }
    }
}

// Add bullets for the boss
void addBossBullets() {
    if (time(NULL) < bossSpawnTime + bossStartTime) return;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bossBulletActive[i]) {
            bossBullets[i] = { gameBoss.x + 40, gameBoss.y };
            bossBulletActive[i] = true;
            break;
        }
    }
}

// Boss logic including shooting and health management
void manageBossLogic() {
    if (time(NULL) < bossSpawnTime + bossStartTime + 2) return;

    // Manage bullets shot by the boss
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bossBulletActive[i]) {
            // Collision detection
            if (bossBullets[i].x >= (playerPlane.x - 5) &&
                bossBullets[i].x <= (playerPlane.x + 35) &&
                bossBullets[i].y >= (playerPlane.y - 5) &&
                bossBullets[i].y <= (playerPlane.y + 20)) {
                bossBulletActive[i] = false; // Bullet disappears
                playerHealthWidth -= 15; // Decrease player health
            }

            bossBullets[i].y += 2; // Move boss bullets down
            if (bossBullets[i].y >= WINDOW_HEIGHT) {
                bossBulletActive[i] = false; // Deactivate if off screen
            }
        }
    }

    // Check for player bullets hitting the boss
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) { // If the bullet is active
            int bulletX = bullets[i].x;
            int bulletY = bullets[i].y;

            if (bulletX >= (gameBoss.x - 10) &&
                bulletX <= (gameBoss.x + 105) &&
                bulletY >= (gameBoss.y - 5) &&
                bulletY <= (gameBoss.y + 35)) {
                gameBoss.healthWidth -= 5; // Damage the boss
                bulletActive[i] = 0; // Deactivate the bullet
            }
        }
    }
}

// Draw the boss on the screen
void drawGameBoss() {
    if (time(NULL) < bossSpawnTime + bossStartTime) return;

    // Draw the boss rectangle
    rectangle(gameBoss.x, gameBoss.y, gameBoss.x + 100, gameBoss.y + 35);

    // Draw boss health bar
    rectangle(gameBoss.x, gameBoss.y - 20, gameBoss.x + 100, gameBoss.y - 5);
    setfillcolor(RED);
    fillrectangle(gameBoss.x, gameBoss.y - 20, gameBoss.x + gameBoss.healthWidth, gameBoss.y - 5);

    // Draw boss bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bossBulletActive[i]) {
            setfillcolor(RED);
            fillcircle(bossBullets[i].x, bossBullets[i].y, 8);
        }
    }
}

// Game over conditions based on player health or boss defeat
void checkGameOver() {
    if (gameBoss.healthWidth <= 0) {
        settextcolor(RED);
        settextstyle(50, 0, _T("黑体"));
        outtextxy(200 - textwidth("GAME WIN!!!") / 2, 300, "GAME WIN!!!");
        while (_getch() != ' ') {};
    }

    if (playerHealthWidth <= 0) {
        settextcolor(RED);
        settextstyle(50, 0, _T("黑体"));
        outtextxy(200 - textwidth("GAME LOSE!!!") / 2, 300, "GAME LOSE!!!");
        while (_getch() != ' ') {};
    }
}

int main() {
    srand((unsigned int)time(NULL)); // Seed random number generator
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE);
    SetCursorPos(-100, -100); // Hide the mouse cursor

    DWORD enemySpawnTime, bulletSpawnTime; // Control timing for enemy and bullet generation
    enemySpawnTime = GetTickCount(); // Initial time for spawning enemies
    bulletSpawnTime = GetTickCount(); // Initial time for spawning bullets

    while (1) {
        // Add a new enemy plane every 1000 milliseconds
        DWORD currentTime = GetTickCount();
        if (currentTime - enemySpawnTime >= 1000) {
            addEnemyPlane();
            addBossBullets();
            enemySpawnTime = currentTime; // Reset enemy spawn timer
        }

        // Add a new bullet every 500 milliseconds
        DWORD bulletCurrentTime = GetTickCount();
        if (bulletCurrentTime - bulletSpawnTime >= 500) {
            addBullet();
            bulletSpawnTime = bulletCurrentTime; // Reset bullet spawn timer
        }

        BeginBatchDraw(); // Start batch drawing
        cleardevice(); // Clear the screen

        addEnemyBullets();
        drawEnemyBullets();
        checkEnemyBulletHit();
        showDroppedWeapon();
        showPlayerHealth();
        checkForNewWeaponPickup();

        manageBossLogic();
        drawGameBoss();

        // Draw player plane
        rectangle(playerPlane.x, playerPlane.y, playerPlane.x + 32, playerPlane.y + 18);

        // Show bullets
        if (!(hasNewWeapon && currentWeapon.type == 2)) { // Stop normal bullets if using the laser
            for (int i = 0; i < MAX_BULLETS; ++i) {
                if (bulletActive[i]) {
                    circle(bullets[i].x, bullets[i].y, 5);
                    bullets[i].y--; // Move bullet up
                }
            }
        }

        // Draw enemy planes
        for (int i = 0; i < MAX_ENEMY_PLANES; ++i) {
            if (enemyActive[i]) {
                roundrect(enemyPlanes[i].x, enemyPlanes[i].y,
                    enemyPlanes[i].x + 30, enemyPlanes[i].y + 20, 5, 5);
                enemyPlanes[i].y++; // Move enemy plane down
            }
        }

        Sleep(5); // Small delay for smoother animation
        shootAtEnemy(); // Check if player's bullets hit any enemies
        checkBulletAndEnemyOutOfBounds(); // Cleanup off-screen bullets/enemies
        EndBatchDraw(); // End batch drawing
        detectKeyPress(); // Check for player input
        checkGameOver(); // Check if game is over
    }

    getchar(); // Wait for user input before exiting
    return 0;
}