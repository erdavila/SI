#ifndef SI_HPP_
#define SI_HPP_

/**
 * @mainpage
 *
 * <a href="http://en.wikipedia.org/wiki/International_System_of_Units">SI
 * values</a> are implemented by the template class ::si::SIValue, but you
 * should not need to use the name of this in class in your code. Instead, use
 * the available @link bits/defs.hpp macro-declarators @endlink or create your
 * own derived SI unit types. See the examples below.
 *
 * To declare a variable which stores any distance in kilometers (0 km, 1 km,
 * 1.4 km, 0.03 km, 100 km, 456.654 km, etc.):
 * @code
 *    SI_LENGTH_km(double) distance;
 * @endcode
 *
 * A variable that stores a distance value in @b integer kilometers (0 km, 1 km,
 * 100 km, etc.; but @b not 1.4 km, 0.03 km, 456.654 km, etc.):
 * @code
 *    SI_LENGTH_km(int) distance;
 * @endcode
 *
 * If you want a unit that measures a displacement progress in minutes per
 * kilometer, you can create your own type:
 * @code
 *    typedef typename si::division<SI_TIME_min(int), SI_LENGTH_km(int)>::type DisplacementProgress;
 * @endcode
 *
 * The namespace si::units has a unit constant available for each
 * macro-declaration. They are useful to have more readable code. So, instead of
 * writing:
 * @code
 *    SI_VOLUME_m3(int) volume = SI_VOLUME_m3(int)(7);  // Initial volume is 7 m³
 *    volume += increasedVolume(SI_TIME_h(int)(2));  // Adds the volume increased in 2 hours.
 * @endcode
 * you can write:
 * @code
 *    using namespace si::units;
 *    SI_VOLUME_m3(int) volume = 7*m3;  // Initial volume is 7 m³
 *    volume += increasedVolume(2*h);  // Adds the volume increased in 2 hours.
 * @endcode
 *
 * You may want to define a unit which measures distance in miles. 1 mile is
 * equal to 1,609.344 meters. Since ratios can only be used with integer
 * numerator and denominator, the ratio 1609344:1000 must be used:
 * @code
 *    typedef typename SI_LENGTH_m(int)::apply_ratio<std::ratio<1609344, 1000>>::type LengthInMiles;
 * @endcode
 * or alternatively:
 * @code
 *    typedef typename SI_LENGTH_km(int)::apply_ratio<std::ratio<1609344, 1000000>>::type LengthInMiles;
 * @endcode
 *
 */

#include "bits/si_value.hpp"
#include "bits/defs.hpp"
#include "bits/units.hpp"
#include "bits/funcs.hpp"


#endif /* SI_HPP_ */
