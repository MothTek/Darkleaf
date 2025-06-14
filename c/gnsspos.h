/*
	these functions are for GPS usage
	GPS to provide lat/long to 5 decimal places (~1.11-0.435m precision)
*/
/*
	bearing between origin (pos1) and target (pos2)
	bearing which is returned is the initial bearing at the origin along the great circle
	for final bearing, calculate bearing from pos2 TO pos1 -180°

	present ground track is found from bearing between present position and previous position
	can compare with gnss module provide track info
*/

/*
	bearing = atan2(X,Y)
	Z = delta(lon1,lon2)
	X = cos(lat2) * sin(Z)
	Y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(Z)
*/
double gnssbearing(double pos1[2], double pos2[2]) {
	double theta, bearing, phi1, phi2, lambda1, lambda2, x, y, dlambda;

	// radians zone (deg * pi/180)
	phi1 = getradian(pos1[0]);
	phi2 = getradian(pos2[0]);
	lambda1 = getradian(pos1[1]);
	lambda2 = getradian(pos2[1]);

	dlambda = lambda2 - lambda1;

	y = sin(dlambda) * cos(phi2);
	x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(dlambda);

	theta = atan2(y,x);

	// degrees zone (rad * 180/pi)
	bearing = getdegree(theta);
	if (bearing > 360.0) {
		bearing = bearing - 360.0;
	}
	else if (bearing <= 0.0) {
		bearing = bearing + 360.0;
	}
	return bearing;
}

/*
	great circle distance between origin (pos1) and target (pos2)
	returns metres
*/
double gnssdistance(double pos1[2], double pos2[2]){
	double hav, arclength, distance;
	hav = haversine(pos1, pos2);
	arclength = 2 * atan2(sqrt(hav),sqrt(1-hav));
	distance = R_EARTH * arclength;
	return distance;
}

/*
	calculate the cross track error
	positive is a fly left command (right of track)
	negative is a fly right command (left of track)
*/
double crosstrack(double pos1[2], double pos2[2], double prespos[2]){
	double origbear, destbear, trk, xtrk1, xtrk2, xte, origdist, destdist;
	origbear = gnssbearing(pos1, prespos) - gnssbearing(pos1, pos2);
	if(origbear <= 0){
		origbear = origbear + 360;
	} else if (origbear > 360){
		origbear = origbear - 360;
	}
	destbear = gnssbearing(pos2, pos1) - gnssbearing(pos2, prespos);
	if(destbear <= 0) {
		destbear = destbear + 360;
	} else if (destbear > 360){
		destbear = destbear - 360;
	}
	origdist = gnssdistance(pos1, prespos);
	destdist = gnssdistance(prespos, pos2);
	xtrk1 = origdist * sin(origbear);
	xtrk2 = destdist * sin(destbear);
	xte = (xtrk1 + xtrk2) / 2;
	return xte;
}

double tofrom(double dest[2], double prespos[2], double lastpos[2]){
	double track, bearing, relativebearing;
	track = gnssbearing(lastpos, prespos);
	bearing = gnssbearing(prespos, dest);
	relativebearing = bearing - track;
	if(relativebearing <= 0){
		relativebearing += 360;
	}
	if(relativebearing > 270 && relativebearing <= 360){
		return 1;
	} else if(relativebearing < 270 && relativebearing > 90){
		return 0;
	} else if(relativebearing < 90 && relativebearing >= 0){
		return 1;
	} else {
		return 2;
	}
}
