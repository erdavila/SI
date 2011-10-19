#ifndef SI_UNITS_HPP_
#define SI_UNITS_HPP_


#include "defs.hpp"


namespace si {
/// Pre-defined unit values that help to create other values.
namespace units {



/**
 * @name Length, distance
 * Base unit: meter<br>
 * Symbol: m
 */
//@{
/// 1mm (1 millimeter)
extern const SI_LENGTH_mm(int) mm;
/// 1cm (1 centimeter)
extern const SI_LENGTH_cm(int) cm;
/// 1m (1 meter)
extern const SI_LENGTH_m(int)  m ;
/// 1km (1 kilometer)
extern const SI_LENGTH_km(int) km;
//@}


/** @name Time
 * Base unit: second<br>
 * Symbol: s
 */
//@{
/// 1 second (1s)
extern const SI_TIME_s(int) s;
/// 1 hour (1h)
extern const SI_TIME_h(int) h;
//@}


/** @name Area
 * Derived unit: square meter<br>
 * Symbol: m²
 */
//@{
/// 1m² (1 square meter)
extern const SI_AREA_m2(int) m2;
//@}


} /* namespace si::units */
} /* namespace si */



#endif /* SI_UNITS_HPP_ */
