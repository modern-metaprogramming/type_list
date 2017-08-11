#include "type_list.hpp"

template <typename T>
struct is_int {
	enum { value = false };
};

template <>
struct is_int<int> {
	enum { value = true };
};

template <typename>
struct to_int {
	using type = int;
};

int main() {
	using list = type_list<int, float, long, double, short, char>;
	static_assert(std::is_same<list::head, int>::value, "");
	static_assert(std::is_same<list::tail, type_list<float, long, double, short, char>>::value, "");
	static_assert(std::is_same<list::init, type_list<int, float, long, double, short>>::value, "");
	static_assert(std::is_same<list::last, char>::value, "");
	static_assert(std::is_same<list::slice<0, 2>, type_list<int, float>>::value, "");
	static_assert(std::is_same<list::slice<-5, 3>, type_list<float, long>>::value, "");
	static_assert(std::is_same<list::slice<-3, -1>, type_list<double, short>>::value, "");
	static_assert(std::is_same<list::slice<-2>, type_list<short, char>>::value, "");
	static_assert(std::is_same<list::concat<list>, type_list<int, float, long, double, short, char, int, float, long, double, short, char>>::value, "");
	static_assert(std::is_same<type_list<int>::template concat<type_list<int>, type_list<int>>, type_list<int, int, int>>::value, "");
	static_assert(std::is_same<list::concat<type_list<>>, list>::value, "");
	static_assert(std::is_same<type_list<>::concat<list>, list>::value, "");
	static_assert(std::is_same<list::push_back<float, int>, type_list<int, float, long, double, short, char, float, int>>::value, "");
	static_assert(std::is_same<list::push_front<float, int>, type_list<float, int, int, float, long, double, short, char>>::value, "");
	static_assert(std::is_same<list::insert<2, float, int>, type_list<int, float, float, int, long, double, short, char>>::value, "");
	static_assert(std::is_same<list::remove_at<2>, type_list<int, float, double, short, char>>::value, "");
	static_assert(std::is_same<list::remove_at<-1>, type_list<int, float, long, double, short>>::value, "");
	static_assert(std::is_same<list::remove_at<0>, type_list<float, long, double, short, char>>::value, "");
	static_assert(std::is_same<list::at<2>, long>::value, "");
	static_assert(list::find<int> == 0, "");
	static_assert(list::find<long> == 2, "");
	static_assert(list::find<unsigned> == -1, "");
	static_assert(type_list<int, int, float, int>::find_if<is_int> == 0, "");
	static_assert(type_list<float, float, int, float>::find_if<is_int> == 2, "");
	static_assert(std::is_same<list::remove_all<unsigned>, list>::value, "");
	static_assert(std::is_same<type_list<int, int, float, int>::remove_all<int>, type_list<float>>::value, "");
	static_assert(std::is_same<type_list<int, int, float, int>::remove_if<is_int>, type_list<float>>::value, "");
	static_assert(type_list<float, float, float, float>::all<float>, "");
	static_assert(!type_list<float, float, int, float>::all<float>, "");
	static_assert(type_list<int, int, int, int>::all_if<is_int>, "");
	static_assert(type_list<float, float, int, float>::any<int>, "");
	static_assert(!type_list<float, float, float, float>::any<int>, "");
	static_assert(type_list<float, float, int, float>::any_if<is_int>, "");
	static_assert(type_list<float, float, int, float>::replace<float, int>::all<int>, "");
	static_assert(type_list<float, float, int, float>::replace<int, float>::all<float>, "");
	static_assert(type_list<float, float, int, float>::map<to_int>::all<int>, "");
}
