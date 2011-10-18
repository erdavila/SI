#ifndef MATH_HPP_
#define MATH_HPP_


namespace math {


void signal() {
	{
		Length_m len(7);
		assert((+len).value == +7);
		assert((-len).value == -7);
	}

	{
		Length_m len(-7);
		assert((+len).value == -7);
		assert((-len).value == +7);
	}
}


void siAbs() {
	{
		Length_m len(7);
		assert(abs(len) == len);
	}

	{
		Length_m len(-7);
		assert(abs(len) == -len);
	}
}


void testSqrt() {
	{
		Area_km2 area(49);
		const auto side = sqrt(area);
		assert(side == Length_km(7));
	}

	CANT_COMPILE(
		sqrt(Length_m());
	);

	CANT_COMPILE(
		sqrt(Acceleration_m_s2());
	);
}


void test() {
	signal();
	siAbs();
	testSqrt();
}


} /* namespace math */


#endif /* MATH_HPP_ */
