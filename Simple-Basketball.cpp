// Simple-Basketball.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "game.h"
#include <cstdlib>
#include <ctime>


void drawShotTracer(SDL_Renderer* renderer,float startX,float startY,float velocity,float angleDegrees,float pixelsPerMeter);


int main(){
   
    constexpr int scale = 1;
    constexpr int viewX = 0; // Show right half of the 640px court
    constexpr int viewW = 640;
    constexpr int viewH = 360;

    // Coordinates in the original full court image (not window coordinates).
    constexpr int rimEdgeX = 525; // Left edge of the right hoop's rim
    constexpr int rimY = 85;
    constexpr int floorY = 300;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << SDL_GetError() << '\n';
        return 1;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_Window* window = SDL_CreateWindow("Simple Basketball", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, viewW * scale, viewH * scale, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) : nullptr;
    SDL_Texture* court = renderer ? IMG_LoadTexture(renderer, "assets/street.png") : nullptr;
    SDL_Texture* player = renderer ? IMG_LoadTexture(renderer, "assets/idle.png") : nullptr;
    SDL_Texture* ball = renderer ? IMG_LoadTexture(renderer, "assets/ball.png") : nullptr;
   

    if (!renderer || !court|| !player|| !ball) {
        std::cerr << "Failed to load window, renderer, or textures: " << IMG_GetError() << '\n';
        
        SDL_DestroyTexture(court);
        SDL_DestroyTexture(player);
        SDL_DestroyTexture(ball);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect courtSource{ viewX, 0, viewW, viewH };
    SDL_Rect courtDest{ 0, 0, viewW * scale, viewH * scale };
    SDL_Rect playerSource{ 0,0,playerX,playerY };
    SDL_Rect playerDest{ PLAYER_FREE_THROW_X,PLAYER_Y,playerX* scale,playerY* scale };
    SDL_Rect ballSource{ 0,0,BALL_X,BALL_Y };
    SDL_Rect ballDest{ 0,0,BALL_Y * scale,BALL_Y * scale };
    

    

    bool running = true;
    float shotPower = 0.0f;
    bool charging = false;

    double theta = 65 * pi / 180.0;
    double y_0 = 2.0;
    double hoopY = feet2Meters(rimHeight);
    std::cout << "Hoop height: " << hoopY << "\n";
    double dy = hoopY - y_0;
    double X = inches2Meters(
        feet2Inches(freeThrowDistanceFeet) + freeThrowDistanceInches
    );
    std::cout << "X is " << X << "\n";
    double v0 = initialVelocity(X, theta, dy);
    bool realse = false;
    float releasedVelocity = 0.0f;
    float ballStartX = 0.0f;
    float ballStartY = 0.0f;
    float shotTime = 0.0f;

    Uint32 lastTick = SDL_GetTicks();
    SDL_Event event;
    while (running) {


        Uint32 currentTick = SDL_GetTicks();

        float deltaTime = (currentTick - lastTick) / 1000.0f;

        lastTick = currentTick;

        // Perfect shot range
        constexpr float perfectStart = 0.70f;
        constexpr float perfectEnd = 0.80f;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_SPACE &&
                event.key.repeat == 0) {
                charging = true;
                realse = false;
                shotPower = 0.0f;
            }

            if (event.type == SDL_KEYUP &&
                event.key.keysym.sym == SDLK_SPACE) {
                charging = false;
                realse = true;

                releasedVelocity = static_cast<float>(
                    v0 + shotPower * v0
                    );

                ballStartX = playerDest.x + playerDest.w / 2.0f;
                ballStartY = playerDest.y + 35.0f;

                shotTime = 0.0f;
                bool perfect =
                    shotPower >= perfectStart &&
                    shotPower <= perfectEnd;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, court, &courtSource, &courtDest);
        SDL_RenderCopy(renderer, player, &playerSource, &playerDest);
        

        if (charging) {
            shotPower += 0.01f;

            if (shotPower > 1.0f)
                shotPower = 1.0f;

            float velocity = v0 + shotPower * v0;

            drawShotTracer(
                renderer,
                playerDest.x + playerDest.w / 2.0f,
                playerDest.y + 35.0f,
                velocity,
                65.0f,
                70.54f
            );

            // Bar underneath the player
            SDL_Rect bar = {
                playerDest.x,
                playerDest.y + playerDest.h - 15,
                playerDest.w,
                8
            };

            // Background
            SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            SDL_RenderFillRect(renderer, &bar);

            // Filled portion
            SDL_Rect fill = bar;
            fill.w = static_cast<int>(bar.w * shotPower);

            SDL_SetRenderDrawColor(renderer, 255, 200, 50, 255);
            SDL_RenderFillRect(renderer, &fill);

            // Perfect release zone
            SDL_Rect perfect = {
                bar.x + static_cast<int>(bar.w * perfectStart),
                bar.y,
                static_cast<int>(bar.w * (perfectEnd - perfectStart)),
                bar.h
            };

            SDL_SetRenderDrawColor(renderer, 0, 220, 80, 255);
            SDL_RenderDrawRect(renderer, &perfect);
        }

        if (realse) {

            shotTime += deltaTime;

            float vx = releasedVelocity * std::cos(theta);
            float vy = releasedVelocity * std::sin(theta);

            float x = ballStartX +
                vx * shotTime * PIXELS_PER_METER;

            float y = ballStartY -
                (vy * shotTime - 0.5f * gravity * shotTime * shotTime)
                * PIXELS_PER_METER;

            ballDest.x = static_cast<int>(x - ballDest.w / 2.0f);
            ballDest.y = static_cast<int>(y - ballDest.h / 2.0f);

            SDL_RenderCopy(renderer, ball, &ballSource, &ballDest);

            // Stop once the ball completely leaves the screen
            if (ballDest.x > viewW ||
                ballDest.x + ballDest.w < 0 ||
                ballDest.y > viewH ||
                ballDest.y + ballDest.h < 0) {

                realse = false;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }


    SDL_DestroyTexture(court);
    SDL_DestroyTexture(player);
    SDL_DestroyTexture(ball);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    
    
   
   
    

    return 0;
}



#include <cmath>

void drawShotTracer(
    SDL_Renderer* renderer,
    float startX,
    float startY,
    float velocity,
    float angleDegrees,
    float pixelsPerMeter
) {
    constexpr float gravity = 9.81f;
    constexpr float pi = 3.14159265f;

    float angle = angleDegrees * pi / 180.0f;

    float vx = velocity * std::cos(angle);
    float vy = velocity * std::sin(angle);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    for (float t = 0.0f; t < 3.0f; t += 0.03f) {

        float x = startX + vx * t * pixelsPerMeter;

        float y = startY -
            (vy * t - 0.5f * gravity * t * t)
            * pixelsPerMeter;

        if (y > FLOOR_Y)
            break;

        SDL_Rect dot = {
            static_cast<int>(x),
            static_cast<int>(y),
            3,
            3
        };

        SDL_RenderFillRect(renderer, &dot);
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
