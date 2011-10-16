#ifndef COMPARISONS_HPP_
#define COMPARISONS_HPP_


namespace comparisons {


#define assertEQ(V1, V2)   assert(V1 == V2); assert(!(V1 != V2));
#define assertNE(V1, V2)   assert(V1 != V2); assert(!(V1 == V2));

#define assertLE(V1, V2)   assert(V1 <= V2); assert(!(V1 >  V2));
#define assertGT(V1, V2)   assert(V1 >  V2); assert(!(V1 <= V2));

#define assertGE(V1, V2)   assert(V1 >= V2); assert(!(V1 <  V2));
#define assertLT(V1, V2)   assert(V1 <  V2); assert(!(V1 >= V2));

#define assertEqual(V1, V2)               \
	assertEQ(V1, V2); assertEQ(V2, V1);   \
	assertLE(V1, V2); assertLE(V2, V1);   \
	assertGE(V1, V2); assertGE(V2, V1)

#define assertLess(V1, V2)                \
	assertNE(V1, V2); assertNE(V2, V1);   \
	assertLT(V1, V2); assertGT(V2, V1);   \
	assertLE(V1, V2); assertGE(V2, V1)

#define assertSame(V) assertEqual(V, V)


void test() {
	const Length_m len2000m(2000);
	const Length_m len2048m(2048);

	const Length_km    len2km(2);
	const LengthDbl_km len2_048km(2.048);

	assertSame(len2000m);
	assertSame(len2048m);
	assertSame(len2km);
	assertSame(len2_048km);

	assertEqual(len2000m, len2km);
	assertEqual(len2048m, len2_048km);

	assertLess(len2000m, len2048m);
	assertLess(len2000m, len2_048km);

	assertLess(len2km, len2048m);
	assertLess(len2km, len2_048km);
}


#undef assertSame
#undef assertLess
#undef assertEqual
#undef assertLT
#undef assertGE
#undef assertGT
#undef assertLE
#undef assertNE
#undef assertEQ


} /* namespace comparisons */


#endif /* COMPARISONS_HPP_ */
