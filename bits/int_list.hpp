#ifndef INT_LIST_HPP_
#define INT_LIST_HPP_



template <int... Values>
struct int_list;

template <int HeadValue, int... TailValues>
struct int_list<HeadValue, TailValues...> {
	static const int head = HeadValue;
	typedef int_list<TailValues...> tail;
	static const bool empty = false;

	template <int NewHeadValue>
	struct with_head {
		typedef int_list<NewHeadValue, HeadValue, TailValues...> type;
	};
};

template <>
struct int_list<> {
	static const bool empty = true;

	template <int NewHeadValue>
	struct with_head {
		typedef int_list<NewHeadValue> type;
	};
};



template <class IntList1, class IntList2>
struct int_list_add;

template <int HeadValue1, int... TailValues1,
          int HeadValue2, int... TailValues2>
struct int_list_add<int_list<HeadValue1, TailValues1...>, int_list<HeadValue2, TailValues2...>> {
	static const int head = HeadValue1 + HeadValue2;
	typedef typename int_list_add<int_list<TailValues1...>, int_list<TailValues2...>>::list tail;

	typedef typename tail::template with_head<head>::type list;

	static const bool empty = false;
};

template <>
struct int_list_add<int_list<>, int_list<>> {
	typedef int_list<> list;

	static const bool empty = true;
};



#endif /* INT_LIST_HPP_ */
