#ifndef SI_FUNCS_HPP_
#define SI_FUNCS_HPP_


#include <cmath>
#include "int_list.hpp"
#include "operations.hpp"


/// The standard C++ namespace.
namespace std {


/// Returns the absolute value of the SI value.
/**
 * @details The return type is the same as the argument.
 */
template <typename ValueType, typename Ratio, int... Dimensions>
::si::SIValue<ValueType, Ratio, Dimensions...>
abs(const ::si::SIValue<ValueType, Ratio, Dimensions...>& v) {
	typedef
		typename ::si::SIValue<ValueType, Ratio, Dimensions...>
		ResultType;

	return ResultType(abs(v.value));
}


/// Returns the square root of the SI value argument.
/**
 * This function accepts only arguments where the power of the base units are
 * even. The return type has base units with half the powers of the argument
 * base units.
 */
template <typename ValueType, typename Ratio, int... Dimensions>
typename ::si::sqrt_function< ::si::SIValue<ValueType, Ratio, Dimensions...>>::type
sqrt(const ::si::SIValue<ValueType, Ratio, Dimensions...>& v) {
	static_assert(::si::int_list_all_even< ::si::int_list<Dimensions...>>::value, "All base unit powers must be even");

	typedef
		typename ::si::sqrt_function< ::si::SIValue<ValueType, Ratio, Dimensions...>>::type
		ResultType;
	return ResultType(sqrt(v.value * Ratio::num / Ratio::den));
}


} /* namespace std */


#endif /* SI_FUNCS_HPP_ */
