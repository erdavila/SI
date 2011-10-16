#include <cassert>
#include "si.hpp"

#include <iostream>

using namespace std;




typedef SI_LENGTH_km(int)    Length_km;
typedef SI_LENGTH_km(double) LengthDbl_km;
typedef SI_LENGTH_m(int)     Length_m;
typedef SI_LENGTH_m(double)  LengthDbl_m;
typedef SI_LENGTH_cm(int)    Length_cm;

typedef SI_TIME_h(int) Time_h;

typedef SI_AREA_km2(int)       Area_km2;
typedef SI_AREA_km2(double)    AreaDbl_km2;
typedef SI_AREA_m2(int)        Area_m2;
typedef SI_AREA_m2(double)     AreaDbl_m2;
typedef SI_AREA_cm2(long long) Area_cm2;



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


#include "tests/constructors.hpp"
#include "tests/additions.hpp"
#include "tests/comparisons.hpp"



int main() {
	constructors::test();
	additions::test();
	comparisons::test();


	Length_m length_m_1;
	Length_m length_m_2(3);
	Length_m length_m_3 = length_m_2;

	assert(length_m_2.value == 3);   // 3m
	assert(length_m_3.value == 3);   // 3m
	assert(length_m_2 == length_m_3);

	length_m_1 = length_m_2 + length_m_3;   // 3m + 3m = 6m
	assert(length_m_1.value == 6);   // 6m
	assert(length_m_1 != length_m_3);

	// 6m = 600cm;
	Length_cm length_cm_1 = length_m_1;
	assert(length_cm_1.value == 600);     // 6m = 600cm
	assert(length_cm_1 == length_m_1);

	length_m_1 *= 2;   // 6m * 2 = 12m
	assert(length_m_1.value == 12);
	assert(static_cast<Length_cm>(length_m_1).value == 1200);   // 12m = 1200cm

	length_m_1 += length_cm_1;    // 12m + 600cm = 18m
	assert(length_m_1.value == 18);

	length_cm_1 /= 24;    // 600cm / 24 = 25cm
	assert(length_cm_1.value == 25);

	assert(length_cm_1 + length_m_1 == Length_cm(1825));    //  25cm + 18m = 1825cm
	assert(length_cm_1 + length_m_1 == LengthDbl_m(18.25));    //  25cm + 18m = 18.25m


	length_m_1.value -= 6;    // 18m - 6m =  12m
	Area_m2 area_m2 = length_cm_1 * length_m_1;    // 25cm * 12m = 3m²
	assert(area_m2.value = 3);

	Area_cm2 area_cm2 = area_m2;    // 3m² = 30000cm²
	assert(area_cm2.value = 30000);


	cout << "OK" << endl;
}
