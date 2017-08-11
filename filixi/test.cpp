
// Copyright 2017 filixi

#include "type_list.h"

int main() {
  using namespace type_list;

  TypeList<int, int &, double> list;
  TypeList<> empty_list;

  { // Algo #1 Reverse
    static_assert(Reverse(list) == typelist<double, int &, int>);
    static_assert(empty_list.Reverse() == empty_list);
  }

  { // Algo #2 AnyOf AllOf NoneOf IsSame
    static_assert(list.AnyOf<int &>());
    static_assert(!AllOf<int>(list));
    static_assert(NoneOf<float>(list));

    static_assert(!AnyOf<int &>(empty_list));
    static_assert(empty_list.AllOf<int>());
    static_assert(NoneOf<float>(empty_list));

    static_assert(!AnyOf<int &>(typelist<int>));
    static_assert(AllOf<int>(typelist<int, int>));
    static_assert(!list.NoneOf<double>());

    static_assert(!IsSame(typelist<int>, list));
    static_assert(IsSame(typelist<int, int &, double>, list));
    static_assert(empty_list.IsSame(empty_list));
    static_assert(!empty_list.IsSame(list));
  }

  { // Algo #3 Merge
    static_assert(Merge(typelist<int, int &>, typelist<double>) == list);
    static_assert(empty_list.Merge(empty_list) == empty_list);
    static_assert(empty_list + list == list);
  }

  { // Algo #4 Slice
    static_assert(Slice<1, 3>(list) == typelist<int&, double>);
    static_assert(Slice<0, 3>(list) == list);
    static_assert(list.Slice<1, 1>() == empty_list);
    static_assert(empty_list.Slice<0, 0>() == empty_list);
  }

  { // Algo #5 Remove
    static_assert(Remove<0>(list) == typelist<int &, double>);
    static_assert(Remove<1>(list) == typelist<int, double>);
    static_assert(list.Remove<2>() == typelist<int, int &>);
  }

  return 0;
}
