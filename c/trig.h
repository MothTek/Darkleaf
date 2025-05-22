#ifndef M_PI
#include <math.h>
#endif

#define R_EARTH 6371008.7714 // mean earth radius (Arithmetic) in metres

// trigonometry
double haversine(double pos1[2], double pos2[0]){
	double phi1, phi2, lambda1, lambda2, dlambda, dphi;
	// radians zone (deg * pi/180)
	phi1 = getradian(pos1[0]);
	phi2 = getradian(pos2[0]);
	lambda1 = getradian(pos1[1]);
	lambda2 = getradian(pos2[1]);
	dlambda = lambda2 - lambda1;
	dphi = phi2 - phi1;
	
	return pow(sin(dphi/2), 2) + cos(phi1) * cos(phi2) * pow(sin(dlambda/2),2);
}