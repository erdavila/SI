#ifndef SI_HPP_
#define SI_HPP_


namespace si {


#include "bits/ratios.hpp"



template <typename ValueType, typename Ratio, int... Units>
struct Value {
	ValueType value;


	Value() = default;

	Value(const Value& v) = default;

	/*
	template <typename ValueType2, typename... Units2>
	Value(const Value<ValueType2, Units2...>& v)
		: value(convertFrom<ValueType2, Units2...>(v.value))
	{
		typedef typename get_dimensions<Units... >::value dimensions1;
		typedef typename get_dimensions<Units2...>::value dimensions2;
		static_assert(same_types<dimensions1, dimensions2>::value, "Can't construct from a different unit");
	}
	*/

	Value(const ValueType& value) : value(value) {}


	/*
	Value& operator*=(int n) {
		value *= n;
		return *this;
	}

	Value& operator*=(double);

	Value& operator/=(int n) {
		value /= n;
		return *this;
	}

	Value& operator/=(double);


	template <typename ValueType2, typename... Units2>
	Value& operator+=(const Value<ValueType2, Units2...>& v) {
		value += convertFrom<ValueType2, Units2...>(v.value);
		return *this;
	}
	*/


	template <unsigned int Multiplier, unsigned int Divider>
	struct with_ratio {
		typedef ratio<Multiplier, Divider> _NewRatio;
		typedef Value<ValueType, _NewRatio, Units...> type;
	};

	template <unsigned int Multiplier, unsigned int Divider>
	struct apply_ratio {
		typedef ratio<Multiplier, Divider> _Ratio2;
		typedef typename ratios::multiply<Ratio, _Ratio2>::value _NewRatio;
		typedef typename with_ratio<_NewRatio::multiplier, _NewRatio::divider>::type type;
	};


	/*
private:
	template <typename ValueType2, typename... Units2>
	static ValueType convertFrom(ValueType2 value) {
		typedef typename get_dimensions<Units... >::value dimensions1;
		typedef typename get_dimensions<Units2...>::value dimensions2;
		static_assert(same_types<dimensions1, dimensions2>::value, "Can't convert from a different unit");

		typedef typename multiply_ratios<Units ...>::value Ratio1;
		typedef typename multiply_ratios<Units2...>::value Ratio2;

		return value * Ratio2::numerator * Ratio1::denominator / (Ratio2::denominator * Ratio1::numerator);
	}
	*/
};



template <typename ValueType, typename Ratio, int... Units>
bool
operator==(const Value<ValueType, Ratio, Units...>& v1,
           const Value<ValueType, Ratio, Units...>& v2)
{
	return v1.value == v2.value;
}



/*
template <typename ValueType1, typename... Units1,
          typename ValueType2, typename... Units2>
bool
operator==(const Value<ValueType1, Units1...>& v1,
           const Value<ValueType2, Units2...>& v2)
{
	typedef typename get_dimensions<Units1...>::value dimensions1;
	typedef typename get_dimensions<Units2...>::value dimensions2;
	static_assert(same_types<dimensions1, dimensions2>::value, "Can't compare value of different units");

	typedef typename multiply_ratios<Units1...>::value Ratio1;
	typedef typename multiply_ratios<Units2...>::value Ratio2;

	return v1.value * Ratio1::numerator * Ratio2::denominator
	    == v2.value * Ratio2::numerator * Ratio1::denominator;
}
*/


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Units>
bool
operator!=(const Value<ValueType1, Ratio1, Units...>& v1,
           const Value<ValueType2, Ratio2, Units...>& v2)
{
	return !(v1 == v2);
}



/*
template <typename ValueType1, typename... Units1,
          typename ValueType2, typename... Units2>
typename _binary_operation<ValueType1, _types_list<Units1...>,
                           ValueType2, _types_list<Units2...>>::product_type
operator*(const Value<ValueType1, Units1...>& v1,
          const Value<ValueType2, Units2...>& v2);
*/



template <typename ValueType, typename Ratio, int... Units>
Value<ValueType, Ratio, Units...>
operator+(const Value<ValueType, Ratio, Units...>& v1,
          const Value<ValueType, Ratio, Units...>& v2)
{
	return { v1.value + v2.value };
}



} /* namespace si */


#define SI_LENGTH_METER(VALUETYPE)       ::si::Value<VALUETYPE, ::si::ratio<1, 1>, 1>
#define SI_LENGTH_CENTIMETER(VALUETYPE)  SI_LENGTH_METER(VALUETYPE)::apply_ratio<1, 100>::type

/*
#define SI_AREA_METER(VALUETYPE) ::si::Value<VALUETYPE, ::si::_Unit<2,1,1>>
*/


#endif /* SI_HPP_ */
