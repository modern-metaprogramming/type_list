#include <utility>


namespace sf_code {
	template<typename...T> struct sf_type_list {};
	template<typename T, typename..._Args>struct push_front;
	template<typename T, typename..._Args>struct push_back;
	template<typename T, typename..._Args>struct first;
	template<typename..._Args>struct back;
	template<typename T, typename..._Args>struct pop_front;
	template<typename..._Args>struct pop_back;
	template<size_t N, typename..._Args>struct left;
	template<size_t N, typename..._Args>struct right;
	template<typename A, typename B> struct merge;
	template<size_t N, typename..._Args> struct get;
	template<size_t _Beg, size_t _Len, typename..._Args> struct erase;
	template<size_t _Pos, typename T, typename..._Args> struct replace;
	template<size_t _Pos, typename T, typename..._Args> struct insert;
	template<size_t _Beg, size_t _Len, typename..._Args> struct mid;

	template<typename T> struct sf_type_list<T>
	{
		typedef T type;
	};

	template<typename T, typename..._Args>
	struct push_front<T, sf_type_list<_Args...>>
	{
		typedef sf_type_list<T, _Args...> type;
	};

	template<typename T, typename..._Args>
	struct push_back<T, sf_type_list<_Args...>>
	{
		typedef sf_type_list<_Args..., T> type;
	};

	template<typename T, typename..._Args>
	struct first<sf_type_list<T, _Args...>>
	{
		typedef T type;
	};

	template<typename T, typename..._Args>
	struct pop_front<sf_type_list<T, _Args...>>
	{
		typedef sf_type_list<_Args...> type;
	};

	template<size_t _Curr, size_t _Dest, typename _Curr_t, typename..._Args>struct _left;

	template<size_t _Curr, size_t _Dest, typename _Curr_t, typename..._Args>
	struct _left<_Curr, _Dest, _Curr_t, sf_type_list<_Args...>>
	{
		typedef typename _left<_Curr + 1, _Dest, typename push_back<typename first<sf_type_list<_Args...>>::type, _Curr_t>::type, typename pop_front<sf_type_list<_Args...>>::type>::type type;
	};

	template<size_t _Curr, typename _Curr_t, typename..._Args>
	struct _left<_Curr, _Curr, _Curr_t, sf_type_list<_Args...>>
	{
		typedef _Curr_t type;
	};

	template<size_t N, typename..._Args>
	struct left<N, sf_type_list<_Args...>>
	{
		typedef typename _left<1, N, sf_type_list<typename first<sf_type_list<_Args...>>::type>, typename pop_front<sf_type_list<_Args...>>::type>::type type;
	};

	template<size_t _Curr, size_t _Dest, typename..._Args>struct _right;

	template<size_t _Curr, size_t _Dest, typename..._Args>
	struct _right<_Curr, _Dest, sf_type_list<_Args...>>
	{
		typedef typename _right<_Curr - 1, _Dest, typename pop_front<sf_type_list<_Args...>>::type>::type type;
	};

	template<size_t N, typename..._Args>
	struct _right<N, N, sf_type_list<_Args...>>
	{
		typedef sf_type_list<_Args...> type;
	};

	template<size_t N, typename..._Args>
	struct right<N, sf_type_list<_Args...>>
	{
		typedef typename _right<sizeof...(_Args), N, sf_type_list<_Args...>>::type type;
	};

	template<typename..._Args>
	struct pop_back<sf_type_list<_Args...>>
	{
		typedef typename left<sizeof...(_Args)-1, sf_type_list<_Args...>>::type type;
	};

	template<typename..._First, typename..._Second>
	struct merge<sf_type_list<_First...>, sf_type_list<_Second...>>
	{
		typedef sf_type_list<_First..., _Second...> type;
	};

	template<size_t N, typename..._Args>
	struct get<N, sf_type_list<_Args...>>
	{
		typedef typename first<typename left< sizeof...(_Args)-N, sf_type_list<_Args...>>::type>::type type;
	};

	template<size_t _Beg, size_t _Len, typename..._Args>
	struct erase<_Beg, _Len, sf_type_list<_Args...>>
	{
		typedef typename merge<typename left<_Beg, sf_type_list<_Args...>>::type, typename right<sizeof...(_Args)-_Beg - _Len, sf_type_list<_Args...>>::type>::type type;
	};

	template<size_t _Pos, typename T, typename..._Args>
	struct replace<_Pos, T, sf_type_list<_Args...>>
	{
		typedef typename merge<typename push_back<T, typename left<_Pos, sf_type_list<_Args...>>::type>::type, typename right<sizeof...(_Args)-_Pos - 1, sf_type_list<_Args...>>::type>::type type;
	};

	template<typename..._Args>
	struct back<sf_type_list<_Args...>>
	{
		typedef typename right<1, sf_type_list<_Args...>>::type::type type;
	};

	template<size_t _Pos, typename T, typename..._Args>
	struct insert<_Pos, T, sf_type_list<_Args...>>
	{
		typedef typename merge<typename push_back<T, typename left<_Pos, sf_type_list<_Args...>>::type>::type, typename right<sizeof...(_Args)-_Pos, sf_type_list<_Args...>>::type >::type type;
	};

	template<size_t _Beg, size_t _Len, typename..._Args>
	struct mid<_Beg, _Len, sf_type_list<_Args...>>
	{
		typedef typename right<_Len, typename left<_Beg + _Len, sf_type_list<_Args...>>::type>::type type;
	};

}
