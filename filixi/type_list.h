#ifndef _TYPE_LIST_FILIXI_TYPE_LIST_H_
#define _TYPE_LIST_FILIXI_TYPE_LIST_H_

#include <cstddef>

#include <tuple>
#include <type_traits>
#include <utility>

namespace type_list {
template <class... Args>
struct TypeList {
  constexpr TypeList() = default;

  static constexpr auto Size() {
    return sizeof...(Args);
  }
};

template <class... Args>
constexpr auto IndexSequenceOf(TypeList<Args...>) {
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

template <class... Args, size_t... index>
constexpr auto ReverseImpl(TypeList<Args...> list,
                           std::index_sequence<index...>) {
  return TypeList<TypeAtT<list.Size() - index - 1, decltype(list)>...>{};
}

//! Algo #1 Reverse
template <class... Args>
constexpr auto Reverse(TypeList<Args...> list) {
  return ReverseImpl(list, IndexSequenceOf(list));
}

//! Algo #2 AnyOf
template <class T, class... Args>
constexpr bool AnyOf(TypeList<Args...> list) {
  return (std::is_same<T, Args>::value || ...);
}

//! Algo #2 AllOf
template <class T, class... Args>
constexpr bool AllOf(TypeList<Args...> list) {
  return (std::is_same<T, Args>::value && ...);
}

//! Algo #2 IsSame
template <class... Args1, class... Args2>
constexpr bool IsSame(TypeList<Args1...> list1, TypeList<Args2...> list2) {
  if constexpr (sizeof...(Args1) != sizeof...(Args2))
    return false;
  else
    return (std::is_same<Args1, Args2>::value && ...);
}

//! Algo #2 NoneOf
template <class T, class... Args>
constexpr bool NoneOf(TypeList<Args...> list) {
  return !AnyOf<T>(list);
}

//! Algo #3 Merge
template <class... Args1, class... Args2>
constexpr auto Merge(TypeList<Args1...>, TypeList<Args2...>) {
  return TypeList<Args1..., Args2...>{};
}

template <std::ptrdiff_t off_pos, class... Args, size_t... index>
constexpr auto SliceImpl(TypeList<Args...> list,
                         std::index_sequence<index...>) {
  return TypeList<TypeAtT<index + off_pos, decltype(list)>...>{};
}

//! Algo #4 Slice
template <size_t first, size_t last, class... Args>
constexpr auto Slice(TypeList<Args...> list) {
  static_assert(first <= last);
  static_assert(last - first <= list.Size());
  return SliceImpl<first>(list, std::make_index_sequence<last-first>());
}

//! Algo #5 Remove
template <size_t pos, class... Args>
constexpr auto Remove(TypeList<Args...> list) {
  constexpr auto count = list.Size();
  static_assert(pos < count);

  if constexpr (pos == 0)
    return Slice<1, count>(list);
  else if constexpr (pos == count-1)
    return Slice<0, pos>(list);
  else
    return Merge(Slice<0, pos>(list), Slice<pos+1, count>(list));
}

} // namespace type_list

#endif // _TYPE_LIST_FILIXI_TYPE_LIST_H_
