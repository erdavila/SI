#ifndef SUBTRACTIONS_HPP_
#define SUBTRACTIONS_HPP_


namespace subtractions {


void differentUnits() {
	Length_m len(3);
	Area_m2  area(4);
	Time_h   time(5);

	CANT_COMPILE(
		len - area;
	);

	CANT_COMPILE(
		len - time;
	);

	CANT_COMPILE(
		len - 7;
	);

	CANT_COMPILE(
		len - 7.0;
	);

	CANT_COMPILE(
		len -= area;
	);

	CANT_COMPILE(
		len -= time;
	);

	CANT_COMPILE(
		len -= 7;
	);

	CANT_COMPILE(
		len -= 7.0;
	);
}


void sameRatioSameUnits() {
	{
		Length_m len1(3);
		Length_m len2(4);
		Length_m sum = len1 - len2;
		assert(sum.value == -1);
	}

	{
		Area_m2 area1(3);
		Area_m2 area2(4);
		Area_m2 area3 = area1 - area2;
		assert(area3.value == -1);
	}

	{
		Length_m len(7);
		len -= Length_m(3);
		assert(len.value == 4);
	}
}


void differentRatioSameUnits1() {
	{
		const Length_m  len_m(4);
		const Length_cm len_cm(2);
		const Length_km len_km(3);

		const auto len_m_minus_cm  = len_m - len_cm         ; // 4m - 2cm = 3.98m = 398cm
		const auto len_m_minus_km  = len_m          - len_km; // 4m - 3km = -2996m = -2.996km
		const auto len_cm_minus_km =         len_cm - len_km; // 2cm - 3km = -299998cm = 2.99998km

		assert(    Length_m(len_m_minus_cm).value == 3); // Truncated!
		assert( LengthDbl_m(len_m_minus_cm).value == 3.98);
		assert(   Length_cm(len_m_minus_cm).value == 398);
		assert(   Length_km(len_m_minus_cm).value == 0); // Truncated!
		assert(LengthDbl_km(len_m_minus_cm).value == 0.00398);

		assert(    Length_m(len_m_minus_km).value == -2996);
		assert(   Length_cm(len_m_minus_km).value == -299600);
		assert(   Length_km(len_m_minus_km).value == -2); // Truncated!
		assert(LengthDbl_km(len_m_minus_km).value == -2.996);

		assert(    Length_m(len_cm_minus_km).value == -2999); // Truncated!
		assert( LengthDbl_m(len_cm_minus_km).value == -2999.98);
		assert(   Length_cm(len_cm_minus_km).value == -299998);
		assert(   Length_km(len_cm_minus_km).value == -2); // Truncated!
		assert(LengthDbl_km(len_cm_minus_km).value == -2.99998);
	}

	{
		Length_m len_m(4);

		// Truncated: int(4m - 123cm) = int(2.77m) = 3m
		len_m -= Length_cm(123);
		assert(len_m.value == 3);

		// int(3m - 3km) = int(-2997m) = -2997m
		len_m -= Length_km(3);
		assert(len_m.value == -2997);
	}

	{
		LengthDbl_m len_m(4);

		// 4m - 123cm = 2.77m
		len_m -= Length_cm(123);
		assert(len_m.value == 2.77);

		// 2.77m - 3km = -2997.23m
		len_m -= Length_km(3);
		assert(len_m.value == -2997.23);
	}
}


void differentRatioSameUnits2() {
	{
		const Area_m2  area_m2(2);
		const Area_cm2 area_cm2(4);
		const Area_km2 area_km2(1);

		const auto area_m2_minus_cm2  = area_m2 - area_cm2           ; // 2m² - 4cm² = 1.9996m² = 19996cm²
		const auto area_m2_minus_km2  = area_m2            - area_km2; // 2m² - 1km² = -999998m² = -0.999998km²
		const auto area_cm2_minus_km2 =           area_cm2 - area_km2; // 4cm² - 1km² = -9999999996cm² = -0.9999999996km²

		assert(    Area_m2(area_m2_minus_cm2).value == 1); // Truncated!
		assert( AreaDbl_m2(area_m2_minus_cm2).value == 1.9996);
		assert(   Area_cm2(area_m2_minus_cm2).value == 19996);
		assert(   Area_km2(area_m2_minus_cm2).value == 0); // Truncated!
		assert(AreaDbl_km2(area_m2_minus_cm2).value == 0.0000019996);

		assert(    Area_m2(area_m2_minus_km2).value == -999998);
		assert(   Area_cm2(area_m2_minus_km2).value == -9999980000);
		assert(   Area_km2(area_m2_minus_km2).value == 0); // Truncated!
		assert(AreaDbl_km2(area_m2_minus_km2).value == -0.999998);

		assert(    Area_m2(area_cm2_minus_km2).value == -999999); // Truncated!
		assert( AreaDbl_m2(area_cm2_minus_km2).value == -999999.9996);
		assert(   Area_cm2(area_cm2_minus_km2).value == -9999999996);
		assert(   Area_km2(area_cm2_minus_km2).value == 0); // Truncated!
		assert(AreaDbl_km2(area_cm2_minus_km2).value == -0.9999999996);
	}

	{
		Area_m2 area(2);

		// Truncated: int(2m² - 1234cm²) = int(1.8766m²) = 2m²
		area -= Area_cm2(1234);
		assert(area.value == 2);

		// int(2m² - 1km²) = int(-999998m²) = -999998m²
		area -= Area_km2(1);
		assert(area.value == -999998);
	}

	{
		AreaDbl_m2 area(2);

		// 2m² - 1234cm² = 1.8766m²
		area -= Area_cm2(1234);
		assert(area.value == 1.8766);

		// 1.8766m² - 1km² = -999998.1234m²
		area -= Area_km2(1);
		assert(area.value == -999998.1234);
	}
}


void test() {
	differentUnits();
	sameRatioSameUnits();
	differentRatioSameUnits1();
	differentRatioSameUnits2();
}


} /* namespace subtractions */


#endif /* SUBTRACTIONS_HPP_ */
