#include <cassert>
#include "si.hpp"

#include <iostream>

using namespace std;
using namespace si::units;


typedef SI_LENGTH_km(int)    Length_km;
typedef SI_LENGTH_km(double) LengthDbl_km;
typedef SI_LENGTH_m(int)     Length_m;
typedef SI_LENGTH_m(double)  LengthDbl_m;
typedef SI_LENGTH_cm(int)    Length_cm;

typedef SI_TIME_s(int)     Time_s;
typedef SI_TIME_s(double)  TimeDbl_s;
typedef SI_TIME_h(int)     Time_h;

typedef SI_AREA_km2(int)       Area_km2;
typedef SI_AREA_km2(double)    AreaDbl_km2;
typedef SI_AREA_m2(int)        Area_m2;
typedef SI_AREA_m2(double)     AreaDbl_m2;
typedef SI_AREA_cm2(long long) Area_cm2;

typedef SI_VOLUME_m3(int)  Volume_m3;

typedef SI_SPEED_m_s(int)     Speed_m_s;
typedef SI_SPEED_m_s(double)  SpeedDbl_m_s;

typedef SI_ACCELERATION_m_s2(int)  Acceleration_m_s2;

typedef SI_ELECTRICCURRENT_A(int) ElectricCurrent_A;

typedef SI_ELECTRICCHARGE_C(int) ElectricCharge_C;

typedef SI_FREQUENCY_Hz(int)     Frequency_Hz;
typedef SI_FREQUENCY_Hz(double)  FrequencyDbl_Hz;



//#define TRY_TO_COMPILE_EVERYTHING

#ifdef TRY_TO_COMPILE_EVERYTHING
 #define CANT_COMPILE(CODE) CODE; assert(!"Compilation should have failed!");
#else
 #define CANT_COMPILE(CODE)
#endif


#include "tests/constructors.hpp"
#include "tests/comparisons.hpp"
#include "tests/additions.hpp"
#include "tests/subtractions.hpp"
#include "tests/multiplications.hpp"
#include "tests/divisions.hpp"
#include "tests/math.hpp"
#include "tests/units.hpp"



int main() {
	constructors::test();
	comparisons::test();
	additions::test();
	subtractions::test();
	multiplications::test();
	divisions::test();
	math::test();
	units::test();

	cout << "OK" << endl;
}
