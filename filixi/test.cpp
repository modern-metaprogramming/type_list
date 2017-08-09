
#include "type_list.h"

int main() {
  using namespace type_list;

  TypeList<int, char, float, uint8_t, uint64_t, int &, double &&> list;

  // Algo #1 Reverse
  auto reverse = Reverse(list);
  static_assert(std::is_same<TypeAtT<2, decltype(reverse)>, uint64_t>::value);

  // Algo #2 AnyOf AllOf NoneOf
  static_assert(AnyOf<double &&>(list));
  static_assert(!AllOf<uint8_t>(list));
  static_assert(NoneOf<uint32_t>(list));

  // Algo #3 Merge
  auto merge = Merge(TypeList<int, double>{}, TypeList<float, char>{});
  static_assert(std::is_same<TypeAtT<2, decltype(merge)>, float>::value);

  // Algo #4 SubList
  auto range = SubList<1, 2>(list); // TypeList<char, float>
  static_assert(std::is_same<TypeAtT<0, decltype(range)>, char>::value);

  // Algo #5 Remove
  auto sub = Remove<6>(list); // removes double &&
  static_assert(NoneOf<double &&>(sub));

  return 0;
}
