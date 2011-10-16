#ifndef ADDITIONS_HPP_
#define ADDITIONS_HPP_


namespace additions {


void differentUnits() {
	Length_m len(3);
	Area_m2  area(4);
	Time_h   time(5);

	CANT_COMPILE(
		len + area;
	);

	CANT_COMPILE(
		len + time;
	);

	CANT_COMPILE(
		len + 7;
	);

	CANT_COMPILE(
		len + 7.0;
	);

	CANT_COMPILE(
		len += area;
	);

	CANT_COMPILE(
		len += time;
	);

	CANT_COMPILE(
		len += 7;
	);

	CANT_COMPILE(
		len += 7.0;
	);
}


void sameScaleSameUnits() {
	{
		Length_m len1(3);
		Length_m len2(4);
		Length_m sum = len1 + len2;
		assert(sum.value == 7);
	}

	{
		Area_m2 area1(3);
		Area_m2 area2(4);
		Area_m2 area3 = area1 + area2;
		assert(area3.value == 7);
	}

	{
		Length_m len(3);
		len += Length_m(4);
		assert(len.value == 7);
	}
}


void differentScaleSameUnits1() {
	{
		const Length_m  len_m(4);
		const Length_cm len_cm(2);
		const Length_km len_km(3);

		const auto len_m_plus_cm  = len_m + len_cm         ; // 4m + 2cm = 4.02m = 402cm
		const auto len_m_plus_km  = len_m +          len_km; // 4m + 3km = 3004m = 3.004km
		const auto len_cm_plus_km =         len_cm + len_km; // 2cm + 3km = 300002cm = 3.00002km

		assert(    Length_m(len_m_plus_cm).value == 4); // Truncated!
		assert( LengthDbl_m(len_m_plus_cm).value == 4.02);
		assert(   Length_cm(len_m_plus_cm).value == 402);
		assert(   Length_km(len_m_plus_cm).value == 0); // Truncated!
		assert(LengthDbl_km(len_m_plus_cm).value == 0.00402);

		assert(    Length_m(len_m_plus_km).value == 3004);
		assert(   Length_cm(len_m_plus_km).value == 300400);
		assert(   Length_km(len_m_plus_km).value == 3); // Truncated!
		assert(LengthDbl_km(len_m_plus_km).value == 3.004);

		assert(    Length_m(len_cm_plus_km).value == 3000); // Truncated!
		assert( LengthDbl_m(len_cm_plus_km).value == 3000.02);
		assert(   Length_cm(len_cm_plus_km).value == 300002);
		assert(   Length_km(len_cm_plus_km).value == 3); // Truncated!
		assert(LengthDbl_km(len_cm_plus_km).value == 3.00002);
	}

	{
		Length_m len_m(4);

		// Truncated: int(4m + 123cm) = int(5.23m) = 5m
		len_m += Length_cm(123);
		assert(len_m.value == 5);

		// int(5m + 3km) = int(3005m) = 3005m
		len_m += Length_km(3);
		assert(len_m.value == 3005);
	}

	{
		LengthDbl_m len_m(4);

		// 4m + 123cm = 5.23m
		len_m += Length_cm(123);
		assert(len_m.value == 5.23);

		// 5.23m + 3km = 3005.23m
		len_m += Length_km(3);
		assert(len_m.value == 3005.23);
	}
}


void differentScaleSameUnits2() {
	{
		const Area_m2  area_m2(2);
		const Area_cm2 area_cm2(4);
		const Area_km2 area_km2(1);

		const auto area_m2_plus_cm2  = area_m2 + area_cm2           ; // 2m² + 4cm² = 2.0004m² = 20004cm²
		const auto area_m2_plus_km2  = area_m2 +            area_km2; // 2m² + 1km² = 1000002m² = 1.000002km²
		const auto area_cm2_plus_km2 =           area_cm2 + area_km2; // 4cm² + 1km² = 10000000004cm² = 1.0000000004km²

		assert(    Area_m2(area_m2_plus_cm2).value == 2); // Truncated!
		assert( AreaDbl_m2(area_m2_plus_cm2).value == 2.0004);
		assert(   Area_cm2(area_m2_plus_cm2).value == 20004);
		assert(   Area_km2(area_m2_plus_cm2).value == 0); // Truncated!
		assert(AreaDbl_km2(area_m2_plus_cm2).value == 0.0000020004);

		assert(    Area_m2(area_m2_plus_km2).value == 1000002);
		assert(   Area_cm2(area_m2_plus_km2).value == 10000020000);
		assert(   Area_km2(area_m2_plus_km2).value == 1); // Truncated!
		assert(AreaDbl_km2(area_m2_plus_km2).value == 1.000002);

		assert(    Area_m2(area_cm2_plus_km2).value == 1000000); // Truncated!
		assert( AreaDbl_m2(area_cm2_plus_km2).value == 1000000.0004);
		assert(   Area_cm2(area_cm2_plus_km2).value == 10000000004);
		assert(   Area_km2(area_cm2_plus_km2).value == 1); // Truncated!
		assert(AreaDbl_km2(area_cm2_plus_km2).value == 1.0000000004);
	}

	{
		Area_m2 area(2);

		// Truncated: int(2m² + 1234cm²) = int(2.1234m²) = 2m²
		area += Area_cm2(1234);
		assert(area.value == 2);

		// int(2m² + 1km²) = int(1000002m²) = 1000002m²
		area += Area_km2(1);
		assert(area.value == 1000002);
	}

	{
		AreaDbl_m2 area(2);

		// 2m² + 1234cm² = 2.1234m²
		area += Area_cm2(1234);
		assert(area.value == 2.1234);

		// 2.1234m² + 1km² = 1000002.1234m²
		area += Area_km2(1);
		assert(area.value == 1000002.1234);
	}
}


void test() {
	differentUnits();
	sameScaleSameUnits();
	differentScaleSameUnits1();
	differentScaleSameUnits2();
}


} /* namespace additions */


#endif /* ADDITIONS_HPP_ */
