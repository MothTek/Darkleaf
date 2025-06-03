#include <stdio.h>
#ifndef M_PI
#include <math.h>
#endif

#include "conversions.h"
#include "trig.h"
#include "gpspos.h"
#include "wind.h"
#include "airdata.h"

int main(void) {
	double pos1[2], pos2[2], dms1[3], dms2[3], result, ppos[2];
	double lat1, lon1, lat2, lon2;

	pos1[0] = 0;
	pos1[1] = 0;
	pos2[0] = 10;
	pos2[1] = 10;
	ppos[0] = 10;
	ppos[1] = 0;

/*
	printf("Origin Latitude (Decimal Degrees): \n");
	scanf("%lf", &lat1);
	pos1[0] = lat1;
	printf("Origin Longtude (Decimal Degrees): \n");
	scanf("%lf", &lon1);
	pos1[1] = lon1;
	printf("Target Latitude (Decimal Degrees): \n");
	scanf("%lf", &lat2);
	pos2[0] = lat2;
	printf("Target Longtude (Decimal Degrees): \n");
	scanf("%lf", &lon2);
	pos2[1] = lon2;
*/

	printf("getbearing\n");

	double bearing = gpsbearing(pos1,pos2);
	double backbearing = gpsbearing(pos2,pos1);
	double distance = convertdistance(11,32,gpsdistance(pos1, pos2));
	double distkm = convertdistance(11,12,gpsdistance(pos1, pos2));
	double error = convertdistance(11,32,crosstrack(pos1, pos2, ppos));

	printf("Origin Position:\t%3.4f° %3.4f°\nTarget Position:\t%3.4f° %3.4f°\nBearing to target:\t%5.2f°\
	\nBearing from target:\t%5.2f°\nDistance to target:\t%.2fNM, %.2fkm\nxTrack:\t%5.2fNM\n", pos1[0], pos1[1], pos2[0],pos2[1],\
	bearing, backbearing, distance, distkm, error);

	return 0;
}
