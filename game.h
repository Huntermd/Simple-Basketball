#pragma once
#include "myPhysics.h"
/*
Basketball related mesaurements are in feet
*/
const double rimHeight = 10;
const double freeThrowDistanceFeet = 13;
/*
Mesaurements in inchies
*/
const double rimDiamater = 18;
const double basketBallDiamater = 9.5;
const double freeThrowDistanceInches = 9;

constexpr float RIM_X = 525.0f;
constexpr float RIM_Y = 85.0f;
constexpr float FLOOR_Y = 360.0f;
constexpr float playerY = 140.0f;
constexpr float playerX = 80.0f;
constexpr float PLAYER_Y = FLOOR_Y - playerY;

constexpr float BALL_X = 16.0f;
constexpr float BALL_Y = 16.0f;


constexpr float PIXELS_PER_FOOT =
(FLOOR_Y - RIM_Y) / 10.0f;

constexpr float PIXELS_PER_METER =
PIXELS_PER_FOOT * 3.28084f;

constexpr float PIXELS_FREE_THROW_AWAY = 378.167f;
constexpr float PLAYER_FREE_THROW_X = (RIM_X - PIXELS_FREE_THROW_AWAY);
