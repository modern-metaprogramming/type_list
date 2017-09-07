#include <type_traits>
#include <tuple>

template<class F, class... Args>
struct filter_helper
{
    template<class T>
    constexpr decltype(auto) operator>>(T t)
    {
        if constexpr (decltype(std::declval<F>()(t))::value)
        {
            return filter_helper<F, Args..., T>{};
        }
        else
        {
            return *this;
        }
    }

    template<template<class...>class Container>
    constexpr decltype(auto) result()
    {
        return Container<Args...>{};
    }
};

template<class F, class Result>
struct reduce_helper
{
    template<class T>
    constexpr decltype(auto) operator>>(T t)
    {
        using R = decltype(std::declval<F>()(std::declval<Result>(), t));
        return reduce_helper<F, R>{};
    }

    constexpr Result result()
    {
        return Result{};
    }
};

template<class F, class Result>
struct flat_map_helper
{
    template<class T>
    constexpr decltype(auto) operator>>(T t)
    {
        using R = decltype(Result{}.concat(std::declval<F>()(t)));
        return flat_map_helper<F, R>{};
    }

    constexpr Result result()
    {
        return Result{};
    }
};

template<class T, class... Args>
struct rest_helper
{
    template<template<class...>class Container>
    constexpr decltype(auto) result()
    {
        return Container<Args...>{};
    }
};

template<class... Args>
struct type_list
{
    constexpr decltype(auto) first()
    {
        return std::tuple_element_t<0, std::tuple<Args...>>{};
    }

    constexpr decltype(auto) rest()
    {
        return rest_helper<Args...>{}.template result<type_list>();
    }

    template<class T>
    constexpr decltype(auto) append(T)
    {
        return type_list<Args..., T>{};
    }

    template<class... Args2>
    constexpr decltype(auto) concat(type_list<Args2...>)
    {
        return type_list<Args..., Args2...>{};
    }

    template<class Fun>
    constexpr decltype(auto) filter(Fun f)
    {
        return (filter_helper<Fun>{} >> ... >> Args{}).template result<type_list>();
    }

    template<class Init, class Fun>
    constexpr decltype(auto) reduce(Init init, Fun f)
    {
        return (reduce_helper<Fun, Init>{} >> ... >> Args{}).result();
    }

    template<class Fun>
    constexpr decltype(auto) map(Fun f)
    {
        return type_list<decltype(f(Args{}))...>{};
    }

    template<class Fun>
    constexpr decltype(auto) flat_map(Fun f)
    {
        return (flat_map_helper<Fun, type_list<>>{} >> ... >> Args{}).result();
    }

    template<class Fun>
    constexpr decltype(auto) sort(Fun less)
    {
        if constexpr (sizeof...(Args) <= 1)
        {
            return *this;
        }
        else
        {
            auto pivot = this->first();
            auto left =
                rest()
                .filter([pivot, less](auto x)
            {
                return less(x, pivot);
            })
                .sort(less);
            auto right =
                rest()
                .filter([pivot, less](auto x)
            {
                return std::integral_constant<bool, !decltype(less(x, pivot))::value>{};
            })
                .sort(less);
            return left.append(pivot).concat(right);
        }
    }
};

template<class... Args>
constexpr std::true_type operator==(type_list<Args...>, type_list<Args...>)
{
    return{};
}

template<class... Args1, class... Args2>
constexpr std::false_type operator==(type_list<Args1...>, type_list<Args2...>)
{
    return{};
}

int main()
{
    //concat test
    type_list<int, double, float> list1;
    type_list<char, long, short> list2;
    type_list<int, double, float, char, long, short> list3;
    static_assert(list1.concat(list2) == list3);

    //filter test
    auto filterFun = [](auto a)
    { //filter function must return either true_type or false_type
        return std::integral_constant<bool, (sizeof(a) >= 2)>{};
    };
    type_list<int, double, float, long, short> list4;
    static_assert(list3.filter(filterFun) == list4);

    //map test
    auto mapFun = [](auto a)
    { //map all type to its pointer
        using R = decltype(a)*;
        return R{};
    };
    type_list<int*, double*, float*, long*, short*> list5;
    static_assert(list4.map(mapFun) == list5);

    //flat map test
    auto flatMapFun = [](auto a)
    {
        using T = decltype(a);
        return type_list<T, T*>{};
    };
    type_list<int, int*, double, double*, float, float*> list6;
    static_assert(list1.flat_map(flatMapFun) == list6);


    //sort test
    auto less = [](auto a, auto b)
    { //compare types by its size
        return std::integral_constant<bool, (sizeof(a) < sizeof(b))>{};
    };
    type_list<short, int, float, double, long> list7;
    static_assert(list4.sort(less) == list7);
}