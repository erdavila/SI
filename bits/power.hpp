#ifndef POWER_HPP_
#define POWER_HPP_



template <int base, unsigned int n>
struct power {
	enum {
		value = base * power<base, n-1>::value
	};
};

template <int base>
struct power<base, 0> {
	enum {
		value = 1
	};
};



template <bool negative, typename Fraction, int n>
struct power_fraction_impl {
	// Here, n is always positive
	struct value {
		enum {
			numerator   = power<Fraction::numerator  , n>::value,
			denominator = power<Fraction::denominator, n>::value,
		};
	};
};

template <typename Fraction, int n>
struct power_fraction_impl<true, Fraction, n> {
	struct inverse {
		enum {
			numerator   = Fraction::denominator,
			denominator = Fraction::numerator
		};
	};

	typedef typename power_fraction_impl<false, inverse, -n>::value value;
};

template <typename Fraction, int n>
struct power_fraction {
	typedef typename power_fraction_impl<(n < 0), Fraction, n>::value value;
};

template <typename Fraction>
struct power_fraction<Fraction, 0> {
	struct value {
		enum {
			numerator = 1,
			denominator = 1
		};
	};
};



#endif /* POWER_HPP_ */
