#ifndef SI_HPP_
#define SI_HPP_


#include <ratio>
#include <type_traits>


namespace si {



template <typename ValueType, typename Ratio, int... BaseUnitPowers>
class Value;

template <typename ValueType, typename Ratio, typename UnitsList>
struct make_value;


#include "bits/operations.hpp"
#include "bits/int_list.hpp"



template <typename _ValueType, typename _Ratio, int... _BaseUnitPowers>
class Value {
public:
	typedef _ValueType ValueType;
	typedef _Ratio Ratio;
	typedef int_list<_BaseUnitPowers...> BaseUnitPowersList;


	template <typename NewRatio>
	struct with_ratio {
		typedef Value<ValueType, NewRatio, _BaseUnitPowers...> type;
	};

	template <typename Ratio2>
	struct apply_ratio {
	private:
		typedef typename std::ratio_multiply<Ratio, Ratio2>::type _NewRatio;
	public:
		typedef typename with_ratio<_NewRatio>::type type;
	};


	ValueType value;


	// Default constructor
	Value() : value() {}

	// Copy constructor
	Value(const Value& v) = default;

	// Copy constructor from different ratio or underlying type
	template <typename ValueTypeFrom, typename RatioFrom>
	Value(const Value<ValueTypeFrom, RatioFrom, _BaseUnitPowers...>& v)
		: value(convertFrom<ValueTypeFrom, RatioFrom>(v.value))
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

	template <typename ValueType2, typename Ratio2>
	Value& operator+=(const Value<ValueType2, Ratio2, _BaseUnitPowers...>& v) {
		value += convertFrom<ValueType2, Ratio2>(v.value);
		return *this;
	}

private:
	template <typename ValueTypeFrom, typename RatioFrom>
	static ValueType convertFrom(ValueTypeFrom value) {
		/*
		result = value * Ratio::den * RatioFrom::num / (Ratio::num * RatioFrom::den)
		       = value * (Ratio::den / RatioFrom::den) * (RatioFrom::num / Ratio::num)
		       = value * factor1 * factor2
		       = value * mult
		*/

		typedef std::ratio<Ratio::den, RatioFrom::den> factor1;
		typedef std::ratio<RatioFrom::num, Ratio::num> factor2;

		typedef typename std::ratio_multiply<factor1, factor2>::type mult;

		const double num = mult::num;
		const double den = mult::den;
		return value * num / den;
	}
};


// Defines an SI Value type from a unit_list type.
template <typename ValueType, typename Ratio, int... BaseUnitPowers>
struct make_value<ValueType, Ratio, int_list<BaseUnitPowers...>> {
	typedef Value<ValueType, Ratio, BaseUnitPowers...> type;
};



// This specialization compares values with same ratios, so no conversion is needed.
template <typename ValueType1, typename ValueType2, typename Ratio, int... BaseUnitPowers>
bool
operator==(const Value<ValueType1, Ratio, BaseUnitPowers...>& v1,
           const Value<ValueType2, Ratio, BaseUnitPowers...>& v2)
{
	return v1.value == v2.value;
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator==(const Value<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const Value<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	const auto n1 = v1.value * Ratio1::num * Ratio2::den;
	const auto n2 = v2.value * Ratio2::num * Ratio1::den;
	return n1 == n2;
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator!=(const Value<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const Value<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return !(v1 == v2);
}


// This specialization compares values with same ratios, so no conversion is needed.
template <typename ValueType1, typename ValueType2, typename Ratio, int... BaseUnitPowers>
bool
operator<(const Value<ValueType1, Ratio, BaseUnitPowers...>& v1,
          const Value<ValueType2, Ratio, BaseUnitPowers...>& v2)
{
	return v1.value < v2.value;
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator<(const Value<ValueType1, Ratio1, BaseUnitPowers...>& v1,
          const Value<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	const auto n1 = v1.value * Ratio1::num * Ratio2::den;
	const auto n2 = v2.value * Ratio2::num * Ratio1::den;
	return n1 < n2;
}




template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator>(const Value<ValueType1, Ratio1, BaseUnitPowers...>& v1,
          const Value<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return v2 < v1;
}



template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator<=(const Value<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const Value<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return !(v1 > v2);
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator>=(const Value<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const Value<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return !(v1 < v2);
}



template <typename ValueType, typename Ratio, int... BaseUnitPowers>
Value<typename multiplication<ValueType, int>::type, Ratio, BaseUnitPowers...>
operator*(const Value<ValueType, Ratio, BaseUnitPowers...>& v, int i) {
	typedef
		Value<typename multiplication<ValueType, int>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(v.value * i);
}


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
Value<typename multiplication<int, ValueType>::type, Ratio, BaseUnitPowers...>
operator*(int i, const Value<ValueType, Ratio, BaseUnitPowers...>& v) {
	typedef
		Value<typename multiplication<int, ValueType>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(i * v.value);
}


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
Value<typename multiplication<ValueType, double>::type, Ratio, BaseUnitPowers...>
operator*(const Value<ValueType, Ratio, BaseUnitPowers...>& v, double d) {
	typedef
		Value<typename multiplication<ValueType, double>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(v.value * d);
}


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
Value<typename multiplication<double, ValueType>::type, Ratio, BaseUnitPowers...>
operator*(double d, const Value<ValueType, Ratio, BaseUnitPowers...>& v) {
	typedef
		Value<typename multiplication<double, ValueType>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(d * v.value);
}



template <typename ValueType1, typename Ratio1, int... BaseUnitPowers1,
          typename ValueType2, typename Ratio2, int... BaseUnitPowers2>
typename multiplication<Value<ValueType1, Ratio1, BaseUnitPowers1...>,
                        Value<ValueType2, Ratio2, BaseUnitPowers2...>>::type
operator*(const Value<ValueType1, Ratio1, BaseUnitPowers1...>& v1,
          const Value<ValueType2, Ratio2, BaseUnitPowers2...>& v2)
{
	typedef
		typename multiplication<Value<ValueType1, Ratio1, BaseUnitPowers1...>,
		                        Value<ValueType2, Ratio2, BaseUnitPowers2...>>::type
		ResultType;

	return ResultType(v1.value * v2.value);
}



template <typename ValueType, typename Ratio, int... BaseUnitPowers>
Value<ValueType, Ratio, BaseUnitPowers...>
operator+(const Value<ValueType, Ratio, BaseUnitPowers...>& v1,
          const Value<ValueType, Ratio, BaseUnitPowers...>& v2)
{
	return Value<ValueType, Ratio, BaseUnitPowers...>(v1.value + v2.value);
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
typename addition<Value<ValueType1, Ratio1, BaseUnitPowers...>,
                  Value<ValueType2, Ratio2, BaseUnitPowers...>>::type
operator+(const Value<ValueType1, Ratio1, BaseUnitPowers...>& v1,
          const Value<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	typedef
		typename addition<Value<ValueType1, Ratio1, BaseUnitPowers...>,
		                  Value<ValueType2, Ratio2, BaseUnitPowers...>>::type
		ResultType;

	return ResultType(
		  v1.value * Ratio1::num * ResultType::Ratio::den / (Ratio1::den * ResultType::Ratio::num)
		+ v2.value * Ratio2::num * ResultType::Ratio::den / (Ratio2::den * ResultType::Ratio::num)
	);
}



} /* namespace si */


#define SI_LENGTH_mm(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::with_ratio< ::std::milli>::type
#define SI_LENGTH_cm(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::with_ratio< ::std::centi>::type
#define SI_LENGTH_m(VALUETYPE)   ::si::Value<VALUETYPE, ::std::ratio<1>, 1>
#define SI_LENGTH_km(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::with_ratio< ::std::kilo>::type

#define SI_TIME_ns(VALUETYPE)    SI_TIME_s(VALUETYPE)::with_ratio< ::std::nano>::type
#define SI_TIME_us(VALUETYPE)    SI_TIME_s(VALUETYPE)::with_ratio< ::std::micro>::type
#define SI_TIME_ms(VALUETYPE)    SI_TIME_s(VALUETYPE)::with_ratio< ::std::milli>::type
#define SI_TIME_s(VALUETYPE)     ::si::Value<VALUETYPE, ::std::ratio<1>, 0, 0, 1, 0>
#define SI_TIME_min(VALUETYPE)   SI_TIME_s(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
#define SI_TIME_h(VALUETYPE)     SI_TIME_min(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
#define SI_TIME_d(VALUETYPE)     SI_TIME_h(VALUETYPE)::apply_ratio< ::std::ratio<24>>::type

#define SI_AREA_cm2(VALUETYPE)   ::si::multiplication<SI_LENGTH_cm(VALUETYPE), SI_LENGTH_cm(VALUETYPE)>::type
#define SI_AREA_m2(VALUETYPE)    ::si::multiplication<SI_LENGTH_m(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type
#define SI_AREA_km2(VALUETYPE)   ::si::multiplication<SI_LENGTH_km(VALUETYPE), SI_LENGTH_km(VALUETYPE)>::type

#define SI_SPEED_m_s(VALUETYPE)  ::si::division<SI_LENGTH_m(VALUETYPE), SI_TIME_s(VALUETYPE)>::type

#define SI_ACCELERATION_m_s2(VALUETYPE)  ::si::division<SI_SPEED_m_s(VALUETYPE), SI_TIME_s(VALUETYPE)>::type

#define SI_VOLUME_m3(VALUETYPE)  ::si::multiplication<SI_AREA_m2(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type

#define SI_ELECTRICCURRENT_A(VALUETYPE) ::si::Value<VALUETYPE, ::std::ratio<1>, 0, 0, 0, 1>

#define SI_ELECTRICCHARGE_C(VALUETYPE)  ::si::multiplication<SI_ELECTRICCURRENT_A(VALUETYPE), SI_TIME_s(VALUETYPE)>::type


#endif /* SI_HPP_ */
