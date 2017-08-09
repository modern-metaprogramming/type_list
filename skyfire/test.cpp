#include "type_list.h"
#include <typeinfo>
#include <iostream>

int main()
{
	using namespace std;
	typedef sf_code::sf_type_list<int, double, short, long, long long> t1;
	typedef sf_code::sf_type_list<unsigned, unsigned char, float> t2;
	typedef sf_code::push_front<char, t1>::type t3;
	typedef sf_code::push_back<char, t1>::type t4;
	typedef sf_code::merge<t1, t2>::type t5;
	typedef sf_code::left<3, t1>::type t6;
	typedef sf_code::right<3, t1>::type t7;
	typedef sf_code::erase<2, 2, t1>::type t8;
	typedef sf_code::replace<3, char, t1>::type t9;
	typedef sf_code::insert<3, char, t1>::type t10;
	typedef sf_code::first<t1>::type t11;
	typedef sf_code::back<t1>::type t12;
	typedef sf_code::pop_front<t1>::type t13;
	typedef sf_code::pop_back<t1>::type t14;
	typedef sf_code::mid<2, 2, t1>::type t15;



	cout << typeid(t1).name() << endl;
	cout << typeid(t2).name() << endl;
	cout << typeid(t3).name() << endl;
	cout << typeid(t4).name() << endl;
	cout << typeid(t5).name() << endl;
	cout << typeid(t6).name() << endl;
	cout << typeid(t7).name() << endl;
	cout << typeid(t8).name() << endl;
	cout << typeid(t9).name() << endl;
	cout << typeid(t10).name() << endl;
	cout << typeid(t11).name() << endl;
	cout << typeid(t12).name() << endl;
	cout << typeid(t13).name() << endl;
	cout << typeid(t14).name() << endl;
	cout << typeid(t15).name() << endl;


	static_assert(is_same<t3, sf_code::sf_type_list<char, int, double, short, long, long long>>::value, "no");
	static_assert(is_same<t4, sf_code::sf_type_list<int, double, short, long, long long, char>>::value, "no");
	static_assert(is_same<t5, sf_code::sf_type_list<int, double, short, long, long long, unsigned, unsigned char, float>>::value, "no");
	static_assert(is_same<t6, sf_code::sf_type_list<int, double, short>>::value, "no");
	static_assert(is_same<t7, sf_code::sf_type_list<short, long, long long>>::value, "no");
	static_assert(is_same<t8, sf_code::sf_type_list<int, double, long long>>::value, "no");
	static_assert(is_same<t9, sf_code::sf_type_list<int, double, short, char, long long>>::value, "no");
	static_assert(is_same<t10, sf_code::sf_type_list<int, double, short, char, long, long long>>::value, "no");
	static_assert(is_same<t11, int>::value, "no");
	static_assert(is_same<t12, long long>::value, "no");
	static_assert(is_same<t13, sf_code::sf_type_list<double, short, long, long long>>::value, "no");
	static_assert(is_same<t14, sf_code::sf_type_list<int, double, short, long>>::value, "no");
	static_assert(is_same<t15, sf_code::sf_type_list<short, long>>::value, "no");

	return 0;
}

