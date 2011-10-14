#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_



#include "scales.hpp"


template <typename Value1, typename Value2>
struct addition;


template <typename ValueType1, typename Scale1,
          typename ValueType2, typename Scale2,
          int... Units>
struct addition<Value<ValueType1, Scale1, Units...>, Value<ValueType2, Scale2, Units...>> {
	typedef decltype(*static_cast<ValueType1*>(0) + *static_cast<ValueType2*>(0)) _NewValueType;

	static const unsigned int _new_multiplier = 1;
	static const unsigned int _new_divider = lcm<Scale1::divider, Scale2::divider>::value;

	typedef scale<_new_multiplier, _new_divider> _NewScale;

	typedef Value<_NewValueType, _NewScale, Units...> type;
};




template <typename Value1, typename Value2>
struct multiplication;


template <typename ValueType1, typename Scale1,
          typename ValueType2, typename Scale2,
          int... Units>
struct multiplication<Value<ValueType1, Scale1, Units...>, Value<ValueType2, Scale2, Units...>> {
	typedef decltype(*static_cast<ValueType1*>(0) * *static_cast<ValueType2*>(0)) _NewValueType;

	typedef typename scales::multiply<Scale1, Scale2>::value _NewScale;

	typedef Value<_NewValueType, _NewScale, Units...> type;
};


#endif /* OPERATIONS_HPP_ */
