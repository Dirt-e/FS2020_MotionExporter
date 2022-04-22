#pragma once
#include <math.h>
#include <iostream>

double AddGravityToAccLon(double CoordAccLon, double pitch_deg);
double AddGravityToAccVert(double CoordAccVert, double pitch_deg, double roll_deg);
double AddGravityToAccLat(double CoordAccLat, double pitch_deg, double roll_deg);