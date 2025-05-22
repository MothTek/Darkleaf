//conversions
/*
	these functions convert angles between radians (for trigonometric functions) and degrees (latitude, longitude, bearing)
	radians are for computers and maths wizs, degrees are for normal people
	radians should never be displayed to the 1D10T operator
*/

#define FT_FAC 3.28084 // metres = ft/FT_FAC
#define MI_FAC 1609.3445 // metres = mi/MI_FAC
#define IN_FAC 39.37009424 // metres = in/IN_FAC
#define NM_FAC 1852 // metres = NM/NM_FAC

#ifndef M_PI
#include <math.h>
#endif

double getradian(double degree) {
	double radian;
	radian = degree * M_PI / 180.0;
	return radian;
}

double getdegree(double radian) {
	double degree;
	degree = radian * 180.0 / M_PI;
	return degree;
}

double dms2dd (double deg, double min, double sec) {
	if (deg < 0) {
		deg = fabs(deg);
		return (deg + (min/60) + (sec/3600)) * -1;
	} else {
		return deg + (min/60) + (sec/3600);
	}
}

void dd2dms (double *dms, double dd) {
	double d, m, s;
	d = floor(dd);
	m = floor(dd - d) * 60;
	s = (dd - d - (m/60)) * 3600;
	dms[0] = d;
	dms[1] = m;
	dms[2] = s;
}

double convertdistance (int in, int out, double dist) {
	//int in/out: 1x metric, 2x imperial, 3x aviation
	//in/out units: 11 m, 12 km, 13 cm, 21 ft, 22 mi, 23 in, 31 ft, 32 nm
	//convert everything to metres if not, then convert out
	double intermediate;
	
	if (in != 11) {
		if (in == 12) {
			intermediate = dist/1000;
		} else if (in == 13){
			intermediate = dist*100;
		} else if (in == 21) {
			intermediate = dist/FT_FAC;
		} else if (in == 22) {
			intermediate = dist/MI_FAC;
		} else if (in == 23) {
			intermediate = dist/IN_FAC;
		} else if (in == 31) {
			intermediate = dist/FT_FAC;
		} else if (in == 32) {
			intermediate = dist/NM_FAC;
		}
	} else if (in == 11) {
		intermediate = dist;
	}
	
	if (out == 11) {
		return intermediate;
	} else if (out == 12) {
		return intermediate/1000;
	} else if (out == 13){
		return intermediate * 100;
	} else if (out == 21) {
		return intermediate / FT_FAC;
	} else if (out == 22) {
		return intermediate / MI_FAC;
	} else if (out == 23) {
		return intermediate / IN_FAC;
	} else if (out == 31) {
		return intermediate / FT_FAC;
	} else if (out == 32) {
		return intermediate / NM_FAC;
	} 
}