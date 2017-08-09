#include <type_traits>

template <typename... Ts>
struct type_list
{
    static const size_t numberOfTypes = sizeof...(Ts);
};

template <typename, typename>
struct push;

template <typename T, typename... Ts>
struct push<T, type_list<Ts...>>
{
    using type = type_list<Ts..., T>;
};

template <typename>
struct pop;

template <>
struct pop<type_list<>>
{
    using type = type_list<>;
};

template <typename T, typename... Ts>
struct pop<type_list<T, Ts...>>
{
    using type = type_list<Ts...>;
};

template <typename, typename>
struct is_first_type
{
    static const bool value = false;
};


template <typename T, typename T_head, typename... Ts>
struct is_first_type<T, type_list<T_head, Ts...>>
{
    static const bool value = std::is_same<T, T_head>::value;
};


template <typename T, typename type_list, bool is_first = is_first_type<T, type_list>::value >
struct has_type
{
    static const bool value = true;
};

template <typename T, typename T_head, typename... Ts>
struct has_type<T, type_list<T_head, Ts...>, false>
{
    static const bool value = has_type<T, type_list<Ts...>>::value;
};

template <typename T>
struct has_type<T, type_list<>, false>
{
    static const bool value = false;
};

template <typename T, typename type_list, bool is_first = is_first_type<T, type_list>::value>
struct remove_head_type_if
{
    using type = typename pop<type_list>::type;
};

template <typename T, typename... Ts>
struct remove_head_type_if<T, type_list<Ts...>, false>
{
    using type = type_list<Ts...>;
};
