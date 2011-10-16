#ifndef UNITS_LIST_HPP_
#define UNITS_LIST_HPP_



template <int... Units>
struct units_list;

template <int HeadUnit, int... TailUnits>
struct units_list<HeadUnit, TailUnits...> {
	static const int head = HeadUnit;
	typedef units_list<TailUnits...> tail;
	static const bool empty = false;

	template <int NewHead>
	struct with_head {
		typedef units_list<NewHead, HeadUnit, TailUnits...> type;
	};
};

template <>
struct units_list<> {
	static const bool empty = true;

	template <int NewHead>
	struct with_head {
		typedef units_list<NewHead> type;
	};
};



#endif /* UNITS_LIST_HPP_ */
