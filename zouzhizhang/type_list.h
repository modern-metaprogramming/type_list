
#include <cstddef>
#include <type_traits>
#include <utility>


template<class ...args_t>
struct type_node_t;
template<class ...args_t>
struct type_list_t;


template<class T, class ...next_t>
struct type_node_t<T, next_t...>
{
  using type = T;
  using next = type_node_t<next_t...>;
};
template<>
struct type_node_t<>
{
};

template<class iter_t, size_t I>
struct type_list_iter_move_next_t
{
  using iter = typename type_list_iter_move_next_t<typename iter_t::next, I - 1>::iter;
};
template<class iter_t>
struct type_list_iter_move_next_t<iter_t, 0>
{
  using iter = iter_t;
};

template<size_t I, class list_t, class ...other_t>
struct type_list_get_list_t;

template<bool found, size_t I, class list_t, class ...other_t>
struct type_list_get_type_t
{
  using iter = typename type_list_iter_move_next_t<typename list_t::begin, I>::iter;
  using type = typename iter::type;
};
template<size_t I, class list_t, class ...other_t>
struct type_list_get_type_t<false, I, list_t, other_t...>
{
  using type = typename type_list_get_list_t<I - list_t::size, other_t...>::type;
};

template<size_t I, class list_t, class ...other_t>
struct type_list_get_list_t
{
  using type = typename type_list_get_type_t<(I < list_t::size), I, list_t, other_t...>::type;
};

template<class ...list_t>
struct type_list_get_t
{
  template<size_t I>
  struct get
  {
    using type = typename type_list_get_list_t<I, list_t...>::type;
  };
};

template<class get_t, size_t B, class T>
struct type_list_builder_t;
template<class get_t, size_t B, size_t ...I>
struct type_list_builder_t<get_t, B, std::integer_sequence<size_t, I...>>
{
  using type = type_list_t<typename get_t::template get<B + I>::type...>;
};

template<class list_t, size_t B, size_t S>
struct type_list_slice_t
{
  using type = typename type_list_builder_t<type_list_get_t<list_t>, B, std::make_index_sequence<S>>::type;
};

template<class ...list_t>
struct type_list_cat_size_t;
template<class list_t, class ...other_list_t>
struct type_list_cat_size_t<list_t, other_list_t...>
{
  static constexpr size_t size = list_t::size + type_list_cat_size_t<other_list_t...>::size;
};
template<>
struct type_list_cat_size_t<>
{
  static constexpr size_t size = 0;
};

template<class ...list_t>
struct type_list_cat_t
{
  static constexpr size_t size = type_list_cat_size_t<list_t...>::size;
  using type = typename type_list_builder_t<type_list_get_t<list_t...>, 0, std::make_index_sequence<size>>::type;
};

template<class ...args_t>
struct type_list_t
{
  using begin = type_node_t<args_t...>;
  using end = type_node_t<>;
  static constexpr size_t size = sizeof ...(args_t);
  static constexpr bool empty = false;
  template<size_t I> using get = typename type_list_get_t<type_list_t<args_t...>>::template get<I>::type;
  using front = get<0>;
  using back = get<size - 1>;
  template<class T> using push_back = type_list_t<args_t..., T>;
  using pop_back = typename type_list_slice_t<type_list_t<args_t...>, 0, size - 1>::type;
  template<class T> using push_front = type_list_t<T, args_t...>;
  using pop_front = typename type_list_slice_t<type_list_t<args_t...>, 1, size - 1>::type;
  template<class other_t> using cat = typename type_list_cat_t<type_list_t<args_t...>, other_t>::type;
  template<size_t B, size_t S> using slice = typename type_list_slice_t<type_list_t<args_t...>, B, S>::type;
  template<size_t I, class T> using insert = typename type_list_cat_t<slice<0, I>, type_list_t<T>, slice<I, size - I>>::type;
  template<size_t I> using remove = typename type_list_cat_t<slice<0, I>, slice<I + 1, size - I - 1>>::type;
  template<template<class...> class T> using transform = type_list_t<typename T<args_t>::type...>;
};
template<>
struct type_list_t<>
{
  using begin = type_node_t<>;
  using end = type_node_t<>;
  static constexpr size_t size = 0;
  static constexpr bool empty = true;
  template<class T> using push_back = type_list_t<T>;
  template<class T> using push_front = type_list_t<T>;
  template<class other_t> using cat = other_t;
  template<size_t B, size_t S> using slice = typename type_list_slice_t<type_list_t<>, B, S>::type;
  template<size_t I, class T> using insert = typename type_list_slice_t<type_list_t<T>, I, 0>::type;
  template<template<class...> class T> using transform = type_list_t<>;
};
