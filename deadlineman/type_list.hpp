#pragma once 

//#include <tuple>

template <typename ...Args>
struct type_list;

using null_list = type_list<>;

template <typename ...Args>
struct length;

template <typename ...Args>
struct length<type_list<Args...>>
{
    enum { value = sizeof...(Args) };
};

template <typename List>
struct head;

template <typename T, typename ...Args>
struct head<type_list<T, Args...>>
{
    using type = T;
};

template <>
struct head<null_list>
{

};

template <typename List>
struct tail;

template <typename T, typename ...Args>
struct tail<type_list<T, Args...>>
{
    using type = type_list<Args...>;
};

template <>
struct tail<null_list>
{
    using type = null_list;
};

template <typename List>
struct last;

template <typename T, typename ...Args>
struct last<type_list<T, Args...>>
{
    using type = typename last<type_list<Args...>>::type;
};

template <typename T>
struct last<type_list<T>>
{
    using type = T;
};

template <typename List, typename FList = null_list>
struct front;

template <typename T, typename ...Args, typename ...TArgs>
struct front<type_list<T, Args...>, type_list<TArgs...>>
{
    using type = typename front<type_list<Args...>, type_list<TArgs..., T>>::type;
};

template <typename T, typename ...Args>
struct front<type_list<T>, type_list<Args...>>
{
    using type = type_list<Args...>;
};

/**
 * push & pop front & back
 */

template <typename List, typename ...Args>
struct push_front;

template <template <typename ...> class List, typename ...Args, typename ...TArgs>
struct push_front<List<Args...>, TArgs...>
{
    using type = List<TArgs..., Args...>;
};

template <typename List, typename ...Args>
using push_front_t = typename push_front<List, Args...>::type;

template <typename List, typename ...TArgs>
struct push_back;

template <template <typename ...> class List, typename ...Args, typename ...TArgs>
struct push_back<List<Args...>, TArgs...>
{
    using type = List<Args..., TArgs...>;
};

template <typename List, typename ...Args>
using push_back_t = typename push_back<List, Args...>::type;

//template <typename List, size_t N>
//struct pop_back;
//
//template <template <typename ...> class List, typename T, typename ...Args, size_t N>
//struct pop_back<List<Args..., T>, N>
//{
//    using type = typename pop_back<List<Args...>, N - 1>::type;
//};
//
//template <template <typename ...> class List, typename ...Args>
//struct pop_back<List<Args...>, 0>
//{
//    using type = List<Args...>;
//};
//
//template <size_t N>
//struct pop_back<null_list, N>
//{
//    using type = null_list;
//};
//
//template <>
//struct pop_back<null_list, 0>
//{
//    using type = null_list;
//};

template <typename List, size_t N = 1>
struct pop_back
{
    using type = typename pop_back<typename front<List>::type, N - 1>::type;
};

template <typename List>
struct pop_back<List, 0>
{
    using type = List;
};

template <size_t N>
struct pop_back<null_list, N>
{
    using type = null_list;
};

template <>
struct pop_back<null_list, 0>
{
    using type = null_list;
};

template <typename List, size_t N = 1>
using pop_back_t = typename pop_back<List, N>::type;


/**
 * concat
 */

template <typename L, typename R>
struct concat;

template <typename ...LList, typename ...RList>
struct concat<type_list<LList...>, type_list<RList...>>
{
    using type = type_list<LList..., RList...>;
};

template <typename T, typename ...List>
struct concat<T, type_list<List...>>
{
    using type = type_list<T, List...>;
};

template <typename T, typename ...List>
struct concat<type_list<List...>, T>
{
    using type = type_list<List..., T>;
};

template <typename L, typename R>
using concat_t = typename concat<L, R>::type;


/**
 * slice
 */

template <typename FList, typename BList, size_t Size>
struct slice_back;

template <template <typename ...> class List, typename ...Args, typename BList, size_t Size>
struct slice_back<List<Args...>, BList, Size>
{
    using type = typename slice_back<
        List<Args..., typename head<BList>::type>,
        typename tail<BList>::type,
        Size-1
    >::type;
};

template <template <typename ...> class List, typename ...Args, size_t Size>
struct slice_back<List<Args...>, null_list, Size>
{
    using type = List<Args...>;
};

template <template <typename ...> class List, typename ...Args, typename BList>
struct slice_back<List<Args...>, BList, 0>
{
    using type = List<Args...>;
};

template <template <typename ...> class List, typename ...Args>
struct slice_back<List<Args...>, null_list, 0>
{
    using type = List<Args...>;
};

template <typename List, size_t Start, size_t Size, size_t Count>
struct slice_front
{
    using type = typename slice_front<typename tail<List>::type, Start, Size, Count+1>::type;
};

template <typename List, size_t Start, size_t Size>
struct slice_front<List, Start, Size, Start>
{
    using type = typename slice_back<null_list, List, Size>::type;
};

template <typename List, int Start, size_t Size>
struct slice
{
    using type = typename slice_front<
        List,
        (Start < 0 ? (Start < -length<List>::value ? 0 : Start + length<List>::value) : (Start >= length<List>::value ? 0 : Start)),
        (Start >= length<List>::value ? 0 : Size),
        0
    >::type;
};

template <typename List, int Start, size_t Size = 1>
using slice_t = typename slice<List, Start, Size>::type;

/**
 * at
 */

template <typename List, int I>
using at = typename head<slice_t<List, I>>::type;

/**
 * index_of
 */

template <typename List, typename T, size_t C = 0>
struct index_of;

template <template <typename ...> class List, typename H, typename ...Args, typename T, size_t C>
struct index_of<List<H, Args...>, T, C>
{
    enum { value = index_of<List<Args...>, T, C+1>::value };
};

template <template <typename ...> class List, typename ...Args, typename T, size_t C>
struct index_of<List<T, Args...>, T, C>
{
    enum { value = C };
};

template <template <typename ...> class List, typename T, size_t C>
struct index_of<List<>, T, C>
{
    enum { value = -1 };
};

/**
 * insert
 */

template <typename List, int I, typename ...Args>
struct insert {
    using type = concat_t<
        slice_t<List, 0, (I > length<List>::value ? length<List>::value
            : (I >= 0 ? I
               : (I < -length<List>::value ? 0 : I + length<List>::value)
        ))>,
        push_front_t<
            slice_t<List, I, (I < 0
                ? (I < -length<List>::value ? length<List>::value : I + length<List>::value)
                : (I > length<List>::value ? I : length<List>::value - I)
            )>,
            Args...>
    >;
};

template <typename List, int I, typename ...Args>
using insert_t = typename insert<List, I, Args...>::type;

/**
 * remove
 */

template <typename List, int I>
struct remove_at
{
    using type = concat_t<
        slice_t<List, 0, (I < 0
            ? (I < -length<List>::value ? length<List>::value : I + length<List>::value)
            : I 
        )>,
        typename tail<slice_t<List, I, (I < -length<List>::value ? 0 : length<List>::value)>>::type
    >;
};

template <typename List, typename T>
struct remove_impl
{
    constexpr static int index = index_of<List, T>::value;
    using type = typename remove_at<List, (index < 0 ? length<List>::value : index)>::type;
};

template <typename List, typename T>
using remove_t = typename remove_impl<List, T>::type;

template <typename List, typename T>
struct remove_all
{
    constexpr static int index = index_of<List, T>::value;
    using type = concat_t<
        slice_t<List, 0, (index < 0 ? length<List>::value : index)>,
        typename remove_all<typename tail<slice_t<List, index, (index < 0 ? 0 : length<List>::value - index)>>::type, T>::type
    >;
};

template <typename T>
struct remove_all<null_list, T>
{
    using type = null_list;
};

/**
 * replace
 */

template <typename List, typename T, typename R>
struct replace
{
    constexpr static int index = index_of<List, T>::value;
    using type = concat_t<
        slice_t<List, 0, (index < 0 ? length<List>::value : index)>,
        push_front_t<typename tail<slice_t<List, index, (index < 0 ? 0 : length<List>::value)>>::type, R>
    >;
};

template <typename List, typename T, typename R>
using replace_t = typename replace<List, T, R>::type;

//template <typename List, typename T, typename R>
//struct replace_all
//{
//    constexpr static int index = index_of<List, T>::value;
//    using type = concat_t<
//        typename push_back<
//            slice_t<List, 0, (index < 0 ? length<List>::value : index)>,
//            R
//        >::type,
//        typename replace_all<slice_t<List, (index < 0 ? 0 : index + 1), (index < 0 ? 0 : length<List>::value)>, T, R>::type
//    >;
//};
//
//template <typename T, typename R>
//struct replace_all<null_list, T, R>
//{
//    using type = null_list;
//};

template <typename List, typename T, typename R>
struct replace_all;

template <template <typename ...> class List, typename ...Args, typename H, typename T, typename R>
struct replace_all<List<H, Args...>, T, R>
{
    using type = concat_t<H, typename replace_all<List<Args...>, T, R>::type>;
};

template <template <typename ...> class List, typename ...Args, typename R, typename H>
struct replace_all<List<H, Args...>, H, R>
{
    using type = concat_t<R, typename replace_all<List<Args...>, H, R>::type >;
};

template <typename T, typename R>
struct replace_all<null_list, T, R>
{
    using type = null_list;
};

/**
 * replace
 */

template <typename List>
struct reverse;

template <template <typename ...> class List, typename T, typename ...Args>
struct reverse<List<T, Args...> >
{
    using type = concat_t<typename reverse<List<Args...>>::type, T>;
};

template <>
struct reverse<null_list>
{
    using type = null_list;
};

template <typename List>
using reverse_t = typename reverse<List>::type;

/**
 * deduplication
 */

template <typename List>
struct deduplication;

template <template <typename ...> class List, typename T, typename ...Args>
struct deduplication<List<T, Args...>>
{
    using type = concat_t<T, typename deduplication<remove_t<List<Args...>, T>>::type>;
};

template <>
struct deduplication<null_list>
{
    using type = null_list;
};

template <typename List>
using deduplication_t = typename deduplication<List>::type;
