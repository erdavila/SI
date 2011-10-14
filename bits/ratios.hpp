#ifndef RATIOS_HPP_
#define RATIOS_HPP_


/*
 * Ratios are fractions.
 */



template <unsigned int Multiplier, unsigned int Divider>
struct ratio {
	static const unsigned int multiplier = Multiplier;
	static const unsigned int divider    = Divider;
};



namespace ratios {



template <unsigned int A, unsigned int B>
struct gcd {
	static const unsigned int value = gcd<B, A % B>::value;
};

template <unsigned int A>
struct gcd<A, 0> {
	static const unsigned int value = A;
};



template <typename Ratio>
struct simplify {
	static const unsigned int gcd = gcd<Ratio::multiplier, Ratio::divider>::value;
	struct value {
		static const unsigned int multiplier = Ratio::multiplier / gcd;
		static const unsigned int divider    = Ratio::divider    / gcd;
	};
};




template <typename... Ratios>
struct multiply;

template <typename FirstRatio, typename... Ratios>
struct multiply<FirstRatio, Ratios...> {
	typedef typename multiply<Ratios...>::value _Aux;

	typedef ratio<FirstRatio::multiplier * _Aux::multiplier,
	              FirstRatio::divider    * _Aux::divider
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



} /* namespace ratios */



#endif /* RATIOS_HPP_ */
