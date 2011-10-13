#ifndef GET_DIMENSIONS_HPP_
#define GET_DIMENSIONS_HPP_


#include "template_arguments_list.hpp"



template <typename... Units>
struct get_dimensions;

template <typename FirstUnit, typename... Units>
struct get_dimensions<FirstUnit, Units...> {
	static const int first = FirstUnit::dimensions;

	typedef get_dimensions<Units...> GetDimensionsType;
	typedef typename GetDimensionsType::value Dimensions;
	typedef typename Dimensions::template with_head<first> WithHeadType;
	typedef typename WithHeadType::value value;

	//typedef typename get_dimensions<Units...>::value::with_head<FirstUnit::dimensions>::value value;
	//typedef make_values_list<int, FirstUnit::dimensions, get_dimensions<Units>::value>::list value;
};

template <>
struct get_dimensions<> {
	typedef values_list<int> value;
};



#endif /* GET_DIMENSIONS_HPP_ */
