/*
	This code is for air data calculations
	All units are to be metric, standard scalars (m, s, kPa, kg, K, mol, etc)
	NO IMPERIAL/US CUSTOMARY/HECTOPASCAL/MILLIBARS/INHG
*/

#ifndef NO_ERROR
#include "errorcodes.h"
#endif

#define GEE 9.80665 // Standard gravity on Earth ms^-2
#define GAMMA 1.4 // Ratio of specific heats for air in "low speed" (< Mach 5) flight (NASA GRC)
#define GASCONST 8.31446261815324 //kg.m^2.s^-2.K^-1.mol^-1
#define AIRCONST 287.052874 //kg.m^2.s^-2.K^-1
#define RHO 1.225 // kg.m^-3 ISA air density @ sea level
#define ISA_SONIC 340.29 //m.s^-1
#define ISA_STATIC 101.325 // kPa
#define GASCONST_DRY 287.058 // J.kg^-1.K^-1
#define GASCONST_WV 461.495 // J.kg^-1.K^-1
#define ALTCONST 44307.69396	//	From NOAA Pressure Altitude formula
#define ALTEXP 0.190284	// From NOAA Pressure Altitude formula

// Speed of Sound c = sqrt(ratio of specific heats * gas constant * absolute temperature)

double soundspeed(double gamma,  double gasconst, double tempK){
	double sound = sqrt(gamma * gasconst * tempK);
	return sound;
}

// Mach Number = TAS / Speed of Sound

double machno(double speed, double sound){
	double machno = speed / sound;
	return machno;
}

// IAS = -/((2 * Dynamic pressure)/ ISA Air density)
double ias(double sPress, double tPress){
	tPress *= 1000;	// kPa to Pa
	sPress *= 1000;	// kPa to Pa
	double indicated = sqrt((2 * (tPress - sPress))/ RHO);
	return indicated;
}

#define TYPECONSTANT 1
// Calibrated air speed is type dependnt, depending on geomtery of pitot/static ports and airframe airflow
// EG C172 flaps up IAS:CAS 50:56 60:62 70:70 80:78 90:87 100:97 110:107 120:117 130:127 140:137 150:147 160:157
double cas(double type, double ias){
	double calibrated = ias * TYPECONSTANT;
	return calibrated;
}

// Equivalent airspeed is CAS compensated for compressibility of air, ISA SL refereced
/*
	impact pressure = static pressure((1+0.2mach^2)^(7/2)-1)
	mach 			= (sqrt(2) * sqrt(PR^(1 - 1/γ) - 1))/sqrt(γ - 1)
	EAS 			= ISA SL sonic speed * mach no * -/(static pressure/ISA SL static pressure)
					= ISA SL sonic speed * -/(5(((impact pressure/static pressure)+1)^(2/7)-1)) * -/(static pressure/ISA SL static pressure)
*/
double mach(double staticP, double impactP){
	double pressRatio = (impactP/staticP);	// dimensionless kPa vs Pa doesn't matter
	double mach = (sqrt(2) * sqrt(pow(pressRatio,(1 - (1/GAMMA))) - 1))/sqrt(GAMMA - 1);
	// double machratio = (impactP/staticP) + 1;
	// double mach = sqrt(5 * (pow(machratio,2.0/7.0) - 1));
	return mach;
}

double eas(double staticP, double impactP){
	double easmach = mach(staticP, impactP);
	double eas = ISA_SONIC * easmach * sqrt(staticP/ISA_STATIC);
	return eas;
}

// True Air Speed is EAS compensated for air density at altitude

/*
	air density = pressure / (gas constant * tempK)
	
	p_1 = saturation vapour pressure = 0.61078 * exp((17.27 * t)/(t + 237.3)) Tetens' Formula (temp > 0°C)
	p_1 = 0.61078 * exp((17.27 * t)/(t + 265.5)) (temp < 0°C)
		t is celcius, returns kPa
	
	p_v = actual vapour pressure = p_1 * RH (Relative humitidty % (0.0-1.0))
	
	p_d = dry air pressure = p - p_v (p = static pressure)
	
	ρ=((p_d)/(R_d * T)) + ((p_v)/(R_v * T))
		T is Kelvin, R_d (specific gas const dry air) = 287.058 J/(kg·K), R_v (specific gas const water vapour) = 461.495 J/(kg·K)
	
*/

double density(double spress, double tempK, double relH) {
	double tempC = toCelcius(tempK);
	double p_1, p_v, p_d, rho;
	if(tempC >= 0.0) {
		p_1 = 0.61078 * exp((17.27 * tempC)/(tempC + 237.3));
	} else if(tempC < 0.0) {
		p_1 = 0.61078 * exp((17.27 * tempC)/(tempC + 265.5));
	} else {
		exit(TEMP_ERROR); // invalid temp
	}
	
	p_v = p_1 * relH;
	p_d = spress - p_v;
	
	rho = (((p_d)/(GASCONST_DRY * tempK)) + ((p_v)/(GASCONST_WV * tempK))) * 1000;
	return rho;	
}

// double density(double staticP, double tempK){
	// double density = staticP / (AIRCONST * tempK);
	// return density;
// }

double tas(double eas, double dens){
	double tas = eas * sqrt(dens/density(101.325, 288.15, 0.0));
	return tas;
}

/*
	Altitude calculations
*/

double ialt(double qnh, double spress){
//	From NOAA Pressure Altitude calculations
//	Returns meters
	double alt;
	alt = (1 - pow((spress/qnh), ALTEXP)) * ALTCONST;
	return alt;
}
