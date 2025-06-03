/*
	This code is for air data calculations
	All units are to be metric, standard scalars (m, s, kPa, kg, K, mol, etc)
	NO IMPERIAL/US CUSTOMARY/HECTOPASCAL/MILLIBARS/INHG
*/

#define GEE 9.80665 // Standard gravity on Earth ms^-2
#define GAMMA 1.4 // Ratio of specific heats for air in "low speed" (< Mach 5) flight (NASA GRC)
#define GASCONST 8.31446261815324 //kg.m^2.s^-2.K^-1.mol^-1
#define AIRCONST 287.052874 //kg.m^2.s^-2.K^-1
#define RHO 1.225 // kg.m^-3 ISA air density @ sea level
#define ISA_SONIC 340.29 //m.s^-1
#define ISA_STATIC 101.325 // kPa

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
	double indicated = sqrt((2* tPress - sPress)/ RHO);
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
	mach 		= -/(5(((impact pressure/static pressure)+1)^(2/7)-1)
	EAS 		= ISA SL sonic speed * mach no * -/(static pressure/ISA SL static pressure)
			= ISA SL sonic speed * -/(5(((impact pressure/static pressure)+1)^(2/7)-1)) * -/(static pressure/ISA SL static pressure)
*/
double mach(double impactP, double staticP){
	double machratio = (impactP/staticP) + 1;
	double mach = sqrt(5 * ((pow(machratio,2/7) - 1)));
}

double eas(double impactP, double staticP){
	double easmach = mach(impactP, staticP);
	double eas = ISA_SONIC * easmach * sqrt(staticP/ISA_STATIC);
	return eas;
}

// True Air Speed is EAS compensated for air density at altitude

/*
	air density = pressure / (gas constant * tempK)
*/
double density(double staticP, double tempK){
	double density = staticP / (AIRCONST * tempK);
	return density;
}

double tas(double eas, double density){
	double tas = eas * sqrt(density/RHO);
	return tas;
}
