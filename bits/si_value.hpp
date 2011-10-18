#ifndef SI_VALUE_HPP_
#define SI_VALUE_HPP_


#include <ratio>
#include "int_list.hpp"
#include "operations.hpp"


namespace si {


template <typename _ValueType, typename _Ratio, int... _BaseUnitPowers>
class SIValue {
public:
	typedef _ValueType ValueType;
	typedef _Ratio Ratio;
	typedef int_list<_BaseUnitPowers...> BaseUnitPowersList;


	template <typename NewRatio>
	struct with_ratio {
		typedef SIValue<ValueType, NewRatio, _BaseUnitPowers...> type;
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
	SIValue() : value() {}

	// Copy constructor
	SIValue(const SIValue& v) = default;

	// Copy constructor from different ratio or underlying type
	template <typename ValueTypeFrom, typename RatioFrom>
	SIValue(const SIValue<ValueTypeFrom, RatioFrom, _BaseUnitPowers...>& v)
		: value(convertFrom<ValueTypeFrom, RatioFrom>(v.value))
	{}

	// Constructor from value
	explicit
	SIValue(const ValueType& value) : value(value) {}


	SIValue operator+() const {
		return SIValue(+value);
	}

	SIValue operator-() const {
		return SIValue(-value);
	}

	SIValue& operator*=(int n) {
		value *= n;
		return *this;
	}

	SIValue& operator*=(double n) {
		value *= n;
		return *this;
	}

	SIValue& operator/=(int n) {
		value /= n;
		return *this;
	}

	SIValue& operator/=(double);


	SIValue& operator+=(const SIValue& v) {
		value += v.value;
		return *this;
	}

	template <typename ValueType2, typename Ratio2>
	SIValue& operator+=(const SIValue<ValueType2, Ratio2, _BaseUnitPowers...>& v) {
		value += convertFrom<ValueType2, Ratio2>(v.value);
		return *this;
	}

	SIValue& operator-=(const SIValue& v) {
		value -= v.value;
		return *this;
	}

	template <typename ValueType2, typename Ratio2>
	SIValue& operator-=(const SIValue<ValueType2, Ratio2, _BaseUnitPowers...>& v) {
		value -= convertFrom<ValueType2, Ratio2>(v.value);
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
	typedef SIValue<ValueType, Ratio, BaseUnitPowers...> type;
};



// This specialization compares values with same ratios, so no conversion is needed.
template <typename ValueType1, typename ValueType2, typename Ratio, int... BaseUnitPowers>
bool
operator==(const SIValue<ValueType1, Ratio, BaseUnitPowers...>& v1,
           const SIValue<ValueType2, Ratio, BaseUnitPowers...>& v2)
{
	return v1.value == v2.value;
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator==(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	const auto n1 = v1.value * Ratio1::num * Ratio2::den;
	const auto n2 = v2.value * Ratio2::num * Ratio1::den;
	return n1 == n2;
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator!=(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return !(v1 == v2);
}


// This specialization compares values with same ratios, so no conversion is needed.
template <typename ValueType1, typename ValueType2, typename Ratio, int... BaseUnitPowers>
bool
operator<(const SIValue<ValueType1, Ratio, BaseUnitPowers...>& v1,
          const SIValue<ValueType2, Ratio, BaseUnitPowers...>& v2)
{
	return v1.value < v2.value;
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator<(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
          const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	const auto n1 = v1.value * Ratio1::num * Ratio2::den;
	const auto n2 = v2.value * Ratio2::num * Ratio1::den;
	return n1 < n2;
}




template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator>(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
          const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return v2 < v1;
}



template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator<=(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return !(v1 > v2);
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
bool
operator>=(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
           const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return !(v1 < v2);
}



template <typename ValueType, typename Ratio, int... BaseUnitPowers>
SIValue<typename multiplication<ValueType, int>::type, Ratio, BaseUnitPowers...>
operator*(const SIValue<ValueType, Ratio, BaseUnitPowers...>& v, int i) {
	typedef
		SIValue<typename multiplication<ValueType, int>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(v.value * i);
}


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
SIValue<typename multiplication<int, ValueType>::type, Ratio, BaseUnitPowers...>
operator*(int i, const SIValue<ValueType, Ratio, BaseUnitPowers...>& v) {
	typedef
		SIValue<typename multiplication<int, ValueType>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(i * v.value);
}


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
SIValue<typename multiplication<ValueType, double>::type, Ratio, BaseUnitPowers...>
operator*(const SIValue<ValueType, Ratio, BaseUnitPowers...>& v, double d) {
	typedef
		SIValue<typename multiplication<ValueType, double>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(v.value * d);
}


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
SIValue<typename multiplication<double, ValueType>::type, Ratio, BaseUnitPowers...>
operator*(double d, const SIValue<ValueType, Ratio, BaseUnitPowers...>& v) {
	typedef
		SIValue<typename multiplication<double, ValueType>::type, Ratio, BaseUnitPowers...>
		ResultType;
	return ResultType(d * v.value);
}



template <typename ValueType1, typename Ratio1, int... BaseUnitPowers1,
          typename ValueType2, typename Ratio2, int... BaseUnitPowers2>
typename multiplication<SIValue<ValueType1, Ratio1, BaseUnitPowers1...>,
                        SIValue<ValueType2, Ratio2, BaseUnitPowers2...>>::type
operator*(const SIValue<ValueType1, Ratio1, BaseUnitPowers1...>& v1,
          const SIValue<ValueType2, Ratio2, BaseUnitPowers2...>& v2)
{
	typedef
		typename multiplication<SIValue<ValueType1, Ratio1, BaseUnitPowers1...>,
		                        SIValue<ValueType2, Ratio2, BaseUnitPowers2...>>::type
		ResultType;

	return ResultType(v1.value * v2.value);
}



template <typename ValueType, typename Ratio, int... BaseUnitPowers>
SIValue<ValueType, Ratio, BaseUnitPowers...>
operator+(const SIValue<ValueType, Ratio, BaseUnitPowers...>& v1,
          const SIValue<ValueType, Ratio, BaseUnitPowers...>& v2)
{
	return SIValue<ValueType, Ratio, BaseUnitPowers...>(v1.value + v2.value);
}


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
typename addition<SIValue<ValueType1, Ratio1, BaseUnitPowers...>,
                  SIValue<ValueType2, Ratio2, BaseUnitPowers...>>::type
operator+(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
          const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	typedef
		typename addition<SIValue<ValueType1, Ratio1, BaseUnitPowers...>,
		                  SIValue<ValueType2, Ratio2, BaseUnitPowers...>>::type
		ResultType;

	return ResultType(
		  v1.value * Ratio1::num * ResultType::Ratio::den / (Ratio1::den * ResultType::Ratio::num)
		+ v2.value * Ratio2::num * ResultType::Ratio::den / (Ratio2::den * ResultType::Ratio::num)
	);
}



template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... BaseUnitPowers>
typename addition<SIValue<ValueType1, Ratio1, BaseUnitPowers...>,
                  SIValue<ValueType2, Ratio2, BaseUnitPowers...>>::type
operator-(const SIValue<ValueType1, Ratio1, BaseUnitPowers...>& v1,
          const SIValue<ValueType2, Ratio2, BaseUnitPowers...>& v2)
{
	return v1 + -v2;
}


} /* namespace si */


#endif /* SI_VALUE_HPP_ */
