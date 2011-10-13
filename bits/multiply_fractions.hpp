#ifndef MULTIPLY_FRACTIONS_HPP_
#define MULTIPLY_FRACTIONS_HPP_



template <typename Fraction1, typename Fraction2>
struct multiply_fractions {
	struct value {
		enum {
			numerator   = Fraction1::numerator   * Fraction2::numerator,
			denominator = Fraction1::denominator * Fraction2::denominator,
		};
	};
};



#endif /* MULTIPLY_FRACTIONS_HPP_ */
