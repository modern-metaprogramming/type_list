#include "type_list.h"

using namespace std;

using namespace typelist;

int main()
{
	using c_s_i = type_list<char, short, int>;
	static_assert(false == typelist::empty<c_s_i>::value, "Meta function empty error!");
	static_assert(3u == typelist::size<c_s_i>::value, "Meta function size error!");
	
	using c_s_i_l = push_back<c_s_i, long>::type;
	static_assert(is_same<c_s_i_l, type_list<char, short, int, long>>::value, "Meta function push_back error!");
	
	using s_i_l = pop_front<c_s_i_l>::type;
	static_assert(is_same<s_i_l, type_list<short, int, long>>::value, "Meta function pop_front error!");
		
	using i_s_i_l = push_front<s_i_l, int>::type;
	static_assert(is_same<i_s_i_l, type_list<int, short, int, long>>::value, "Meta function push_front error!");
	
	using c_i_l = typelist::remove<c_s_i_l, short>::type;
	static_assert(is_same<c_i_l, type_list<char, int, long>>::value, "Meta function remove error!");
	
	using l_i_c = reverse<c_i_l>::type;
	static_assert(is_same<l_i_c, type_list<long, int, char>>::value, "Meta function reverse error!");
	
	using i_s_l = unique<i_s_i_l>::type;
	static_assert(is_same<i_s_l, type_list<int, short, long>>::value, "Meta function unique error!");
	
	using b = typelist::begin<c_s_i>::type;
	static_assert(typelist::equal<b, type_list_iterator<c_s_i, position<0>>>::value, "Meta function begin error!");
	static_assert(is_same<deref<b>::type, char>::value, "Meta function deref error!");
	
	using b2 = typelist::next<b, 2>::type;
	static_assert(is_same<deref<b2>::type, int>::value, "Meta function next error!"); 
	
	using e = typelist::end<c_s_i>::type;
	static_assert(typelist::equal<e, type_list_iterator<c_s_i, position<3>>>::value, "Meta function end error!");
	
	using p = typelist::find<c_s_i_l, int>::type;
	static_assert(typelist::equal<p, type_list_iterator<c_s_i_l, position<2>>>::value, "Meta function find error!");

}