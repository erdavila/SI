#ifndef MULTIPLIERS_HPP_
#define MULTIPLIERS_HPP_


#include "multiply_fractions.hpp"
#include "power.hpp"


template <typename... Units>
struct _multipliers;

template <>
struct _multipliers<> {
	struct value {
		enum {
			numerator = 1,
			denominator = 1
		};
	};
};

template <typename First, typename... Units>
struct _multipliers<First, Units...> {
	typedef typename _power_fraction<typename First::ratio, First::dimensions>::value _first_power;
	typedef typename _multipliers<Units...>::value _rest_powers;

	typedef typename _multiply_fractions<_first_power, _rest_powers>::value value;
};



#endif /* MULTIPLIERS_HPP_ */
