#ifndef SI_HPP_
#define SI_HPP_

/*

namespace si {


#include "bits/get_dimensions.hpp"
#include "bits/multiply_ratios.hpp"
#include "bits/same_types.hpp"



template <int Dimensions, unsigned int RatioNumerator, unsigned int RatioDenominator>
struct _Unit {
	static const int dimensions = Dimensions;
	struct ratio {
		enum {
			numerator = RatioNumerator,
			denominator = RatioDenominator
		};
	};
};



template <typename ValueType, typename... Units>
struct Value {
	ValueType value;


	Value() = default;

	Value(const Value& v) = default;

	template <typename ValueType2, typename... Units2>
	Value(const Value<ValueType2, Units2...>& v)
		: value(convertFrom<ValueType2, Units2...>(v.value))
	{
		typedef typename get_dimensions<Units... >::value dimensions1;
		typedef typename get_dimensions<Units2...>::value dimensions2;
		static_assert(same_types<dimensions1, dimensions2>::value, "Can't construct from a different unit");
	}

	Value(ValueType value) : value(value) {}


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
};



template <typename ValueType, typename... Units>
bool
operator==(const Value<ValueType, Units...>& v1,
           const Value<ValueType, Units...>& v2)
{
	return v1.value == v2.value;
}



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


template <typename ValueType1, typename... Units1,
          typename ValueType2, typename... Units2>
bool
operator!=(const Value<ValueType1, Units1...>& v1,
           const Value<ValueType2, Units2...>& v2)
{
	return !(v1 == v2);
}



template <typename ValueType1, typename... Units1,
          typename ValueType2, typename... Units2>
typename _binary_operation<ValueType1, _types_list<Units1...>,
                           ValueType2, _types_list<Units2...>>::product_type
operator*(const Value<ValueType1, Units1...>& v1,
          const Value<ValueType2, Units2...>& v2);



template <typename ValueType, typename... Units>
Value<ValueType, Units...>
operator+(const Value<ValueType, Units...>& v1,
          const Value<ValueType, Units...>& v2)
{
	return { v1.value + v2.value };
}



} / * namespace si * /



#define SI_LENGTH_METER(VALUETYPE)      ::si::Value<VALUETYPE, ::si::_Unit<1,1,1>>
#define SI_LENGTH_CENTIMETER(VALUETYPE) ::si::Value<VALUETYPE, ::si::_Unit<1,1,100>>

#define SI_AREA_METER(VALUETYPE) ::si::Value<VALUETYPE, ::si::_Unit<2,1,1>>
*/


#endif /* SI_HPP_ */
