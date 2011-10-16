#ifndef CONSTRUCTORS_HPP_
#define CONSTRUCTORS_HPP_


namespace constructors {


void defaultConstructor() {
	SI_LENGTH_m(int) len_m_int;
	assert(len_m_int.value == int());

	SI_LENGTH_m(double) len_m_dbl;
	assert(len_m_dbl.value == double());

	SI_AREA_m2(int) area_m2_int;
	assert(area_m2_int.value == int());

	SI_AREA_m2(double) area_m2_dbl;
	assert(area_m2_dbl.value == double());
}


void copyConstructor1() {
	Length_m len1(7);
	Length_m len2 = len1;
	assert(len2.value == 7);

	Area_m2 area1(7);
	Area_m2 area2 = area1;
	assert(area2.value == 7);
}


void copyConstructor2() {
	{
		// int from double
		const Length_m len1 = LengthDbl_m(43.21);
		assert(len1.value == 43);

		// double from int
		const LengthDbl_m len2 = Length_m(7);
		assert(len2.value == 7);
	}

	{
		// m from cm
		const Length_m len1 = Length_cm(7);
		assert(len1.value == 0); // Truncated!

		// m from cm
		const LengthDbl_m len2 = Length_cm(7);
		assert(len2.value == 0.07);

		// cm from m
		const Length_cm len3 = Length_m(7);
		assert(len3.value == 700);
	}

	{
		// int m from double km
		const Length_m len1 = LengthDbl_km(2);
		assert(len1.value == 2000);

		// int km from double m
		const Length_km len2 = LengthDbl_m(1234.56);
		assert(len2.value == 1);

		// double m from int km
		const LengthDbl_m len3 = Length_km(12);
		assert(len3.value == 12000);

		// double km from int m
		const LengthDbl_km len4 = Length_m(1234);
		assert(len4.value == 1.234);
	}
}


void cant() {
	CANT_COMPILE(
		Length_m len1 = 7; // Implicit conversion
	);

	CANT_COMPILE(
		Length_m len2 = Area_m2(7); // Different unit
	);
}


void test() {
	defaultConstructor();
	copyConstructor1();
	copyConstructor2();
	cant();
}


}


#endif /* CONSTRUCTORS_HPP_ */
