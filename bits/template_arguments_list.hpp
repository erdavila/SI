#ifndef TYPE_LIST_HPP_
#define TYPE_LIST_HPP_


/*
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
*/




template <typename Type, Type... Values>
struct values_list;

template <typename Type, Type Head, Type... TailValues>
struct values_list<Type, Head, TailValues...> {
	typedef Type type;
	static const Type head = Head;
	typedef values_list<Type, TailValues...> tail;

	static const bool empty = false;

	template <Type NewHead>
	struct with_head {
		typedef values_list<Type, NewHead, Head, TailValues...> value;
	};
};

template <typename Type>
struct values_list<Type> {
	typedef Type type;

	static const bool empty = true;

	template <Type NewHead>
	struct with_head {
		typedef values_list<Type, NewHead> value;
	};
};



#endif /* TYPE_LIST_HPP_ */
