#ifndef UNITS_HPP_
#define UNITS_HPP_


namespace units {


void test() {
	using namespace si::units;

	{
		const Length_m len = 7*m;
		assert(len.value == 7);
	}

	{
		const Length_m len = 7*km;
		assert(len.value == 7000);
	}

	{
		const Length_km len = 7*m;
		assert(len.value == 0); // Truncated!
	}

	{
		const Length_km len = 7*km;
		assert(len.value == 7);
	}

	{
		const Length_km len = 4321*m;
		assert(len.value == 4); // Truncated!
	}

	{
		const Length_m len = 7.2*m;
		assert(len.value == 7); // Truncated!
	}

	{
		const Length_m len = 7.2*km;
		assert(len.value == 7200);
	}

	{
		const Length_km len = 7.2*m;
		assert(len.value == 0); // Truncated!
	}

	{
		const Length_km len = 7.2*km;
		assert(len.value == 7); // Truncated!
	}

	{
		const Length_km len = 4321.2*m;
		assert(len.value == 4);
	}
}


}


#endif /* UNITS_HPP_ */
