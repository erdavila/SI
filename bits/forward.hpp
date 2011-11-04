#ifndef SI_FORWARD_HPP_
#define SI_FORWARD_HPP_


namespace si {


template <typename ValueType, typename Ratio, int... Dimensions>
class SIValue;

template <typename ValueType, typename Ratio, typename DimensionsList>
struct make_value;


} /* namespace si */


#endif /* SI_FORWARD_HPP_ */
