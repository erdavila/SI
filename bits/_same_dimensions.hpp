#ifndef SAME_DIMENSIONS_HPP_
#define SAME_DIMENSIONS_HPP_


#include "template_arguments_list.hpp"


template <typename TypesList1, typename TypesList2>
struct _same_dimensions {
	static const bool value = (
			TypesList1::HeadType::dimensions ==
			TypesList2::HeadType::dimensions
		)  &&  _same_dimensions<typename TypesList1::TailTypes, typename TypesList2::TailTypes>::value;
};


template <>
struct _same_dimensions<types_list<>, types_list<>> {
	static const bool value = true;
};



#endif /* SAME_DIMENSIONS_HPP_ */
