#ifndef SCALES_HPP_
#define SCALES_HPP_


/*
 * A scale is defined by a fraction.
 */



template <unsigned int Multiplier, unsigned int Divider>
struct scale {
	static const unsigned int multiplier = Multiplier;
	static const unsigned int divider    = Divider;
};



namespace scales {



template <unsigned int A, unsigned int B>
struct gcd {
	static const unsigned int value = gcd<B, A % B>::value;
};

template <unsigned int A>
struct gcd<A, 0> {
	static const unsigned int value = A;
};



template <typename Scale>
struct simplify {
	static const unsigned int gcd = gcd<Scale::multiplier, Scale::divider>::value;
	struct value {
		static const unsigned int multiplier = Scale::multiplier / gcd;
		static const unsigned int divider    = Scale::divider    / gcd;
	};
};




template <typename... Scales>
struct multiply;

template <typename FirstScale, typename... Scales>
struct multiply<FirstScale, Scales...> {
	typedef typename multiply<Scales...>::value _Aux;

	typedef scale<FirstScale::multiplier * _Aux::multiplier,
	              FirstScale::divider    * _Aux::divider
	             >
	        _Aux2;

	typedef typename simplify<_Aux2>::value value;
};

template <>
struct multiply<> {
	struct value {
		static const int multiplier = 1;
		static const int divider    = 1;
	};
};



} /* namespace scales */



#endif /* SCALES_HPP_ */
