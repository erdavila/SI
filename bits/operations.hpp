#ifndef SI_OPERATIONS_HPP_
#define SI_OPERATIONS_HPP_


#include <ratio>
#include <cmath>
#include "forward.hpp"
#include "int_list.hpp"


namespace si {


/// Provides the return type of the sum of two operands.
/**
 * @see multiplication
 */
template <typename T1, typename T2>
struct addition {
private:
	// The implementation is on an internal struct because Doxygen was confused the other way.
	struct _impl {
		typedef decltype(*static_cast<T1*>(nullptr) + *static_cast<T2*>(nullptr)) type;
	};

public:
	/// The resulting type of the sum of values whose types are defined by the template arguments.
	typedef typename _impl::type type;
};


template <typename ValueType1, typename Ratio1, int... Dimensions1,
          typename ValueType2, typename Ratio2, int... Dimensions2>
struct addition<SIValue<ValueType1, Ratio1, Dimensions1...>,
                SIValue<ValueType2, Ratio2, Dimensions2...>>
{
private:
	static_assert(std::is_same<int_list<Dimensions1...>,
	                           int_list<Dimensions2...>>::value,
	              "The units must be the same on the addition");

	typedef typename addition<ValueType1, ValueType2>::type _NewValueType;
	static const unsigned int _new_num = 1;
	static const unsigned int _new_den = std::ratio_add<Ratio1, Ratio2>::type::den;
	typedef std::ratio<_new_num, _new_den> _NewRatio;

	typedef int_list<Dimensions1...> _NewDimensionsList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewDimensionsList>::type type;
};




/// Provides the return type of the product of two operands.
/**
 * It can be used to define derived SI units. For instance, given a @c Length_cm
 * type which measures lengths in centimeters and an @c Area_cm2 type which
 * measures areas in square centimeters (and is a derived SI unit itself), a new
 * type that measures volumes in cubic centimeters can be defined this way:
 * @code
 *   typedef typename ::si::multiplication<Length_cm, Area_cm2>::type Volume_cm3;
 * @endcode
 */
template <typename T1, typename T2>
struct multiplication {
private:
	// The implementation is on an internal struct because Doxygen was confused the other way.
	struct _impl {
		typedef decltype(*static_cast<T1*>(nullptr) * *static_cast<T2*>(nullptr)) type;
	};

public:
	/// The resulting type of the product of values whose types are defined by the template arguments.
	typedef typename _impl::type type;
};




template <typename ValueType1, typename Ratio1, int... Dimensions1,
          typename ValueType2, typename Ratio2, int... Dimensions2>
struct multiplication<SIValue<ValueType1, Ratio1, Dimensions1...>,
                      SIValue<ValueType2, Ratio2, Dimensions2...>>
{
private:
	typedef typename multiplication<ValueType1, ValueType2>::type _NewValueType;
	typedef typename std::ratio_multiply<Ratio1, Ratio2>::type _NewRatio;

	typedef int_list<Dimensions1...> _DimensionsList1;
	typedef int_list<Dimensions2...> _DimensionsList2;
	typedef typename int_list_add<_DimensionsList1, _DimensionsList2>::type _NewDimensionsList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewDimensionsList>::type type;
};



/// Provides the return type of the quotient of two operands.
/**
 * It can be used to define derived SI units. For instance, given a @c Length_cm
 * type which measures lengths in centimeters and an @c Time_s type which
 * measures time in seconds, a new type that measures speed in centimeters per
 * second can be defined this way:
 * @code
 *   typedef typename ::si::division<Length_cm, Time_s>::type Speed_cm_s;
 * @endcode
 */
template <typename T1, typename T2>
struct division {
	typedef decltype(*static_cast<T1*>(nullptr) / *static_cast<T2*>(nullptr)) type;
};


template <typename ValueType1, typename Ratio1, int... Dimensions1,
          typename ValueType2, typename Ratio2, int... Dimensions2>
struct division<SIValue<ValueType1, Ratio1, Dimensions1...>,
                SIValue<ValueType2, Ratio2, Dimensions2...>>
{
private:
	typedef typename division<ValueType1, ValueType2>::type _NewValueType;
	typedef typename std::ratio_divide<Ratio1, Ratio2>::type _NewRatio;

	typedef int_list<Dimensions1...> _DimensionsList1;
	typedef int_list<Dimensions2...> _DimensionsList2;
	typedef typename int_list_subtract<_DimensionsList1, _DimensionsList2>::type _NewDimensionsList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewDimensionsList>::type type;
};



template <typename SIValue>
struct sqrt_function {
private:
	typedef typename SIValue::ValueType _Ptr;
	typedef decltype(sqrt(*static_cast<_Ptr*>(nullptr))) _NewValueType;

	typedef ::std::ratio<1> _NewRatio;

	typedef typename SIValue::DimensionsList _DimensionsList;
	typedef typename int_list_half<_DimensionsList>::type _NewDimensionsList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewDimensionsList>::type type;
};



} /* namespace si */


#endif /* SI_OPERATIONS_HPP_ */
