// build by msvc 2017.15.3.preview(7)

#include <type_traits>

/*
 *	Get
 *	emplace_back
 *	pop_back
 *	has_element
 *	cat
 */
template<typename ..._Types>
struct TypeListVal
{
	constexpr static size_t size = sizeof...(_Types);
};
template<typename _Type>
struct TypeListVal<_Type>
{
	constexpr static  size_t size = 1;
	using type = _Type;
};
namespace detail
{
	template<size_t _Idx, size_t _Finder, typename _Type, typename ..._Types>
	constexpr auto _Get1(TypeListVal<_Type, _Types...> v)
	{
		if constexpr (_Idx == _Finder)
		{
			return TypeListVal<_Type>{};
		}
		else
		{
			return _Get1<_Idx + 1, _Finder>(TypeListVal<_Types...>{});
		}
	}

	template<size_t _Idx, typename ..._Types>
	constexpr auto Get1(TypeListVal<_Types...> v)
	{
		static_assert(_Idx < v.size);
		return _Get1<0, _Idx>(v);
	}
	template<size_t _Idx, typename _Ty>
	struct Get
	{
		using type_ = decltype(Get1<_Idx>(std::declval<_Ty>()));
		using type = typename type_::type;
	};
	template<size_t _Idx>
	struct Get<_Idx, TypeListVal<>>
	{

	};
}

template<size_t _Idx, typename _Ty>
using Get_t = typename detail::Get<_Idx, std::decay_t<_Ty>>::type;

template<typename _Ty, typename _New>
struct emplace_back;

template<typename... _Types, typename _New>
struct emplace_back<TypeListVal<_Types...>, _New>
{
	using type = TypeListVal<_Types..., _New>;
};

template<typename _Ty, typename _New>
using emplace_back_t = typename emplace_back<std::decay_t<_Ty>, _New>::type;

template<typename _NewList,typename _List>
struct _pop_back;

template<typename... _NewList, typename _Type1>
struct _pop_back<TypeListVal<_NewList...>, TypeListVal<_Type1>>
{
	using type = TypeListVal<_NewList...>;
};

template<typename... _NewList, typename _Type1, typename _Type2>
struct _pop_back<TypeListVal<_NewList...>, TypeListVal<_Type1, _Type2>>
{
	using type = TypeListVal<_NewList..., _Type1>;
};

template<typename... _NewList,typename _Type,typename ..._Types>
struct _pop_back<TypeListVal<_NewList...>, TypeListVal<_Type, _Types...>>
	: _pop_back<TypeListVal<_NewList..., _Type>, TypeListVal<_Types...>>
{
	
};

template<typename _Ty>
struct pop_back;

template<typename _Type>
struct pop_back<TypeListVal<_Type>>
{
	using type = TypeListVal<>;
};
template<>
struct pop_back<TypeListVal<>>
{
	using type = TypeListVal<>;
};

template<typename _Type,typename... _Types>
struct pop_back<TypeListVal<_Type, _Types...>>
{
	using type = typename _pop_back<TypeListVal<_Type>, TypeListVal<_Types...>>::type;
};
template<typename _Ty>
using pop_back_t = typename pop_back<std::decay_t<_Ty>>::type;

template<typename _Finder, size_t _Ix, typename _Ty>
constexpr bool _has_element(_Ty v)
{
	if constexpr(std::is_same_v<Get_t<_Ix, _Ty>, _Finder>)
		return true;
	else if constexpr(_Ix + 1 < _Ty::size)
	{
		return _has_element<_Finder, _Ix + 1>(v);
	}
	else
	{
		return false;
	}
}

template<typename _Finder,typename _Ty>
constexpr bool has_element(_Ty v)
{
	if constexpr(std::is_same_v<_Ty,TypeListVal<>>)
	{
		return false;
	}
	else
	{
		return _has_element<_Finder, 0, _Ty>(v);
	}
}

template<typename ..._Ty1,typename ..._Ty2>
constexpr auto cat(TypeListVal<_Ty1...>,TypeListVal<_Ty2...>)
{
	return TypeListVal<_Ty1..., _Ty2...>{};
}

int main()
{
	constexpr TypeListVal<> null;
	using null_type = std::decay_t<decltype(null)>;
	static_assert(std::is_same_v<pop_back_t<decltype(null)>, null_type>);
	static_assert(std::is_same_v<emplace_back_t<decltype(null), int>, TypeListVal<int>>);
	static_assert(!has_element<void>(null));
	static_assert(std::is_same_v<decltype(cat(null, null)), null_type>);

	constexpr TypeListVal<int&&> t1;
	static_assert(std::is_same_v< Get_t<0, decltype(t1)>, int&&>);
	static_assert(std::is_same_v< pop_back_t<decltype(t1)>, null_type>);
	static_assert(std::is_same_v< emplace_back_t<decltype(t1), int>, TypeListVal<int&&, int>>);
	static_assert(!has_element<int>(t1));
	static_assert(std::is_same_v<decltype(cat(t1, t1)), TypeListVal<int&&, int&&>>);

	constexpr TypeListVal<int, double&, const int&, float&&, const bool> a;

	static_assert(std::is_same_v<Get_t<0, decltype(a)>, int>);
	static_assert(std::is_same_v<Get_t<1, decltype(a)>, double&>);
	static_assert(std::is_same_v<Get_t<2, decltype(a)>, const int&>);
	static_assert(std::is_same_v<Get_t<3, decltype(a)>, float&&>);
	static_assert(std::is_same_v<Get_t<4, decltype(a)>, const bool>);

	constexpr emplace_back_t<decltype(a), long double&> b;
	static_assert(std::is_same_v<Get_t<0, decltype(b)>, int>);
	static_assert(std::is_same_v<Get_t<1, decltype(b)>, double&>);
	static_assert(std::is_same_v<Get_t<2, decltype(b)>, const int&>);
	static_assert(std::is_same_v<Get_t<3, decltype(b)>, float&&>);
	static_assert(std::is_same_v<Get_t<4, decltype(b)>, const bool>);
	static_assert(std::is_same_v<Get_t<5, decltype(b)>, long double&>);

	constexpr pop_back_t<decltype(b)> c;
	static_assert(std::is_same_v<decltype(a), decltype(c)>);


	static_assert(has_element<int>(a));
	static_assert(has_element<double&>(a));
	static_assert(!has_element<int&>(a));
	using type = decltype(cat(a, b));

	return 0;
}

