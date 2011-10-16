#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_



#include "int_list.hpp"



template <typename SIValue1, typename SIValue2>
struct addition {
private:
	static_assert(std::is_same<typename SIValue1::BaseUnitPowersList,
	                           typename SIValue2::BaseUnitPowersList>::value,
	              "The powers of base units must be the same on the addition");

	typedef typename SIValue1::ValueType _ValueType1;
	typedef typename SIValue2::ValueType _ValueType2;
	typedef decltype(*static_cast<_ValueType1*>(0) + *static_cast<_ValueType2*>(0)) _NewValueType;

	typedef typename SIValue1::Ratio _Ratio1;
	typedef typename SIValue2::Ratio _Ratio2;
	static const unsigned int _new_num = 1;
	static const unsigned int _new_den = std::ratio_add<_Ratio1, _Ratio2>::den;
	typedef std::ratio<_new_num, _new_den> _NewRatio;

	typedef typename SIValue1::BaseUnitPowersList _NewBaseUnitPowersList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewBaseUnitPowersList>::type type;
};




template <typename SIValue1, typename SIValue2>
struct multiplication {
private:
	typedef typename SIValue1::ValueType _ValueType1;
	typedef typename SIValue2::ValueType _ValueType2;
	typedef decltype(*static_cast<_ValueType1*>(0) * *static_cast<_ValueType2*>(0)) _NewValueType;

	typedef typename SIValue1::Ratio _Ratio1;
	typedef typename SIValue2::Ratio _Ratio2;
	typedef std::ratio_multiply<_Ratio1, _Ratio2> _NewRatio;

	typedef typename SIValue1::BaseUnitPowersList _BaseUnitPowersList1;
	typedef typename SIValue2::BaseUnitPowersList _BaseUnitPowersList2;
	typedef typename int_list_add<_BaseUnitPowersList1, _BaseUnitPowersList2>::list _NewBaseUnitPowersList;

public:
	typedef typename make_value<_NewValueType, _NewRatio, _NewBaseUnitPowersList>::type type;
};




#endif /* OPERATIONS_HPP_ */
