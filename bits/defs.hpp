#ifndef SI_DEFS_HPP_
#define SI_DEFS_HPP_

/** @file */


/** @name Length, Distance
 * Base unit: metre (meter)<br>
 * Symbol: m
 */
//@{
/// Length in millimeters.
#define SI_LENGTH_mm(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::apply_ratio< ::std::milli>::type
/// Length in centimeters
#define SI_LENGTH_cm(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::apply_ratio< ::std::centi>::type
/// Length in meters
#define SI_LENGTH_m(VALUETYPE)   ::si::SIValue<VALUETYPE, ::std::ratio<1>, 1, 0, 0, 0>
/// Length in kilometers
#define SI_LENGTH_km(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::apply_ratio< ::std::kilo>::type
//@}

/** @name Time
 * Base unit: second<br>
 * Symbol: s
 */
//@{
/// Time in nanoseconds
#define SI_TIME_ns(VALUETYPE)    SI_TIME_s(VALUETYPE)::apply_ratio< ::std::nano>::type
/// Time in microseconds
#define SI_TIME_us(VALUETYPE)    SI_TIME_s(VALUETYPE)::apply_ratio< ::std::micro>::type
/// Time in milliseconds
#define SI_TIME_ms(VALUETYPE)    SI_TIME_s(VALUETYPE)::apply_ratio< ::std::milli>::type
/// Time in seconds
#define SI_TIME_s(VALUETYPE)     ::si::SIValue<VALUETYPE, ::std::ratio<1>, 0, 0, 1, 0>
/// Time in minutes
#define SI_TIME_min(VALUETYPE)   SI_TIME_s(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
/// Time in hours
#define SI_TIME_h(VALUETYPE)     SI_TIME_min(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
/// Time in days
#define SI_TIME_d(VALUETYPE)     SI_TIME_h(VALUETYPE)::apply_ratio< ::std::ratio<24>>::type
/// Time in weeks
#define SI_TIME_w(VALUETYPE)     SI_TIME_d(VALUETYPE)::apply_ratio< ::std::ratio<7>>::type
//@}


/** @name Electric Current
 * Base unit: ampere<br>
 * Symbol: A
 */
//@{
/// Electric current in amperes
#define SI_ELECTRICCURRENT_A(VALUETYPE) ::si::SIValue<VALUETYPE, ::std::ratio<1>, 0, 0, 0, 1>
//@}


/** @name Area
 * Derived unit: square meter<br>
 * Symbol: m²
 */
//@{
/// Area in square centimeters
#define SI_AREA_cm2(VALUETYPE)   ::si::multiplication<SI_LENGTH_cm(VALUETYPE), SI_LENGTH_cm(VALUETYPE)>::type
/// Area in square meters
#define SI_AREA_m2(VALUETYPE)    ::si::multiplication<SI_LENGTH_m(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type
/// Area in square kilometers
#define SI_AREA_km2(VALUETYPE)   ::si::multiplication<SI_LENGTH_km(VALUETYPE), SI_LENGTH_km(VALUETYPE)>::type
//@}


/** @name Frequency
 * Derived unit: hertz<br>
 * Symbol: Hz
 */
//@{
/// Frequency in hertz
#define SI_FREQUENCY_Hz(VALUETYPE)  ::si::division<VALUETYPE, SI_TIME_s(VALUETYPE)>::type
//@}


/** @name Volume
 * Derived unit: cubic meter<br>
 * Symbol: m³
 */
//@{
/// Volume in cubic meters
#define SI_VOLUME_m3(VALUETYPE)  ::si::multiplication<SI_AREA_m2(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type
//@}


/** @name Speed, Velocity
 * Derived unit: meters per second<br>
 * Symbol: m/s
 */
//@{
/// Speed in meters per second
#define SI_SPEED_m_s(VALUETYPE)  ::si::division<SI_LENGTH_m(VALUETYPE), SI_TIME_s(VALUETYPE)>::type
//@}


/** @name Acceleration
 * Derived unit: meters per second squared<br>
 * Symbol: m/s²
 */
//@{
/// Acceleration in meters per second squared
#define SI_ACCELERATION_m_s2(VALUETYPE)  ::si::division<SI_SPEED_m_s(VALUETYPE), SI_TIME_s(VALUETYPE)>::type
//@}


/** @name Electric Charge
 * Derived unit: coulomb<br>
 * Symbol: C<br>
 * Definition: 1C = 1A . 1s
 */
//@{
/// Electric charge in coulombs
#define SI_ELECTRICCHARGE_C(VALUETYPE)  ::si::multiplication<SI_ELECTRICCURRENT_A(VALUETYPE), SI_TIME_s(VALUETYPE)>::type
//@}


#endif /* SI_DEFS_HPP_ */
