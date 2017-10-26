#pragma once

#include <iterator>
#include <type_traits>

namespace typelist{

template<std::size_t N_>
using position = std::integral_constant<std::size_t, N_>;

template<typename TypeList_, typename Position_>
struct type_list_iterator
{
    using category = std::bidirectional_iterator_tag;
    using container = TypeList_;
    static constexpr std::size_t value = Position_::value;
};

template<class Position_, std::ptrdiff_t N_>
struct plus;

template<std::size_t N1_, std::ptrdiff_t N2_>
struct plus<position<N1_>, N2_>
{
    using type = position<N1_ + N2_>;
};

template<class Position_, std::ptrdiff_t N_>
struct minus;

template<std::size_t N1_, std::ptrdiff_t N2_>
struct minus<position<N1_>, N2_>
{
    using type = position<N1_ - N2_>;
};

template<class Iterator1_, class Iterator2_>
struct equal
{
    static constexpr bool value = (std::is_same<typename Iterator1_::container, typename Iterator2_::container>::value
                                && Iterator1_::value == Iterator2_::value);
};

template<class Iterator_, std::ptrdiff_t N_ = 1>
struct next;

template<typename TypeList_, typename Position_, std::ptrdiff_t N_>
struct next<type_list_iterator<TypeList_, Position_>, N_>
{
    using type = type_list_iterator<TypeList_, typename plus<Position_, N_>::type>;
};

template<class Iterator_>
struct inc;

template<typename TypeList_, typename Position_>
struct inc<type_list_iterator<TypeList_, Position_>>: next<type_list_iterator<TypeList_, Position_>>
{
};

template<class Iterator_, std::ptrdiff_t N_ = 1>
struct prev;

template<typename TypeList_, typename Position_, std::ptrdiff_t N_>
struct prev<type_list_iterator<TypeList_, Position_>, N_>
{
    using type = type_list_iterator<TypeList_, typename minus<Position_, N_>::type>;
};

template<class Iterator_>
struct dec;

template<typename TypeList_, typename Position_>
struct dec<type_list_iterator<TypeList_, Position_>>: prev<type_list_iterator<TypeList_, Position_>>
{
};

template<typename... Types_>
struct type_list
{
};

using null_type_list = type_list<>;

template<typename TypeList_>
struct front;

template<typename Head_, typename... Tail_>
struct front<type_list<Head_, Tail_...>>
{
    using type = Head_;
};

template<typename TypeList_>
struct back;

template<typename Tail_, typename... Head_>
struct back<type_list<Head_..., Tail_>>
{
    using type = Tail_;
};

template<typename TypeList_>
struct empty
{
    static constexpr bool value = false;
};

template<>
struct empty<null_type_list>
{
    static constexpr bool value = true;
};

template<typename TypeList_>
struct size;

template<typename... Types_>
struct size<type_list<Types_...>>
{
    static constexpr std::size_t value = sizeof...(Types_);
};

template<typename TypeList_>
struct begin
{
    using type = type_list_iterator<TypeList_, position<0>>;
};

template<typename TypeList_>
struct end
{
    using type = type_list_iterator<TypeList_, position<size<TypeList_>::value>>;
};

template<typename TypeList_>
struct clear
{
    using type = null_type_list;
};

template<typename TypeList_, typename Type_>
struct push_back;

template<typename... Types_, typename Type_>
struct push_back<type_list<Types_...>, Type_>
{
    using type = type_list<Types_..., Type_>;
};

template<typename TypeList_>
struct pop_back;

template<typename Tail_, typename... Head_>
struct pop_back<type_list<Head_..., Tail_>>
{
    using type = type_list<Head_...>;
};

template<typename TypeList_, typename Type_>
struct push_front;

template<typename... Types_, typename Type_>
struct push_front<type_list<Types_...>, Type_>
{
    using type = type_list<Type_, Types_...>;
};

template<typename TypeList_>
struct pop_front;

template<typename Head_, typename... Tail_>
struct pop_front<type_list<Head_, Tail_...>>
{
    using type = type_list<Tail_...>;
};

template<typename TypeList1_, typename TypeList2_>
struct concat;

template<typename... Types1_, typename... Types2_>
struct concat<type_list<Types1_...>, type_list<Types2_...>>
{
    using type = type_list<Types1_..., Types2_...>;
};

template<typename TypeList_, typename Type_>
struct remove;

template<typename Type_>
struct remove<null_type_list, Type_>
{
    using type = null_type_list;
};

template<typename Head_, typename... Tail_, typename Type_>
struct remove<type_list<Head_, Tail_...>, Type_>
{
    using type = typename std::conditional<std::is_same<Head_, Type_>::value,
                                              typename remove<type_list<Tail_...>, Type_>::type,
                                           typename push_front<typename remove<type_list<Tail_...>, Type_>::type, Head_>::type>::type;
};

template<typename TypeList_>
struct reverse;

template<>
struct reverse<null_type_list>
{
    using type = null_type_list;
};

template<typename Head_, typename... Tail_>
struct reverse<type_list<Head_, Tail_...>>
{
    using type = typename push_back<typename reverse<type_list<Tail_...>>::type, Head_>::type;
};

template<typename TypeList_>
struct unique;

template<>
struct unique<null_type_list>
{
    using type = null_type_list;
};

template<typename Head_, typename... Tail_>
struct unique<type_list<Head_, Tail_...>>
{
    using sub_type = typename remove<type_list<Tail_...>, Head_>::type;
    using type = typename push_front<typename unique<sub_type>::type, Head_>::type;
};

template<typename TypeList_, typename Iterator_>
struct erase;

template<typename Head_, typename... Tail_, typename Iterator_>
struct erase<type_list<Head_, Tail_...>, Iterator_>
{
    using type = typename std::conditional<0u == Iterator_::value,
                                           type_list<Tail_...>,
                                           typename push_front<typename erase<type_list<Tail_...>,
                                                                              typename dec<Iterator_>::type>::type,
                                                               Head_>::type>::type;
};

template<typename TypeList_, std::size_t N_>
struct at;

template<typename Head_, typename... Tail_>
struct at<type_list<Head_, Tail_...>, 0u>
{
    using type = Head_;
};

template<typename Head_, typename... Tail_, std::size_t N_>
struct at<type_list<Head_, Tail_...>, N_>
{
    using type = typename at<type_list<Tail_...>, N_ - 1>::type;
};

template<typename Iterator_>
struct deref: at<typename Iterator_::container, Iterator_::value>
{
};

template<class First_, class Last_, typename Type_>
struct find_impl
{
    using type = typename std::conditional<std::is_same<typename deref<First_>::type, Type_>::value,
                                           First_,
                                           typename find_impl<typename inc<First_>::type, Last_, Type_>::type>::type;
};

template<class Last_, typename Type_>
struct find_impl<Last_, Last_, Type_>
{
    using type = Last_;
};

template<typename TypeList_, typename Type_>
struct find: find_impl<typename begin<TypeList_>::type, typename end<TypeList_>::type, Type_>
{
};

template<typename TypeList_, typename OldType_, typename NewType_>
struct replace;

template<typename OldType_, typename NewType_>
struct replace<null_type_list, OldType_, NewType_>
{
    using type = null_type_list;
};

template<typename Head_, typename...Tail_, typename OldType_, typename NewType_>
struct replace<type_list<Head_, Tail_...>, OldType_, NewType_>
{
    using sub_type = typename replace<type_list<Tail_...>, OldType_, NewType_>::type;
    using type = typename std::conditional<std::is_same<Head_, OldType_>::value,
                                           typename push_front<sub_type, NewType_>::type,
                                           typename push_front<sub_type, OldType_>::type>::type;
};

}
