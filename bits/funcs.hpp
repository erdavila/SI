#ifndef SI_FUNCS_HPP_
#define SI_FUNCS_HPP_


#include <cmath>


namespace std {


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
::si::SIValue<ValueType, Ratio, BaseUnitPowers...>
abs(const ::si::SIValue<ValueType, Ratio, BaseUnitPowers...>& v) {
	typedef
		typename ::si::SIValue<ValueType, Ratio, BaseUnitPowers...>
		ResultType;

	return ResultType(abs(v.value));
}


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
typename ::si::sqrt_function< ::si::SIValue<ValueType, Ratio, BaseUnitPowers...>>::type
sqrt(const ::si::SIValue<ValueType, Ratio, BaseUnitPowers...>& v) {
	static_assert(::si::int_list_all_even< ::si::int_list<BaseUnitPowers...>>::value, "All base unit powers must be even");

	typedef
		typename ::si::sqrt_function< ::si::SIValue<ValueType, Ratio, BaseUnitPowers...>>::type
		ResultType;
	return ResultType(sqrt(v.value * Ratio::num / Ratio::den));
}


} /* namespace std */


#endif /* SI_FUNCS_HPP_ */
