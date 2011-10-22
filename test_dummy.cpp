#include "si.hpp"

typedef SI_LENGTH_m(int) Length_m;

using namespace si::units;

void dummy_function() {
	__attribute__((unused)) Length_m len = 7*km;
}
