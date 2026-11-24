#pragma once
#include "math.h"
const double gravity = 9.81;
const double pi = 3.14159265359;
struct Vector {
	double x, y;
};

double feet2Meters(double feet);
double inches2Meters(double inches);
double feet2Inches(double feet);
//Kinematic equations basics
double vFinal(double vinital, double accerlation, double time);
double vFinalSquared(double vInitialSquared, double accerlation, double displacement);
double displacementWithoutA(double vInitial, double vFinal, double time);
double displacementWithoutVfinal(double vInitial, double time, double accerlation);
//Projectile motion
double horizontalLauchSpeed(double vInitial, double angle);
double verticalLauchSpeed(double vInitial, double angle);
double horizontalPostion(double x, double vInitialx, double time);
double verticalPostion(double y, double vInitialy, double time);
double verticalVelocity(double vInitialy, double time);
double verticalVelocityNoTimeSquared(double vInitialySqaured, double y,double yInitital);
double timeOfFlight(double vInitital, double degree);
double horizontalRange(double vInititalSquared, double degree);
double pathShape(double x, double degree, double vInitial);
double initialVelocity(double xTarget, double degree, double yDisplacement);