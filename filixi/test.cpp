
#include "type_list.h"

int main() {
  using namespace type_list;

  TypeList<int, int &, double> list;
  TypeList<> empty_list;

  { // Algo #1 Reverse
    static_assert(IsSame(Reverse(list), TypeList<double, int &, int>()));
    static_assert(IsSame(Reverse(empty_list), empty_list));
  }

  { // Algo #2 AnyOf AllOf NoneOf IsSame
    static_assert(AnyOf<int &>(list));
    static_assert(!AllOf<int>(list));
    static_assert(NoneOf<float>(list));

    static_assert(!AnyOf<int &>(empty_list));
    static_assert(AllOf<int>(empty_list));
    static_assert(NoneOf<float>(empty_list));

    static_assert(!AnyOf<int &>(TypeList<int>()));
    static_assert(AllOf<int>(TypeList<int, int>()));
    static_assert(!NoneOf<double>(list));

    static_assert(!IsSame(TypeList<int>(), list));
    static_assert(IsSame(TypeList<int, int &, double>(), list));
    static_assert(IsSame(empty_list, empty_list));
    static_assert(!IsSame(empty_list, list));
  }

  { // Algo #3 Merge
    auto merge = Merge(TypeList<int, int &>(), TypeList<double>());
    static_assert(IsSame(merge, list));
    static_assert(IsSame(Merge(empty_list, empty_list), empty_list));
  }

  { // Algo #4 Slice
    static_assert(IsSame(Slice<1, 3>(list), TypeList<int&, double>()));
    static_assert(IsSame(Slice<0, 3>(list), list));
    static_assert(IsSame(Slice<1, 1>(list), empty_list));
    static_assert(IsSame(Slice<0, 0>(empty_list), empty_list));
  }

  { // Algo #5 Remove
    static_assert(IsSame(Remove<0>(list), TypeList<int &, double>()));
    static_assert(IsSame(Remove<1>(list), TypeList<int, double>()));
    static_assert(IsSame(Remove<2>(list), TypeList<int, int &>()));
  }

  return 0;
}
