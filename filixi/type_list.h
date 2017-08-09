#ifndef TYPE_LIST_FILIXI_TYPE_LIST_H_
#define TYPE_LIST_FILIXI_TYPE_LIST_H_

#include <cstddef>

#include <tuple>
#include <type_traits>
#include <utility>

namespace type_list {
template <class... Args>
struct TypeList {
  static constexpr auto Size() {
    return sizeof...(Args);
  }
};

template <class... Args>
auto IndexSequenceOf(TypeList<Args...>) {
  return std::index_sequence_for<Args...>();
}

template <size_t pos, class... Args>
std::tuple_element_t<pos, std::tuple<Args...>> TypeAt(TypeList<Args...>);

template <size_t pos, class T>
struct TypeAtS;

template <size_t pos, class... Args>
struct TypeAtS<pos, TypeList<Args...>> {
  using Type = decltype(TypeAt<pos>(TypeList<Args...>{}));
};

template <size_t pos, class List>
using TypeAtT = typename TypeAtS<pos, List>::Type;

//! Algo #1 Reverse
template <class... Args, size_t... index>
auto ReverseImpl(TypeList<Args...> list, std::index_sequence<index...>) {
  return TypeList<decltype(TypeAt<sizeof...(index) - index - 1>(list))...>{};
}

template <class... Args>
auto Reverse(TypeList<Args...> list) {
  return ReverseImpl(list, IndexSequenceOf(list));
}

//! Algo #2 AnyOf AllOf NoneOf
template <class T, class... Args>
constexpr bool AnyOf(TypeList<Args...> list) {
  return (std::is_same<T, Args>::value || ...);
}

template <class T, class... Args>
constexpr bool AllOf(TypeList<Args...> list) {
  return (std::is_same<T, Args>::value && ...);
}

template <class T, class... Args>
constexpr bool NoneOf(TypeList<Args...> list) {
  return !AnyOf<T>(list);
}

//! Algo #3 Merge
template <class... Args1, class... Args2>
auto Merge(TypeList<Args1...>, TypeList<Args2...>) {
  return TypeList<Args1..., Args2...>{};
}

//! Algo #4 SubList
template <std::ptrdiff_t off_pos, class... Args, size_t... index>
auto SubListImpl(TypeList<Args...> list, std::index_sequence<index...>) {
  return TypeList<decltype(TypeAt<index + off_pos>(list))...>{};
}

template <size_t first, size_t last, class... Args>
auto SubList(TypeList<Args...> list) {
  static_assert(first < last);
  static_assert(last - first < list.Size());
  return SubListImpl<first>(list, std::make_index_sequence<last-first>());
}

//! Algo #5 Remove
template <size_t pos, class... Args>
auto Remove(TypeList<Args...> list) {
  constexpr auto count = list.Size();
  static_assert(pos < count);

  if constexpr (pos == 0)
    return SubList<1, count>(list);
  else if constexpr (pos == count-1)
    return SubList<0, pos>(list);
  else
    return Merge(SubList<0, pos>(list), SubList<pos+1, count>(list));
}

} // namespace type_list

#endif // TYPE_LIST_FILIXI_TYPE_LIST_H_
