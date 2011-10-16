#ifndef SI_HPP_
#define SI_HPP_


namespace si {



template <typename ValueType, typename Scale, int... Units>
struct Value;

template <typename ValueType, typename Scale, typename UnitsList>
struct make_value;


#include "bits/operations.hpp"
#include "bits/scales.hpp"
#include "bits/units_list.hpp"



template <typename ValueType, typename _Scale, int... _Units>
struct Value {
	typedef _Scale Scale;
	typedef units_list<_Units...> Units;


	ValueType value;


	// Default constructor
	Value() : value() {}

	// Copy constructor
	Value(const Value& v) = default;

	// Copy constructor for different scale or underlying type
	template <typename ValueTypeFrom, typename ScaleFrom>
	Value(const Value<ValueTypeFrom, ScaleFrom, _Units...>& v)
		: value(convertFrom<ValueTypeFrom, ScaleFrom>(v.value))
	{}

	// Constructor from value
	explicit
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


	Value& operator+=(const Value& v) {
		value += v.value;
		return *this;
	}

	template <typename ValueType2, typename Scale2>
	Value& operator+=(const Value<ValueType2, Scale2, _Units...>& v) {
		value += convertFrom<ValueType2, Scale2>(v.value);
		return *this;
	}


	template <unsigned int Multiplier, unsigned int Divider>
	struct with_scale {
		typedef scale<Multiplier, Divider> _NewScale;
		typedef Value<ValueType, _NewScale, _Units...> type;
	};

	template <unsigned int Multiplier, unsigned int Divider>
	struct apply_scale {
		typedef scale<Multiplier, Divider> _Scale2;
		typedef typename scales::multiply<Scale, _Scale2>::value _NewScale;
		typedef typename with_scale<_NewScale::multiplier, _NewScale::divider>::type type;
	};


private:
	template <typename ValueTypeFrom, typename ScaleFrom>
	static ValueType convertFrom(ValueTypeFrom value) {
		/*
		result = value * Scale::divider * ScaleFrom::multiplier / (Scale::multiplier * ScaleFrom::divider)
		       = value * (Scale::divider / ScaleFrom::divider) * (ScaleFrom::multiplier / Scale::multiplier)
		       = value * factor1 * factor2;
		*/

		typedef scale<Scale::divider       , ScaleFrom::divider> factor1;
		typedef scale<ScaleFrom::multiplier, Scale::multiplier>  factor2;

		const double mult = factor1::multiplier * factor2::multiplier;
		const double div = factor1::divider    * factor2::divider;
		return value * mult / div;
	}
};


template <typename ValueType, typename Scale, int... Units>
struct make_value<ValueType, Scale, units_list<Units...>> {
	typedef Value<ValueType, Scale, Units...> type;
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



template <typename ValueType1, typename Scale1,
          typename ValueType2, typename Scale2,
          int... Units>
typename multiplication<Value<ValueType1, Scale1, Units...>, Value<ValueType2, Scale2, Units...>>::type
operator*(const Value<ValueType1, Scale1, Units...>& v1,
          const Value<ValueType2, Scale2, Units...>& v2)
{
	typedef
			typename multiplication<Value<ValueType1, Scale1, Units...>, Value<ValueType2, Scale2, Units...>>::type
			ResultType;

	return ResultType(v1.value * v2.value);
}



template <typename ValueType, typename Scale, int... Units>
Value<ValueType, Scale, Units...>
operator+(const Value<ValueType, Scale, Units...>& v1,
          const Value<ValueType, Scale, Units...>& v2)
{
	return Value<ValueType, Scale, Units...>(v1.value + v2.value);
}


template <typename ValueType1, typename Scale1,
          typename ValueType2, typename Scale2,
          int... Units>
typename addition<Value<ValueType1, Scale1, Units...>, Value<ValueType2, Scale2, Units...>>::type
operator+(const Value<ValueType1, Scale1, Units...>& v1,
          const Value<ValueType2, Scale2, Units...>& v2)
{
	typedef
			typename addition<Value<ValueType1, Scale1, Units...>, Value<ValueType2, Scale2, Units...>>::type
			ResultType;

	return ResultType(
		  v1.value * Scale1::multiplier * ResultType::Scale::divider / (Scale1::divider * ResultType::Scale::multiplier)
		+ v2.value * Scale2::multiplier * ResultType::Scale::divider / (Scale2::divider * ResultType::Scale::multiplier)
	);
}




} /* namespace si */


#define SI_LENGTH_m(VALUETYPE)   ::si::Value<VALUETYPE, ::si::scale<1, 1>, 1>
#define SI_LENGTH_cm(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::apply_scale<1, 100>::type
#define SI_LENGTH_km(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::apply_scale<1000, 1>::type

#define SI_TIME_s(VALUETYPE)     ::si::Value<VALUETYPE, ::si::scale<1, 1>, 0, 0, 1>
#define SI_TIME_min(VALUETYPE)   SI_TIME_s(VALUETYPE)::apply_scale<60, 1>::type
#define SI_TIME_h(VALUETYPE)     SI_TIME_min(VALUETYPE)::apply_scale<60, 1>::type


#define SI_AREA_m2(VALUETYPE)    ::si::multiplication<SI_LENGTH_m(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type
#define SI_AREA_cm2(VALUETYPE)   ::si::multiplication<SI_LENGTH_cm(VALUETYPE), SI_LENGTH_cm(VALUETYPE)>::type
#define SI_AREA_km2(VALUETYPE)   ::si::multiplication<SI_LENGTH_km(VALUETYPE), SI_LENGTH_km(VALUETYPE)>::type


#endif /* SI_HPP_ */
