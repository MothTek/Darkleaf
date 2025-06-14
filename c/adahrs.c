#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#ifndef M_PI
#include <math.h>
#endif

#include "conversions.h"
#include "trig.h"
#include "gnsspos.h"
#include "wind.h"
#include "airdata.h"
#include "errorcodes.h"

static int tocalc_flag;

int main(int argc, char **argv) {
	int c;
	
	double spress, tpress, tempC, tempK, relH, hdg, gspeed, gtrack;
	double ppos[2], opos[2], dpos[2], lpos[2];
	
	//	variable has been set?
	double spress_set = 0, tpress_set = 0, tempC_set = 0, tempK_set = 0, relH_set = 0, hdg_set = 0, gspeed_set = 0, gtrack_set = 0;
	double ppos_set = 0, opos_set = 0, dpos_set = 0, lpos_set = 0;
	
	while (1) {
		static struct option long_options[] = {
			// What to calculate
			{"ias", no_argument, &tocalc_flag, 1},		//	Calculate indicated airspeed
			{"tas", no_argument, &tocalc_flag, 2},		//	Calculate true airspeed
			{"ialt", no_argument, &tocalc_flag,3},		//	Calculate indicated altitude
			{"bearing", no_argument, &tocalc_flag, 4},	//	Calculate bearing from point to another (gnss)
			{"distance", no_argument, &tocalc_flag, 5},	//	Calculate distance between points (gnss)
			{"xte", no_argument, &tocalc_flag, 6},		//	Calculate cross track error (gnss)
			{"tofr", no_argument, &tocalc_flag, 7},		//	Calculate to-from flag
			
			//	Sensor inputs for calculations
			{"spress", required_argument, 0, 's'},	//	Static pressure (kPa)
			{"tpress", required_argument, 0, 't'},	//	Total pressure reading (kPa) [Static + Dynamic]
			{"tempC", required_argument, 0, 'c'},		//	Temperature in degrees Celcius
			{"tempK", required_argument, 0, 'k'}, 	//	Temperature in Kelvin
			{"relH", required_argument, 0, 'h'},	//	Relative humidity % as value 0.0 -> 1.0
			{"ppos", required_argument, 0, 'p'},		//	Present position [lat,lon]
			{"opos", required_argument, 0, 'o'},		//	Origin position [lat,lon]
			{"dpos", required_argument, 0, 'd'},		//	Destination position [lat,lon]
			{"lpos", required_argument, 0, 'l'},		//	Last position [lat,lon] (for when a previous actual position is required)
			{"hdg", required_argument, 0, 'H'},		//	Magnetic heading degrees (2dp)
			{"gspeed", required_argument, 0, 'S'},	//	Ground speed (gnss)
			{"gtrack", required_argument, 0, 'T'},		//	Ground track (gnss)
			{0, 0, 0, 0}
		};
		
		int option_index = 0;
		c = getopt_long (argc, argv, "", long_options, &option_index);
		
		if (c == -1) {
			break;
		}
		
		switch(c) {
			// case 0:
				// if(long_options[option_index].flag != 0)
					// break;
			case 's':
				spress = atof(optarg); // kPa
				spress_set = 1;
				break;
			case 't':
				tpress = atof(optarg); // kPa
				tpress_set = 1;
				break;
			case 'c':
				tempC = atof(optarg);
				tempC_set = 1;
				break;
			case 'k':
				tempK = atof(optarg);
				tempK_set = 1;
				break;
			case 'h':
				relH = atof(optarg);
				relH_set = 1;
				
//	usage --ppos lat,lon or \[lat,lon] or "[lat,lon]"				
			case 'p':
				parse_position(optarg, ppos);
				ppos_set = 1;
				break;
			case 'o':
				parse_position(optarg, opos);
				opos_set = 1;
				break;
			case 'd':
				parse_position(optarg, dpos);
				dpos_set = 1;
				break;
			case 'l':
				parse_position(optarg, lpos);
				lpos_set = 1;
				break;
			case 'H':
				hdg = atof(optarg);
				hdg_set = 1;
				break;
			case 'S':
				gspeed = atof(optarg);
				gspeed_set = 1;
				break;
			case 'T':
				gtrack = atof(optarg);
				gtrack_set = 1;
				break;
			case '?':
				//error
				break;
			//default:
				//abort();
		}
	}
	
	switch(tocalc_flag) {
		case 0:	{	//	no function called
			printf("no function called\n");
			exit(NO_FUNC);
		}
		case 1:	{	//	ias
			if (!spress_set) {
				printf("no spress\n");
				exit(INSUFF_ARGS);
			} else if (!tpress_set) {
				printf("no tpress\n");
				exit(INSUFF_ARGS);
			} else {
				printf("%f\n", ias(spress, tpress));
				exit(NO_ERROR);
			}
		}
		case 2: {
			if (!spress_set) {
				printf("no spress\n");
				exit(INSUFF_ARGS);
			} else if (!tpress_set) {
				printf("no tpress\n");
				exit(INSUFF_ARGS);
			} else if (!tempK_set) {
				printf("no tempK\n");
				exit(INSUFF_ARGS);
			} else if (!relH_set) {
				printf("no relH\n");
			} else {
				printf("%f\n", tas(eas(spress,tpress),density(spress, tempK, relH)));
				exit(NO_ERROR);
			}
		}
	}
	exit(NO_FUNC);
}
