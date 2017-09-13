
#include "type_list.h"
#include <string>

int main()
{
  using list1_t = type_list_t<int, long, char, double>;
  using list2_t = list1_t::push_back<std::string>;
  using list3_t = list2_t::push_front<list1_t>;
  using list4_t = list3_t::pop_front;
  using list5_t = list1_t::pop_front;
  using list6_t = list5_t::pop_back;
  using list7_t = type_list_t<int>;
  using list8_t = list7_t::cat<list5_t>;
  using list9_t = list1_t::slice<0, 3>;
  using list10_t = list1_t::slice<3, 1>;
  using list11_t = list10_t::pop_back;
  using list12_t = list9_t::cat<list11_t>;
  using list13_t = list11_t::cat<list9_t>;
  using list14_t = list1_t::remove<0>;
  using list15_t = list1_t::remove<1>;
  using list16_t = list1_t::remove<3>;
  using list17_t = list1_t::insert<0, float>;
  using list18_t = list1_t::insert<1, float>;
  using list19_t = list1_t::insert<4, float>;
  using list20_t = list1_t::transform<std::add_pointer>;
  using list21_t = list1_t::transform<std::add_rvalue_reference>;

  static_assert(list1_t::size == 4, "no");
  static_assert(std::is_same<list1_t::begin::type, int>::value, "no");
  static_assert(std::is_same<list1_t::begin::next::type, long>::value, "no");
  static_assert(std::is_same<list1_t::begin::next::next::next::next, list1_t::end>::value, "no");
  static_assert(std::is_same<list1_t::get<0>, int>::value, "no");
  static_assert(std::is_same<list1_t::get<1>, long>::value, "no");
  static_assert(std::is_same<list1_t::get<2>, char>::value, "no");
  static_assert(std::is_same<list1_t::get<3>, double>::value, "no");
  static_assert(std::is_same<list2_t::get<4>, std::string>::value, "no");
  static_assert(std::is_same<list3_t::get<1>, int>::value, "no");
  static_assert(std::is_same<list3_t::get<0>, list1_t>::value, "no");
  static_assert(std::is_same<list3_t::begin::type::begin::type, int>::value, "no");
  static_assert(std::is_same<list2_t, list4_t>::value, "no");
  static_assert(std::is_same<list2_t, list4_t>::value, "no");
  static_assert(list5_t::size == 3, "no");
  static_assert(std::is_same<list5_t::begin::type, long>::value, "no");
  static_assert(std::is_same<list5_t::begin::next::next::next, list5_t::end>::value, "no");
  static_assert(list6_t::size == 2, "no");
  static_assert(std::is_same<list6_t::begin::type, long>::value, "no");
  static_assert(std::is_same<list6_t::begin::next::next, list6_t::end>::value, "no");
  static_assert(list8_t::size == 4, "no");
  static_assert(std::is_same<list8_t, list1_t>::value, "no");
  static_assert(list9_t::size == 3, "no");
  static_assert(list10_t::size == 1, "no");
  static_assert(list10_t::pop_back::size == 0, "no");
  static_assert(std::is_same<list10_t, type_list_t<double>>::value, "no");
  static_assert(std::is_same<list11_t, type_list_t<>>::value, "no");
  static_assert(std::is_same<list12_t, list9_t>::value, "no");
  static_assert(std::is_same<list13_t, list12_t>::value, "no");
  static_assert(std::is_same<list14_t, type_list_t<long, char, double>>::value, "no");
  static_assert(std::is_same<list15_t, type_list_t<int, char, double>>::value, "no");
  static_assert(std::is_same<list16_t, type_list_t<int, long, char>>::value, "no");
  static_assert(std::is_same<list17_t, type_list_t<float, int, long, char, double>>::value, "no");
  static_assert(std::is_same<list18_t, type_list_t<int, float, long, char, double>>::value, "no");
  static_assert(std::is_same<list19_t, type_list_t<int, long, char, double, float>>::value, "no");
  static_assert(std::is_same<list20_t, type_list_t<int *, long *, char *, double *>>::value, "no");
  static_assert(std::is_same<list21_t, type_list_t<int &&, long &&, char &&, double &&>>::value, "no");

}

