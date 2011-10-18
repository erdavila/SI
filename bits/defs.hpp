#ifndef DEFS_HPP_
#define DEFS_HPP_


#define SI_LENGTH_mm(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::with_ratio< ::std::milli>::type
#define SI_LENGTH_cm(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::with_ratio< ::std::centi>::type
#define SI_LENGTH_m(VALUETYPE)   ::si::SIValue<VALUETYPE, ::std::ratio<1>, 1, 0, 0, 0>
#define SI_LENGTH_km(VALUETYPE)  SI_LENGTH_m(VALUETYPE)::with_ratio< ::std::kilo>::type

#define SI_TIME_ns(VALUETYPE)    SI_TIME_s(VALUETYPE)::with_ratio< ::std::nano>::type
#define SI_TIME_us(VALUETYPE)    SI_TIME_s(VALUETYPE)::with_ratio< ::std::micro>::type
#define SI_TIME_ms(VALUETYPE)    SI_TIME_s(VALUETYPE)::with_ratio< ::std::milli>::type
#define SI_TIME_s(VALUETYPE)     ::si::SIValue<VALUETYPE, ::std::ratio<1>, 0, 0, 1, 0>
#define SI_TIME_min(VALUETYPE)   SI_TIME_s(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
#define SI_TIME_h(VALUETYPE)     SI_TIME_min(VALUETYPE)::apply_ratio< ::std::ratio<60>>::type
#define SI_TIME_d(VALUETYPE)     SI_TIME_h(VALUETYPE)::apply_ratio< ::std::ratio<24>>::type

#define SI_AREA_cm2(VALUETYPE)   ::si::multiplication<SI_LENGTH_cm(VALUETYPE), SI_LENGTH_cm(VALUETYPE)>::type
#define SI_AREA_m2(VALUETYPE)    ::si::multiplication<SI_LENGTH_m(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type
#define SI_AREA_km2(VALUETYPE)   ::si::multiplication<SI_LENGTH_km(VALUETYPE), SI_LENGTH_km(VALUETYPE)>::type

#define SI_FREQUENCY_Hz(VALUETYPE)  ::si::division<VALUETYPE, SI_TIME_s(VALUETYPE)>::type

#define SI_SPEED_m_s(VALUETYPE)  ::si::division<SI_LENGTH_m(VALUETYPE), SI_TIME_s(VALUETYPE)>::type

#define SI_ACCELERATION_m_s2(VALUETYPE)  ::si::division<SI_SPEED_m_s(VALUETYPE), SI_TIME_s(VALUETYPE)>::type

#define SI_VOLUME_m3(VALUETYPE)  ::si::multiplication<SI_AREA_m2(VALUETYPE), SI_LENGTH_m(VALUETYPE)>::type

#define SI_ELECTRICCURRENT_A(VALUETYPE) ::si::SIValue<VALUETYPE, ::std::ratio<1>, 0, 0, 0, 1>

#define SI_ELECTRICCHARGE_C(VALUETYPE)  ::si::multiplication<SI_ELECTRICCURRENT_A(VALUETYPE), SI_TIME_s(VALUETYPE)>::type


#endif /* DEFS_HPP_ */
