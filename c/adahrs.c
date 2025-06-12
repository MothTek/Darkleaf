#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#ifndef M_PI
#include <math.h>
#endif

#include "conversions.h"
#include "trig.h"
#include "gnsspos.h"
#include "wind.h"
#include "airdata.h"

int main(int argc, char **argv) {
	static struct option long_options[] = {
		// What to calculate
		{"ias", 0, 0, 0},		//	Calculate indicated airspeed
		{"tas", 0, 0, 0},		//	Calculate true airspeed
		{"ialt", 0, 0,0},		//	Calculate indicated altitude
		{"bearing", 0, 0, 0},	//	Calculate bearing from point to another (gnss)
		{"distance", 0, 0, 0},	//	Calculate distance between points (gnss)
		{"xte", 0, 0, 0},		//	Calculate cross track error (gnss)
		{"tofr", 0, 0, 0},		//	Calculate to-from flag
		
		//	Sensor inputs for calculations
		{"spress", 1, 0, 0},	//	Static pressure (kPa)
		{"tpress", 1, 0, 0},	//	Total pressure reading (kPa) [Static + Dynamic]
		{"tempC", 1, 0, 0},		//	Temperature in degrees Celcius
		{"tempK", 1, 0, 0}, 	//	Temperature in Kelvin
		{"ppos", 1, 0, 0},		//	Present position [lat,lon]
		{"opos", 1, 0, 0},		//	Origin position [lat,lon]
		{"dpos", 1, 0, 0},		//	Destination position [lat,lon]
		{"lpos", 1, 0, 0},		//	Last position [lat,lon] (for when a previous actual position is required)
		{"hdg", 1, 0, 0},		//	Magnetic heading degrees (2dp)
		{"gspeed", 1, 0, 0},	//	Ground speed (gnss)
		{"gtrack", 1, 0, 0}		//	Ground track (gnss)
	}
	return 0;
}
