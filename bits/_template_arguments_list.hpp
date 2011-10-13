#ifndef TYPE_LIST_HPP_
#define TYPE_LIST_HPP_


template <typename... Types>
struct types_list;


template <typename Head, typename... TailTypes>
struct types_list<Head, TailTypes...> {
	typedef Head                     head;
	typedef types_list<TailTypes...> tail;
	static const bool empty = false;
};

template <>
struct types_list<> {
	static const bool empty = true;
};



template <typename Head, typename Tail>
struct _make_types_list;

template <typename Head, typename... TailTypes>
struct _make_types_list<Head, types_list<TailTypes...>> {
	typedef types_list<Head, TailTypes...> list;
};



#endif /* TYPE_LIST_HPP_ */
