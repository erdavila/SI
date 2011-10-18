#ifndef DIVISIONS_HPP_
#define DIVISIONS_HPP_


namespace divisions {


void scalar() {
	{
		const Time_s time_s(12);
		assert(time_s / 3 == Time_s(4));
		assert(36 / time_s == Frequency_Hz(3));
		assert(time_s / 1.6 == TimeDbl_s(7.5));
		assert(18.0 / time_s == FrequencyDbl_Hz(1.5));
		assert(18.0 / time_s != Frequency_Hz(1));
		assert(18   / time_s == FrequencyDbl_Hz(1)); // Truncated!
	}

	{
		Time_s time_s(12);
		time_s /= 3; // 12s / 3 = 4s
		assert(time_s.value == 4);
	}

	{
		Time_s time_s(12);
		time_s /= 1.6; // Truncated: int(12s / 1.6) = int(7.5s) = 7s
		assert(time_s.value == 7);
	}
}


void derived() {
	{
		const Length_m len12m(12);
		const Time_s time3s(3);
		const TimeDbl_s time1_6s(1.6);

		assert(len12m / time1_6s == SpeedDbl_m_s(7.5)); // 12m / 1.6s = 7.5m/s

		const Speed_m_s speed4m_s = len12m / time3s; // 12m / 3s = 4m/s
		assert(speed4m_s == Speed_m_s(4));
		assert(speed4m_s / Time_s(2) == Acceleration_m_s2(2)); // 4m/s / 2s = 2m/s²
	}

	{
		const Length_m     len4m(4);
		const LengthDbl_m  lenDbl4m(4);
		const Length_cm    len160cm(160);

		assert(len4m    / len160cm == 2); // Truncated: int(4m / 160cm) = int(2.5) = 2
		assert(lenDbl4m / len160cm == 2.5); // 4m / 160cm = 2.5
	}

	{
		// Electric charge: 1*C = 1*A*s
		const ElectricCharge_C  eCharge8C(8);
		const ElectricCurrent_A eCurr2A(2);
		const Time_s            time4s(4);

		assert(eCharge8C / eCurr2A == time4s); // 8C / 2A = 4s
		assert(eCharge8C / time4s == eCurr2A); // 8C / 4s = 2A
	}

	{
		Length_m len(7);
		CANT_COMPILE(
			len /= len;
		);

		CANT_COMPILE(
			len /= Length_km(1);
		);
	}
}


void test() {
	scalar();
	derived();
}


} /* namespace divisions */


#endif /* DIVISIONS_HPP_ */
