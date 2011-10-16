#ifndef SI_HPP_
#define SI_HPP_


#include <ratio>


namespace si {



template <typename ValueType, typename Ratio, int... Units>
class Value;

template <typename ValueType, typename Ratio, typename UnitsList>
struct make_value;


#include "bits/operations.hpp"
#include "bits/units_list.hpp"



template <typename _ValueType, typename _Ratio, int... _Units>
class Value {
public:
	typedef _ValueType ValueType;
	typedef _Ratio Ratio;
	typedef units_list<_Units...> UnitsList;


	ValueType value;


	// Default constructor
	Value() : value() {}

	// Copy constructor
	Value(const Value& v) = default;

	// Copy constructor from different ratio or underlying type
	template <typename ValueTypeFrom, typename RatioFrom>
	Value(const Value<ValueTypeFrom, RatioFrom, _Units...>& v)
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
	Value& operator+=(const Value<ValueType2, Ratio2, _Units...>& v) {
		value += convertFrom<ValueType2, Ratio2>(v.value);
		return *this;
	}


	template <typename NewRatio>
	struct with_ratio {
		typedef Value<ValueType, NewRatio, _Units...> type;
	};

	template <typename Ratio2>
	struct apply_ratio {
	private:
		typedef std::ratio_multiply<Ratio, Ratio2> _NewRatio;
	public:
		typedef typename with_ratio<_NewRatio>::type type;
	};


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

		typedef std::ratio_multiply<factor1, factor2> mult;

		const double num = mult::num;
		const double den = mult::den;
		return value * num / den;
	}
};


// Defines an SI Value type from a unit_list type.
template <typename ValueType, typename Ratio, int... Units>
struct make_value<ValueType, Ratio, units_list<Units...>> {
	typedef Value<ValueType, Ratio, Units...> type;
};



template <typename ValueType, typename Ratio, int... Units>
bool
operator==(const Value<ValueType, Ratio, Units...>& v1,
           const Value<ValueType, Ratio, Units...>& v2)
{
	return v1.value == v2.value;
}



template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Units>
bool
operator==(const Value<ValueType1, Ratio1, Units...>& v1,
           const Value<ValueType2, Ratio2, Units...>& v2)
{
	return v1.value * Ratio1::num * Ratio2::den
	    == v2.value * Ratio2::num * Ratio1::den;
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Units>
bool
operator!=(const Value<ValueType1, Ratio1, Units...>& v1,
           const Value<ValueType2, Ratio2, Units...>& v2)
{
	return !(v1 == v2);
}



template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Units>
typename multiplication<Value<ValueType1, Ratio1, Units...>, Value<ValueType2, Ratio2, Units...>>::type
operator*(const Value<ValueType1, Ratio1, Units...>& v1,
          const Value<ValueType2, Ratio2, Units...>& v2)
{
	typedef
			typename multiplication<Value<ValueType1, Ratio1, Units...>, Value<ValueType2, Ratio2, Units...>>::type
			ResultType;

	return ResultType(v1.value * v2.value);
}



template <typename ValueType, typename Ratio, int... Units>
Value<ValueType, Ratio, Units...>
operator+(const Value<ValueType, Ratio, Units...>& v1,
          const Value<ValueType, Ratio, Units...>& v2)
{
	return Value<ValueType, Ratio, Units...>(v1.value + v2.value);
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Units>
typename addition<Value<ValueType1, Ratio1, Units...>, Value<ValueType2, Ratio2, Units...>>::type
operator+(const Value<ValueType1, Ratio1, Units...>& v1,
          const Value<ValueType2, Ratio2, Units...>& v2)
{
	typedef
			typename addition<Value<ValueType1, Ratio1, Units...>, Value<ValueType2, Ratio2, Units...>>::type
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
#define SI_TIME_s(VALUETYPE)     ::si::Value<VALUETYPE, ::std::ratio<1>, 0, 0, 1>
#define SI_TIME_min(VALUETYPE)   SI_TIME_s(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
#define SI_TIME_h(VALUETYPE)     SI_TIME_min(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
#define SI_TIME_d(VALUETYPE)     SI_TIME_h(VALUETYPE)::apply_ratio< ::std::ratio<24>>::type

#define SI_AREA_cm2(VALUETYPE)   ::si::multiplication<SI_LENGTH_cm(VALUETYPE), SI_LENGTH_cm(VALUETYPE)>::type
#define SI_AREA_m2(VALUETYPE)    ::si::multiplication<SI_LENGTH_m(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type
#define SI_AREA_km2(VALUETYPE)   ::si::multiplication<SI_LENGTH_km(VALUETYPE), SI_LENGTH_km(VALUETYPE)>::type

#define SI_SPEED_m_s(VALUETYPE)  ::si::division<SI_LENGTH_m(VALUETYPE), SI_TIME_s(VALUETYPE)>::type

#define SI_ACCELERATION_m_s2(VALUETYPE)  ::si::division<SI_SPEED_m_s(VALUETYPE), SI_TIME_s(VALUETYPE)>::type


#endif /* SI_HPP_ */
