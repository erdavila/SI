#include <cassert>
#include "si.hpp"

#include <iostream>

using namespace std;




typedef SI_LENGTH_METER(int) LengthInMeters;
typedef SI_LENGTH_CENTIMETER(int) LengthInCentimeters;

typedef SI_AREA_METER(int) AreaInMeters;



/*
ostream& operator<<(ostream& os, ::si::units_list<>) {
	return os;
}

template <int... Units>
ostream& operator<<(ostream& os, ::si::units_list<Units...> list) {
	os << list.head << ',' << typename ::si::units_list<Units...>::tail();
	return os;
}
*/


//#define TRY_TO_COMPILE_EVERYTHING


#ifdef TRY_TO_COMPILE_EVERYTHING
 #define CANT_COMPILE(CODE) CODE; assert(!"Compilation should have failed!");
#else
 #define CANT_COMPILE(CODE)
#endif


#include "tests/test_additions.hpp"



int main() {
	additions::test();


	LengthInMeters lengthMeter1;
	LengthInMeters lengthMeter2(3);
	LengthInMeters lengthMeter3 = lengthMeter2;

	assert(lengthMeter2.value == 3);   // 3m
	assert(lengthMeter3.value == 3);   // 3m
	assert(lengthMeter2 == lengthMeter3);

	lengthMeter1 = lengthMeter2 + lengthMeter3;   // 3m + 3m = 6m
	assert(lengthMeter1.value == 6);   // 6m
	assert(lengthMeter1 != lengthMeter3);

	// 6m = 600cm;
	typedef SI_LENGTH_CENTIMETER(int) LengthInCentimeters;
	LengthInCentimeters lengthCentimeter1 = lengthMeter1;
	assert(lengthCentimeter1.value == 600);     // 6m = 600cm
	assert(lengthCentimeter1 == lengthMeter1);

	lengthMeter1 *= 2;   // 6m * 2 = 12m
	assert(lengthMeter1.value == 12);
	assert(static_cast<LengthInCentimeters>(lengthMeter1).value == 1200);   // 12m = 1200cm

	lengthMeter1 += lengthCentimeter1;    // 12m + 600cm = 18m
	assert(lengthMeter1.value == 18);

	lengthCentimeter1 /= 24;    // 600cm / 24 = 25cm
	assert(lengthCentimeter1.value == 25);

	typedef SI_LENGTH_METER(double) LengthInMetersDouble;
	assert(lengthCentimeter1 + lengthMeter1 == LengthInCentimeters(1825));    //  25cm + 18m = 1825cm
	assert(lengthCentimeter1 + lengthMeter1 == LengthInMetersDouble(18.25));    //  25cm + 18m = 18.25m


	lengthMeter1.value -= 6;    // 18m - 6m =  12m
	AreaInMeters areaMeter = lengthCentimeter1 * lengthMeter1;    // 25cm * 12m = 3m²
	assert(areaMeter.value = 3);

	typedef SI_AREA_CENTIMETER(int) AreaInCentimeters;
	AreaInCentimeters areaCentimeter = areaMeter;    // 3m² = 30000cm²
	assert(areaCentimeter.value = 30000);


	cout << "OK" << endl;
}
