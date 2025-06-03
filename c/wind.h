/*
	Various wind based calculations: drift, wind speed, wind direction, wind vectors
*/

// Drift Angle = TRK - HDG (positive drift angle: right of track)

double getdrift(double hdg, double trk){
	double drift = hdg - trk;
	return drift;
}

// vec(windspeed) = vec(ground speed) - vec(air speed)
// wind from north = ground speed * cos(track) - air speed * cos(heading)
// wind from east = ground speed  sin(track) - air speed * sin(heading)
// calculated with radians
// all bearings are TRUE NORTH
double northwind(double rhdg, double rtrk, double aSpeed, double gSpeed){
	double north = gSpeed * cos(rtrk) - aSpeed * cos(rhdg);
	return north;
}

double eastwind(double rhdg, double rtrk, double aSpeed, double gSpeed){
	double east = gSpeed * sin(rtrk) - aSpeed * sin(rhdg);
	return east;
}

double winddir(double northwind, double eastwind){
	double direction = atan2(-northwind, -eastwind);
	return direction;
}
