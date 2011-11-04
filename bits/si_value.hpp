#ifndef SI_VALUE_HPP_
#define SI_VALUE_HPP_


#include <ratio>

#include "int_list.hpp"
#include "operations.hpp"


/// The namespace where the SI library is defined.
namespace si {


/**
 * @brief This class defines a type for storing an SI value.
 *
 * @details The type uses an underlying type (@c _ValueType) to store the value
 * internally. The type also considers a ratio (@c _Ratio) for storing the
 * value, and any SI base unit or derived unit can be defined.
 *
 * @tparam _ValueType The underlying type to store the SI value. Any type like
 *         @c int or @c double can be used.
 * @tparam _Ratio A ratio type used for scaling the unit. The standard @c
 *         std::ratio template can be used and its predefined instantiations as
 *         well (@c std::centi, @c std::milli, @c std::micro, @c std::kilo,
 *         @c std::mega, etc. See documentation for the standard
 *         <tt>\<ratio\></tt> header file).
 * @tparam _Dimensions... The powers of each SI base unit.
 *
 * @see http://en.wikipedia.org/wiki/International_System_of_Units
 */
template <typename _ValueType, typename _Ratio, int... _Dimensions>
class SIValue {
public:
	typedef _ValueType ValueType;
	typedef _Ratio Ratio;
	typedef int_list<_Dimensions...> DimensionsList;


	// Defines a new SI type from this type by specifying a different ratio.
	/*
	 * For example, if @c Length_cm is an SI type that stores lengths in @b
	 * centimeters, which is defined from the @a base unit @b meter, then the
	 * line @code
	 *     typedef typename Length_cm::with_ratio< ::std::ratio<1,1000>>::type Length_mm;
	 * @endcode defines the SI type @c Length_mm which stores values in @b
	 * millimeters, which is defined as a @b meter divided by @b 1000. This new
	 * type can be alternatively defined this way:
	 * @code
	 *     typedef typename Length_cm::with_ratio< ::std::milli>::type Length_mm;
	 * @endcode
	 *
	 * @see apply_ratio
	 */
	template <typename NewRatio>
	struct with_ratio {
		/// The new defined type.
		typedef SIValue<ValueType, NewRatio, _Dimensions...> type;
	};

	/// Defines a new SI type from this type by applying another ratio.
	/**
	 * For example, if @c Length_cm is an SI type that stores lengths in @b
	 * centimeters, then the line @code
	 *     typedef typename Length_cm::apply_ratio< ::std::ratio<1,10>>::type Length_mm;
	 * @endcode defines the SI type @c Length_mm which stores values in @b
	 * millimeters, which is equivalent to a @b centimeter divided by @b 10.
	 *
	 * @see with_ratio
	 */
	template <typename Ratio2>
	struct apply_ratio {
	private:
		typedef typename std::ratio_multiply<Ratio, Ratio2>::type _NewRatio;
	public:
		/// The new defined type.
		typedef typename with_ratio<_NewRatio>::type type;
	};


	/// @brief The value stored with the underlying type.
	ValueType value;


	/// Default constructor
	SIValue() : value() {}

	/// Copy constructor
	SIValue(const SIValue& v) = default;

	/// Copy constructor from different ratio or underlying type.
	/**
	 * The value is converted according to the ratios and the underlying types.
	 * A value can only be copied from another value of the same unit.
	 */
	template <typename ValueTypeFrom, typename RatioFrom>
	SIValue(const SIValue<ValueTypeFrom, RatioFrom, _Dimensions...>& v)
		: value(convertFrom<ValueTypeFrom, RatioFrom>(v.value))
	{}

	/// Constructor from underlying type value.
	explicit
	SIValue(const ValueType& value) : value(value) {}


	/// Positive operator
	SIValue operator+() const {
		return SIValue(+value);
	}

	/// Negative operator
	SIValue operator-() const {
		return SIValue(-value);
	}

	/// Multiplication assignment by an integer.
	SIValue& operator*=(int n) {
		value *= n;
		return *this;
	}

	/// Multiplication assignment by a double.
	SIValue& operator*=(double n) {
		value *= n;
		return *this;
	}

	/// Division assignment by an integer.
	SIValue& operator/=(int n) {
		value /= n;
		return *this;
	}

	/// Division assignment by a double.
	SIValue& operator/=(double n) {
		value /= n;
		return *this;
	}

	/// Addition assignment with a value with same type.
	SIValue& operator+=(const SIValue& v) {
		value += v.value;
		return *this;
	}

	/// Addition assignment with an SI value with different underlying type or ratio.
	/**
	 * The value is added according to the ratios and the underlying types.
	 * A value can only be added to another value of the same unit.
	 */
	template <typename ValueType2, typename Ratio2>
	SIValue& operator+=(const SIValue<ValueType2, Ratio2, _Dimensions...>& v) {
		value += convertFrom<ValueType2, Ratio2>(v.value);
		return *this;
	}

	/// Subtraction assignment with a value with same type.
	SIValue& operator-=(const SIValue& v) {
		value -= v.value;
		return *this;
	}

	/// Subtraction assignment with an SI value with different underlying type or ratio.
	/**
	 * The value is subtracted according to the ratios and the underlying types.
	 * A value can only be subtracted from another value of the same unit.
	 */
	template <typename ValueType2, typename Ratio2>
	SIValue& operator-=(const SIValue<ValueType2, Ratio2, _Dimensions...>& v) {
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
template <typename ValueType, typename Ratio, int... Dimensions>
struct make_value<ValueType, Ratio, int_list<Dimensions...>> {
	typedef SIValue<ValueType, Ratio, Dimensions...> type;
};



// This specialization compares values with same ratios, so no conversion is needed.
template <typename ValueType1, typename ValueType2, typename Ratio, int... Dimensions>
bool
operator==(const SIValue<ValueType1, Ratio, Dimensions...>& v1,
           const SIValue<ValueType2, Ratio, Dimensions...>& v2)
{
	return v1.value == v2.value;
}


/**
 * @brief Tests if two SI values with same units have equal values.
 *
 * Conversion of underlying types and ratios are performed as needed.
 *
 * @relates SIValue
 * @todo Cross-simplify num and den.
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
bool
operator==(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
           const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	const auto n1 = v1.value * Ratio1::num * Ratio2::den;
	const auto n2 = v2.value * Ratio2::num * Ratio1::den;
	return n1 == n2;
}


/**
 * @brief Tests if two SI values with same units have different values.
 *
 * Conversion of underlying types and ratios are performed as needed.
 *
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
bool
operator!=(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
           const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	return !(v1 == v2);
}


// This specialization compares values with same ratios, so no conversion is needed.
template <typename ValueType1, typename ValueType2, typename Ratio, int... Dimensions>
bool
operator<(const SIValue<ValueType1, Ratio, Dimensions...>& v1,
          const SIValue<ValueType2, Ratio, Dimensions...>& v2)
{
	return v1.value < v2.value;
}


/**
 * @brief Compares two SI values with same units.
 *
 * Conversion of underlying types and ratios are performed as needed.
 *
 * @relates SIValue
 * @todo Cross-simplify ratios.
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
bool
operator<(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
          const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	const auto n1 = v1.value * Ratio1::num * Ratio2::den;
	const auto n2 = v2.value * Ratio2::num * Ratio1::den;
	return n1 < n2;
}



/**
 * @brief Compares two SI values with same units.
 *
 * Conversion of underlying types and ratios are performed as needed.
 *
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
bool
operator>(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
          const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	return v2 < v1;
}



/**
 * @brief Compares two SI values with same units.
 *
 * Conversion of underlying types and ratios are performed as needed.
 *
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
bool
operator<=(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
           const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	return !(v1 > v2);
}


/**
 * @brief Compares two SI values with same units.
 *
 * Conversion of underlying types and ratios are performed as needed.
 *
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
bool
operator>=(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
           const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	return !(v1 < v2);
}



/// Multiplies an SI value to an int value.
/**
 * @return The product of the arguments. The type of the returned value is an
 *         SI value with same ratio and unit of the left operand. The underlying
 *         type of the returned value is the same type resulting from multiplying
 *         a value of the underlying type of the left operator to an int value.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename multiplication<ValueType, int>::type, Ratio, Dimensions...>
operator*(const SIValue<ValueType, Ratio, Dimensions...>& v, int i) {
	typedef
		SIValue<typename multiplication<ValueType, int>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(v.value * i);
}


/// Multiplies an int value to an SI value.
/**
 * @return The product of the arguments. The type of the returned value is an
 *         SI value with same ratio and unit of the right operand. The underlying
 *         type of the returned value is the same type resulting from multiplying
 *         an int value to a value of the underlying type of the right operator.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename multiplication<int, ValueType>::type, Ratio, Dimensions...>
operator*(int i, const SIValue<ValueType, Ratio, Dimensions...>& v) {
	typedef
		SIValue<typename multiplication<int, ValueType>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(i * v.value);
}


/// Multiplies an SI value to a double value.
/**
 * @return The product of the arguments. The type of the returned value is an
 *         SI value with same ratio and unit of the left operand. The underlying
 *         type of the returned value is the same type resulting from multiplying
 *         a value of the underlying type of the left operator to a double value.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename multiplication<ValueType, double>::type, Ratio, Dimensions...>
operator*(const SIValue<ValueType, Ratio, Dimensions...>& v, double d) {
	typedef
		SIValue<typename multiplication<ValueType, double>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(v.value * d);
}


/// Multiplies a double value to an SI value.
/**
 * @return The product of the arguments. The type of the returned value is an
 *         SI value with same ratio and unit of the right operand. The underlying
 *         type of the returned value is the same type resulting from multiplying
 *         a double value to a value of the underlying type of the right operator.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename multiplication<double, ValueType>::type, Ratio, Dimensions...>
operator*(double d, const SIValue<ValueType, Ratio, Dimensions...>& v) {
	typedef
		SIValue<typename multiplication<double, ValueType>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(d * v.value);
}



/// Multiplies two SI values.
/**
 * @return The product of the arguments. The type of the returned value is an
 *         derived SI value suitable for storing the result of the
 *         multiplication. It is implementation-defined and can be
 *         assigned/converted to a derived SI value type with the desired
 *         underlying type and ratio, given its unit is compatible.
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1, int... Dimensions1,
          typename ValueType2, typename Ratio2, int... Dimensions2>
typename multiplication<SIValue<ValueType1, Ratio1, Dimensions1...>,
                        SIValue<ValueType2, Ratio2, Dimensions2...>>::type
operator*(const SIValue<ValueType1, Ratio1, Dimensions1...>& v1,
          const SIValue<ValueType2, Ratio2, Dimensions2...>& v2)
{
	typedef
		typename multiplication<SIValue<ValueType1, Ratio1, Dimensions1...>,
		                        SIValue<ValueType2, Ratio2, Dimensions2...>>::type
		ResultType;

	return ResultType(v1.value * v2.value);
}



/// Divides an SI value by an int value.
/**
 * @return The quotient of the arguments. The type of the returned value is an
 *         SI value with same ratio and unit of the left operand. The underlying
 *         type of the returned value is the same type resulting from dividing
 *         a value of the underlying type of the left operator by an int value.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename division<ValueType, int>::type, Ratio, Dimensions...>
operator/(const SIValue<ValueType, Ratio, Dimensions...>& v, int i) {
	typedef
		SIValue<typename division<ValueType, int>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(v.value / i);
}


/// Divides an int value by an SI value.
/**
 * @return The quotient of the arguments. The type of the returned value is a
 *         derived SI value. The underlying type of the returned value is the
 *         same type resulting from dividing an int value by a value of the
 *         underlying type of the right operator.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename division<int, ValueType>::type, Ratio, Dimensions...>
operator/(int i, const SIValue<ValueType, Ratio, Dimensions...>& v) {
	typedef
		SIValue<typename division<int, ValueType>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(i / v.value);
}


/// Divides an SI value by a double value.
/**
 * @return The quotient of the arguments. The type of the returned value is an
 *         SI value with same ratio and unit of the left operand. The underlying
 *         type of the returned value is the same type resulting from dividing
 *         a value of the underlying type of the left operator by a double value.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename division<ValueType, double>::type, Ratio, Dimensions...>
operator/(const SIValue<ValueType, Ratio, Dimensions...>& v, double d) {
	typedef
		SIValue<typename division<ValueType, double>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(v.value / d);
}


/// Divides a double value by an SI value.
/**
 * @return The quotient of the arguments. The type of the returned value is a
 *         derived SI value. The underlying type of the returned value is the
 *         same type resulting from dividing a double value by a value of the
 *         underlying type of the right operator.
 * @relates SIValue
 */
template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<typename division<double, ValueType>::type, Ratio, Dimensions...>
operator/(double d, const SIValue<ValueType, Ratio, Dimensions...>& v) {
	typedef
		SIValue<typename division<double, ValueType>::type, Ratio, Dimensions...>
		ResultType;
	return ResultType(d / v.value);
}



/// Divides an SI value by another SI value with the same unit.
/**
 * @return The quotient of the arguments as a scalar number which is the
 *         proportion of the arguments. The type of the returned value is the
 *         same type of the quotient of values of the underlying types of the
 *         arguments.
 * @todo Cross-simplify ratios.
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
typename division<ValueType1, ValueType2>::type
operator/(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
          const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	/*
	result = (v1.value * Ratio1::num / Ratio1::den) / (v2.value * Ratio2::num / Ratio2::den)
	       = (v1.value * Ratio1::num / Ratio1::den) * (Ratio2::den / v2.value * Ratio2::num)
	       = v1.value * Ratio1::num * Ratio2::den / (v2.value * Ratio2::num * Ratio1::den)
	*/
	return v1.value * Ratio1::num * Ratio2::den / (v2.value * Ratio2::num * Ratio1::den);
}



/// Divides two SI values with different units.
/**
 * @return The quotient of the arguments. The type of the returned value is an
 *         SI value suitable for storing the result of the division. It is
 *         implementation-defined and can be assigned/converted to a SI value
 *         type with the desired underlying type and ratio, given its unit is
 *         compatible.
 * @relates SIValue
 * @todo Cross-simplify ratios.
 */
template <typename ValueType1, typename Ratio1, int... Dimensions1,
          typename ValueType2, typename Ratio2, int... Dimensions2>
typename division<SIValue<ValueType1, Ratio1, Dimensions1...>,
                  SIValue<ValueType2, Ratio2, Dimensions2...>>::type
operator/(const SIValue<ValueType1, Ratio1, Dimensions1...>& v1,
          const SIValue<ValueType2, Ratio2, Dimensions2...>& v2)
{
	typedef
		typename division<SIValue<ValueType1, Ratio1, Dimensions1...>,
		                  SIValue<ValueType2, Ratio2, Dimensions2...>>::type
		ResultType;

	return ResultType(v1.value / v2.value);
}



template <typename ValueType, typename Ratio, int... Dimensions>
SIValue<ValueType, Ratio, Dimensions...>
operator+(const SIValue<ValueType, Ratio, Dimensions...>& v1,
          const SIValue<ValueType, Ratio, Dimensions...>& v2)
{
	return SIValue<ValueType, Ratio, Dimensions...>(v1.value + v2.value);
}


/**
 * @brief Adds two SI values with same units.
 *
 * Conversion of underlying types and ratios are performed as needed.
 * @return The sum of the arguments. The type of the returned value is an SI
 *         value suitable for storing the result of the sum. It is
 *         implementation-defined and can be assigned/converted to an SI value
 *         type with the desired underlying type and ratio, given its unit is
 *         compatible.
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
typename addition<SIValue<ValueType1, Ratio1, Dimensions...>,
                  SIValue<ValueType2, Ratio2, Dimensions...>>::type
operator+(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
          const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	typedef
		typename addition<SIValue<ValueType1, Ratio1, Dimensions...>,
		                  SIValue<ValueType2, Ratio2, Dimensions...>>::type
		ResultType;

	return ResultType(
		  v1.value * Ratio1::num * ResultType::Ratio::den / (Ratio1::den * ResultType::Ratio::num)
		+ v2.value * Ratio2::num * ResultType::Ratio::den / (Ratio2::den * ResultType::Ratio::num)
	);
}



/**
 * @brief Subtracts two SI values with same units.
 *
 * Conversion of underlying types and ratios are performed as needed.
 * @return The difference of the arguments. The type of the returned value is an
 *         SI value suitable for storing the result of the subtraction. It is
 *         implementation-defined and can be assigned/converted to an SI value
 *         type with the desired underlying type and ratio, given its unit is
 *         compatible.
 * @relates SIValue
 */
template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Dimensions>
typename addition<SIValue<ValueType1, Ratio1, Dimensions...>,
                  SIValue<ValueType2, Ratio2, Dimensions...>>::type
operator-(const SIValue<ValueType1, Ratio1, Dimensions...>& v1,
          const SIValue<ValueType2, Ratio2, Dimensions...>& v2)
{
	return v1 + -v2;
}


} /* namespace si */


#endif /* SI_VALUE_HPP_ */
