namespace additions {


void differentUnits() {
	LengthInMeters len = 3;
	AreaInMeters   area = 4;
	CANT_COMPILE(
		len + area;
	);
}


void sameScaleSameUnits() {
	LengthInMeters len1 = 3;
	LengthInMeters len2 = 4;
	LengthInMeters sum = len1 + len2;
	assert(sum.value == 7);
}


void test() {
	differentUnits();
	sameScaleSameUnits();
}

} /* namespace additions */
