#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_



#include "units_list.hpp"




template <class T1, class T2>
struct sum_units;

template <int First1, int... Units1, int First2, int... Units2>
struct sum_units<units_list<First1, Units1...>, units_list<First2, Units2...>> {
private:
	static const int _NewFirst = First1 + First2;
	typedef typename sum_units<units_list<Units1...>, units_list<Units2...>>::list _NewRestList;
public:
	typedef typename _NewRestList::template with_head<_NewFirst>::type list;
};

template <>
struct sum_units<units_list<>, units_list<>> {
	typedef units_list<> list;
};




template <typename Value1, typename Value2>
struct addition;


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Units>
struct addition<Value<ValueType1, Ratio1, Units...>, Value<ValueType2, Ratio2, Units...>> {
private:
	typedef decltype(*static_cast<ValueType1*>(0) + *static_cast<ValueType2*>(0)) _NewValueType;

	static const unsigned int _new_num = 1;
	static const unsigned int _new_den = std::ratio_add<Ratio1, Ratio2>::den;

	typedef std::ratio<_new_num, _new_den> _NewRatio;

public:
	typedef Value<_NewValueType, _NewRatio, Units...> type;
};




template <typename Value1, typename Value2>
struct multiplication;


template <typename ValueType1, typename Ratio1,
          typename ValueType2, typename Ratio2,
          int... Units>
struct multiplication<Value<ValueType1, Ratio1, Units...>, Value<ValueType2, Ratio2, Units...>> {
private:
	typedef decltype(*static_cast<ValueType1*>(0) * *static_cast<ValueType2*>(0)) _NewValueType;

	typedef std::ratio_multiply<Ratio1, Ratio2> _NewRatio;

	typedef typename sum_units<units_list<Units...>, units_list<Units...>>::list _NewUnitsList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewUnitsList>::type type;
};




#endif /* OPERATIONS_HPP_ */
