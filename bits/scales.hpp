#ifndef SCALES_HPP_
#define SCALES_HPP_


/*
 * A scale is defined by a fraction.
 */


typedef unsigned long long ullong;



template <ullong A, ullong B>
struct gcd {
	static const ullong value = gcd<B, A % B>::value;
};

template <ullong A>
struct gcd<A, 0> {
	static const ullong value = A;
};



template <ullong A, ullong B>
struct lcm {
	static const ullong value = A * B / gcd<A, B>::value;
};








template <unsigned int Multiplier, unsigned int Divider>
struct scale {
	static const ullong gcd = gcd<Multiplier, Divider>::value;

	static const ullong multiplier = Multiplier / gcd;
	static const ullong divider    = Divider    / gcd;
};



namespace scales {


// Pre-declaration
template <typename... Scales>
struct multiply;

// Template specialization for a list of scales with at least one item.
template <typename FirstScale, typename... Scales>
struct multiply<FirstScale, Scales...> {
	typedef typename multiply<Scales...>::value _Rest;

	// Force "cross-simplification"
	typedef scale<FirstScale::multiplier,      _Rest::divider> _Aux1;
	typedef scale<     _Rest::multiplier, FirstScale::divider> _Aux2;

	typedef scale<_Aux1::multiplier * _Aux2::multiplier,
	              _Aux1::divider    * _Aux2::divider>
	        value;
};

// Template specialization for an empty list of scales.
template <>
struct multiply<> {
	struct value {
		static const ullong multiplier = 1;
		static const ullong divider    = 1;
	};
};



} /* namespace scales */



#endif /* SCALES_HPP_ */
