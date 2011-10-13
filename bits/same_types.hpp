#ifndef SAME_TYPES_HPP_
#define SAME_TYPES_HPP_



template <typename T1, typename T2>
struct same_types {
	static const bool value = false;
};

template <typename T>
struct same_types<T, T> {
	static const bool value = true;
};



#endif /* SAME_TYPES_HPP_ */
