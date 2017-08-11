#pragma once

#include <type_traits>
#include <tuple>

namespace details {
	struct list_invalid {
		enum {
			is_valid = false,
			value = false,
		};
	};

	template <size_t S, size_t SC, size_t E, size_t EC, typename List>
	struct list_slice {
		using type = typename list_slice<S, SC+1, E, EC, typename List::tail>::type;
	};

	template <size_t S, size_t E, size_t EC, typename List>
	struct list_slice<S, S, E, EC, List> {
		using type = typename list_slice<S, S, E, EC-1, typename List::init>::type;
	};

	template <size_t S, size_t E, typename List>
	struct list_slice<S, S, E, E, List> {
		using type = List;
	};

	template <typename List, typename ...Args>
	struct list_init;

	template <template <typename ...> class List, typename ...TArgs, typename H, typename ...Args>
	struct list_init<List<TArgs...>, H, Args...> {
		using type = typename list_init<List<TArgs..., H>, Args...>::type;
	};

	template <template <typename ...> class List, typename ...TArgs, typename H>
	struct list_init<List<TArgs...>, H> {
		using type = List<TArgs...>;
	};

	template <template <typename ...> class List>
	struct list_init<List<>> {
		using type = List<>;
	};

	template <typename List>
	struct list_tail;

	template <template <typename ...> class List, typename H, typename ...Args>
	struct list_tail<List<H, Args...>> {
		using type = List<Args...>;
	};

	template <template <typename ...> class List>
	struct list_tail<List<>> {
		using type = List<>;
	};

	template <typename ...Args>
	struct list_head;

	template <typename H, typename ...Args>
	struct list_head<H, Args...> {
		using type = H;
	};

	template <>
	struct list_head<> {
		using type = list_invalid;
	};

	template <typename ...Args>
	struct list_last;

	template <typename H, typename ...Args>
	struct list_last<H, Args...> {
		using type = typename list_last<Args...>::type;
	};

	template <typename T>
	struct list_last<T> {
		using type = T;
	};

	template <>
	struct list_last<> {
		using type = list_invalid;
	};

	template <typename ListL, typename ListR>
	struct list_concat_helper;

	template <template <typename ...> class T, typename ...Args, typename ...TArgs>
	struct list_concat_helper<T<Args...>, T<TArgs...>> {
		using type = T<Args..., TArgs...>;
	};

	template <typename ListL, typename ...ListRs>
	struct list_concat;

	template <typename ListL, typename H, typename ...ListRs>
	struct list_concat<ListL, H, ListRs...> {
		using type = typename list_concat<typename list_concat_helper<ListL, H>::type, ListRs...>::type;
	};

	template <typename ListL>
	struct list_concat<ListL> {
		using type = ListL;
	};

	template <typename List, int I>
	struct list_remove_at {
		using type = typename List::template slice<0, I>::template concat<List::template slice<I + 1>>;
	};

	template <typename List>
	struct list_remove_at<List, -1> {
		using type = typename List::init;
	};

	template <typename List>
	struct list_remove_at<List, 0> {
		using type = typename List::tail;
	};

	template <typename List, size_t M, size_t N, typename T>
	struct list_find;

	template <template <typename ...> class List, typename H, typename ...Args, size_t M, size_t N, typename T>
	struct list_find<List<H, Args...>, M, N, T> {
		enum { value = list_find<List<Args...>, M, N+1, T>::value };
	};

	template <template <typename ...> class List, typename ...Args, size_t M, size_t N, typename T>
	struct list_find<List<T, Args...>, M, N, T> {
		enum { value = N };
	};

	template <typename List, size_t M, typename T>
	struct list_find<List, M, M, T> {
		enum { value = -1 };
	};

	template <typename List, size_t M, size_t N, template <typename ...> class P>
	struct list_find_if;

	template <template <typename ...> class List, typename H, typename ...Args, size_t M, size_t N, template <typename ...> class P>
	struct list_find_if<List<H, Args...>, M, N, P> {
		enum { value = P<H>::value ? N : list_find_if<List<Args...>, M, N+1, P>::value };
	};

	template <typename List, size_t M, template <typename ...> class P>
	struct list_find_if<List, M, M, P> {
		enum { value = -1 };
	};

	template <typename List, int N>
	struct list_remove {
		using type = typename std::conditional<
			N < 0,
			List,
			typename List::template remove_at<N>
		>::type;
	};

	template <typename List, int N, typename T>
	struct list_remove_all {
		using ntype = typename list_remove_at<List, N>::type;
		using type = typename list_remove_all<ntype, ntype::template find<T>, T>::type;
	};

	template <typename List, typename T>
	struct list_remove_all<List, -1, T> {
		using type = List;
	};

	template <typename List, typename T, template <typename ...> class P>
	struct list_remove_if;

	template <typename List, template <typename ...> class T, typename H, typename ...Args, template <typename...> class P>
	struct list_remove_if<List, T<H, Args...>, P> {
		using type = typename std::conditional<
			P<H>::value,
			typename List::template concat<typename list_remove_if<List, T<Args...>, P>::type>,
			typename List::template concat<T<H>, typename list_remove_if<List, T<Args...>, P>::type>
		>::type;
	};

	template <typename List, template <typename ...> class T, template <typename...> class P>
	struct list_remove_if<List, T<>, P> {
		using type = List;
	};

	template <typename List, typename T, template <typename ...> class P>
	struct list_map;

	template <typename List, template <typename ...> class T, typename H, typename ...Args, template <typename ...> class P>
	struct list_map<List, T<H, Args...>, P> {
		using type = typename list_map<
			typename List::template push_back<typename P<H>::type>,
			T<Args...>,
			P
		>::type;
	};

	template <typename List, template <typename ...> class T, template <typename ...> class P>
	struct list_map<List, T<>, P> {
		using type = List;
	};

	template <typename S, typename T, typename U>
	struct type_convert_pred_impl {
		using type = U;
	};

	template <typename S, typename T>
	struct type_convert_pred_impl<S, T, S> {
		using type = T;
	};

	template <typename S, typename T>
	struct type_convert_pred {
		template <typename U>
		using type = type_convert_pred_impl<S, T, U>;
	};
}

template <typename ...Args>
struct type_list {
	template <template <typename ...Args> class Tuple>
	using from_tuple = ::type_list<Args...>;
	using to_tuple = std::tuple<Args...>;

	enum { is_valid = true };
	enum { size = sizeof...(Args) };

	using head = typename details::list_head<Args...>::type;
	using tail = typename details::list_tail<type_list>::type;
	using init = typename details::list_init<::type_list<>, Args...>::type;
	using last = typename details::list_last<Args...>::type;

	template <int S, int E = size+1>
	using slice = typename details::list_slice<
		(S < 0 ? S + size : S),
		0,
		(E < 0 ? E + size : (E > size ? size : E)),
		size,
		type_list
	>::type;

	template <typename ...List>
	using concat = typename details::list_concat<type_list, List...>::type;

	template <typename ...TArgs>
	using push_back = ::type_list<Args..., TArgs...>;

	template <typename ...TArgs>
	using push_front = ::type_list<TArgs..., Args...>;

	template <int I>
	using at = typename slice<I>::head;

	template <int I, typename ...TArgs>
	using insert = typename slice<0, I>::template push_back<TArgs...>::template concat<slice<I>>;

	template <int I>
	using remove_at = typename details::list_remove_at<type_list, I>::type;

	template <typename T>
	constexpr static int find = details::list_find<type_list, size, 0, T>::value;
	
	template <template <typename ...> class P>
	constexpr static int find_if = details::list_find_if<type_list, size, 0, P>::value;

	template <typename T>
	using remove = typename details::list_remove<type_list, find<T>>::type;

	template <typename T>
	using remove_all = typename details::list_remove_all<type_list, find<T>, T>::type;

	template <template <typename ...> class P>
	using remove_if = typename details::list_remove_if<::type_list<>, type_list, P>::type;

	template <typename T>
	constexpr static auto all = std::is_same<::type_list<>, remove_all<T>>::value;

	template <template <typename ...> class P>
	constexpr static auto all_if = std::is_same<::type_list<>, remove_if<P>>::value;

	template <typename T>
	constexpr static auto any = !std::is_same<type_list, remove<T>>::value;

	template <template <typename ...> class P>
	constexpr static auto any_if = !std::is_same<type_list, remove_if<P>>::value;

	template <template <typename ...> class P>
	using map = typename details::list_map<::type_list<>, type_list, P>::type;

	template <typename S, typename T>
	using replace = map<typename details::type_convert_pred<S, T>::type>;
};
