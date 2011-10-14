#ifndef SI_HPP_
#define SI_HPP_


namespace si {


#include "bits/scales.hpp"



template <typename ValueType, typename Scale, int... Units>
struct Value {
	ValueType value;


	Value() = default;

	Value(const Value& v) = default;

	template <typename ValueType2, typename Scale2>
	Value(const Value<ValueType2, Scale2, Units...>& v)
		: value(convertFrom<ValueType2, Scale2>(v.value))
	{}

	Value(const ValueType& value) : value(value) {}


	Value& operator*=(int n) {
		value *= n;
		return *this;
	}

	Value& operator*=(double n) {
		value *= n;
		return *this;
	}

	Value& operator/=(int n) {
		value /= n;
		return *this;
	}

	Value& operator/=(double);


	Value& operator+=(const Value&);

	template <typename ValueType2, typename Scale2>
	Value& operator+=(const Value<ValueType2, Scale2, Units...>& v) {
		value += convertFrom<ValueType2, Scale2>(v.value);
		return *this;
	}


	template <unsigned int Multiplier, unsigned int Divider>
	struct with_scale {
		typedef scale<Multiplier, Divider> _NewScale;
		typedef Value<ValueType, _NewScale, Units...> type;
	};

	template <unsigned int Multiplier, unsigned int Divider>
	struct apply_scale {
		typedef scale<Multiplier, Divider> _Scale2;
		typedef typename scales::multiply<Scale, _Scale2>::value _NewScale;
		typedef typename with_scale<_NewScale::multiplier, _NewScale::divider>::type type;
	};


private:
	template <typename ValueType2, typename Scale2>
	static ValueType convertFrom(ValueType2 value) {
		return value * Scale2::multiplier * Scale::divider / (Scale2::divider * Scale::multiplier);
	}
};



template <typename ValueType, typename Scale, int... Units>
bool
operator==(const Value<ValueType, Scale, Units...>& v1,
           const Value<ValueType, Scale, Units...>& v2)
{
	return v1.value == v2.value;
}



template <typename ValueType1, typename Scale1,
          typename ValueType2, typename Scale2,
          int... Units>
bool
operator==(const Value<ValueType1, Scale1, Units...>& v1,
           const Value<ValueType2, Scale2, Units...>& v2)
{
	return v1.value * Scale1::multiplier * Scale2::divider
	    == v2.value * Scale2::multiplier * Scale1::divider;
}


template <typename ValueType1, typename Scale1,
          typename ValueType2, typename Scale2,
          int... Units>
bool
operator!=(const Value<ValueType1, Scale1, Units...>& v1,
           const Value<ValueType2, Scale2, Units...>& v2)
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



template <typename ValueType, typename Scale, int... Units>
Value<ValueType, Scale, Units...>
operator+(const Value<ValueType, Scale, Units...>& v1,
          const Value<ValueType, Scale, Units...>& v2)
{
	return { v1.value + v2.value };
}



} /* namespace si */


#define SI_LENGTH_METER(VALUETYPE)       ::si::Value<VALUETYPE, ::si::scale<1, 1>, 1>
#define SI_LENGTH_CENTIMETER(VALUETYPE)  SI_LENGTH_METER(VALUETYPE)::apply_scale<1, 100>::type

/*
#define SI_AREA_METER(VALUETYPE) ::si::Value<VALUETYPE, ::si::_Unit<2,1,1>>
*/


#endif /* SI_HPP_ */
