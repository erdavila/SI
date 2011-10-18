#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_


#include <ratio>
#include "forward.hpp"
#include "int_list.hpp"


namespace si {


template <typename T1, typename T2>
struct addition {
	typedef decltype(*static_cast<T1*>(0) + *static_cast<T2*>(0)) type;
};


template <typename ValueType1, typename Ratio1, int... BaseUnitPowers1,
          typename ValueType2, typename Ratio2, int... BaseUnitPowers2>
struct addition<SIValue<ValueType1, Ratio1, BaseUnitPowers1...>,
                SIValue<ValueType2, Ratio2, BaseUnitPowers2...>>
{
private:
	static_assert(std::is_same<int_list<BaseUnitPowers1...>,
	                           int_list<BaseUnitPowers2...>>::value,
	              "The powers of base units must be the same on the addition");

	typedef typename addition<ValueType1, ValueType2>::type _NewValueType;
	static const unsigned int _new_num = 1;
	static const unsigned int _new_den = std::ratio_add<Ratio1, Ratio2>::type::den;
	typedef std::ratio<_new_num, _new_den> _NewRatio;

	typedef int_list<BaseUnitPowers1...> _NewBaseUnitPowersList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewBaseUnitPowersList>::type type;
};



template <typename T1, typename T2>
struct multiplication {
	typedef decltype(*static_cast<T1*>(0) * *static_cast<T2*>(0)) type;
};


template <typename ValueType1, typename Ratio1, int... BaseUnitPowers1,
          typename ValueType2, typename Ratio2, int... BaseUnitPowers2>
struct multiplication<SIValue<ValueType1, Ratio1, BaseUnitPowers1...>,
                      SIValue<ValueType2, Ratio2, BaseUnitPowers2...>>
{
private:
	typedef typename multiplication<ValueType1, ValueType2>::type _NewValueType;
	typedef typename std::ratio_multiply<Ratio1, Ratio2>::type _NewRatio;

	typedef int_list<BaseUnitPowers1...> _BaseUnitPowersList1;
	typedef int_list<BaseUnitPowers2...> _BaseUnitPowersList2;
	typedef typename int_list_add<_BaseUnitPowersList1, _BaseUnitPowersList2>::list _NewBaseUnitPowersList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewBaseUnitPowersList>::type type;
};


} /* namespace si */


#endif /* OPERATIONS_HPP_ */
