#include "AddGravity.h"

#define rad2deg 57.2957795131
#define deg2rad 1.0/rad2deg
#define Gravity 9.806

double AddGravityToAccLon(double CoordAccLon, double pitch_deg)
{
	double pitch_rad = -pitch_deg * deg2rad;		//Minus, because in FS2020 pitch-up has negative values :-/ go figure...
	double GravityLon = sin(pitch_rad) * Gravity;

	return CoordAccLon + GravityLon;
}

double AddGravityToAccVert(double CoordAccVert, double pitch_deg, double roll_deg)
{
	double pitch_rad = -pitch_deg * deg2rad;		//Minus, because in FS2020 pitch-up has negative values :-/ go figure...
	double roll_rad = roll_deg * deg2rad;

	double GravityVert = cos(pitch_rad) * cos(roll_rad) * Gravity;

	return CoordAccVert + GravityVert;
}

double AddGravityToAccLat(double CoordAccLat, double pitch_deg, double roll_deg)
{
	double pitch_rad = -pitch_deg * deg2rad;		//Minus, because in FS2020 pitch-up has negative values :-/ go figure...
	double roll_rad = roll_deg * deg2rad;

	double GravityLat = cos(pitch_rad) * sin(roll_rad) * Gravity;

	return CoordAccLat + GravityLat;
}