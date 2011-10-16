#ifndef MULTIPLICATIONS_HPP_
#define MULTIPLICATIONS_HPP_


namespace multiplications {


void scalar() {
	{
		const Length_m len_m(4);
		assert(len_m * 3 == Length_m(12));
		assert(3 * len_m == Length_m(12));
		assert(len_m * 2.8 == LengthDbl_m(11.2));
		assert(2.8 * len_m == LengthDbl_m(11.2));
	}

	{
		Length_m len_m(4);
		len_m *= 3; // 4m * 3 = 12m
		assert(len_m.value == 12);
	}

	{
		Length_m len_m(4);
		len_m *= 2.8; // Truncated: int(4m * 2.8) = int(11.2m) = 11m
		assert(len_m.value == 11);
	}
}


void derived() {
	{
		const Length_m len4m(4);
		const Length_m len3m(3);
		const LengthDbl_m len2_8m(2.8);

		assert(len4m * len2_8m == AreaDbl_m2(11.2)); // 4m * 2.8m = 11.2m²

		const Area_m2 area12m2 = len4m * len3m; // 4m * 3m = 12m²
		assert(area12m2 == Area_m2(12));
		assert(area12m2 * Length_m(2) == Volume_m3(24)); // 12m² * 2m = 24m³
	}

	{
		const Length_m  len4m(4);
		const Length_cm len321cm(321);

		const auto area = len4m * len321cm;
		assert(area == AreaDbl_m2(12.84)); // 4m * 321cm = 4m * 3.21m = 12.84m²
		assert(area == Area_cm2(128400)); // 4m * 321cm = 400cm * 321cm = 128400cm²
	}

	{
		// Electric charge: 1*C = 1*A*s
		const ElectricCurrent_A eCurr2A(2);
		const Time_s            time4s(4);

		assert(eCurr2A * time4s == ElectricCharge_C(8)); // 2A * 4s = 8C
		assert(time4s * eCurr2A == ElectricCharge_C(8)); // 4s * 2A = 8C
	}

	{
		Length_m len(7);
		CANT_COMPILE(
			len *= len;
		);

		CANT_COMPILE(
			len *= Length_km(1);
		);
	}

}


void test() {
	scalar();
	derived();
}


} /* namespace multiplications */


#endif /* MULTIPLICATIONS_HPP_ */
