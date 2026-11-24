#include "myPhysics.h"

double feet2Meters(double feet){
	return feet / 3.281;
}

double inches2Meters(double inches){
	return inches/39.37;
}

double feet2Inches(double feet){
	return feet *12;
}


double vFinal(double vinital, double accerlation, double time){
	return vinital+accerlation*time;
}
/*
Inputs uncalculated square and returns an uncalculated square

*/
double vFinalSquared(double vInitialSquared, double accerlation, double displacement){
	double squared = vInitialSquared * vInitialSquared;


	return squared + 2* accerlation*displacement;
}

double displacementWithoutA(double vInitial, double vFinal, double time){
	double addition = vInitial + vFinal;
	return addition/2 * time;
}

double displacementWithoutVfinal(double vInitial, double time, double accerlation){
	double squared = time * time;
	return vInitial*time+0.5*accerlation*squared;
}

double horizontalLauchSpeed(double vInitial, double angle){
	return vInitial* cos(angle);
}

double verticalLauchSpeed(double vInitial, double angle){
	return vInitial * sin(angle);
}

double horizontalPostion(double x, double vInitialx, double time){
	return x + vInitialx*time;
}

double verticalPostion(double y, double vInitialy, double time){
	double squared = time * time;

	return y+ vInitialy*time-0.5*gravity*squared;
}

double verticalVelocity(double vInitialy, double time){
	return vInitialy - gravity * time;
}

double verticalVelocityNoTimeSquared(double vInitialySqaured, double y, double yInitital){
	double subtraction = y - yInitital;
	double squared = vInitialySqaured * vInitialySqaured;
	return squared - 2* gravity*subtraction;
}

double timeOfFlight(double vInitital, double degree){
	double top = 2 * vInitital * sin(degree);
	return top / gravity;
}

double horizontalRange(double vInititalSquared, double degree){
	double squared = vInititalSquared * vInititalSquared;
	double top = squared * sin(2 * degree);
	return top/gravity;
}

double pathShape(double x, double degree, double vInitial){
	double xSqaured = x * x;
	double bottomSquared = (vInitial * cos(degree)) * (vInitial * cos(degree));
	return tan(degree)*x-(gravity/bottomSquared)*xSqaured;
}

double initialVelocity(double xTarget, double degree, double yDisplacement){
	double cosSquared = cos(degree) * cos(degree);
	double xSquared = xTarget * xTarget;
	double top = gravity * xSquared;
	double bottom = 2 * cosSquared * (xTarget * tan(degree) - yDisplacement);
	
	return sqrt(top/bottom);
}


