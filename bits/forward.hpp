#ifndef FORWARD_HPP_
#define FORWARD_HPP_


namespace si {


template <typename ValueType, typename Ratio, int... BaseUnitPowers>
class SIValue;

template <typename ValueType, typename Ratio, typename UnitsList>
struct make_value;


} /* namespace si */


#endif /* FORWARD_HPP_ */
