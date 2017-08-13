
#include <iostream>
#include <type_traits>
#include "type_list.hpp"

int main(int argc, const char * argv[]) {
    using list1 = type_list<char, short, int, long, float, double>;
    std::cout << "list1 length = " << length<list1>::value << std::endl;

    static_assert(std::is_same<head<list1>::type, char>::value, "");
    static_assert(std::is_same<tail<list1>::type, type_list<short, int, long, float, double>>::value, "");
    static_assert(std::is_same<last<list1>::type, double>::value, "");
    static_assert(std::is_same<front<list1>::type, type_list<char, short, int, long, float>>::value, "");
    static_assert(std::is_same<pop_back_t<list1, 2>, type_list<char, short, int, long>>::value, "");

    static_assert(std::is_same<list1, concat_t<type_list<char, short, int, long>, type_list<float, double>>>::value, "");

    static_assert(std::is_same<slice_t<list1, 1, 2>, type_list<short, int>>::value, "");
    static_assert(std::is_same<slice_t<list1, -2, 2>, type_list<float, double>>::value, "");
    static_assert(std::is_same<slice_t<list1, 3, 0>, null_list>::value, "");
    static_assert(std::is_same<slice_t<list1, 3, 6>, type_list<long, float, double>>::value, "");
    static_assert(std::is_same<slice_t<list1, 6, 6>, null_list>::value, "");
    static_assert(std::is_same<slice_t<list1, -7, 6>, list1>::value, "");
    static_assert(std::is_same<slice_t<list1, -7, 3>, type_list<char, short, int>>::value, "");

    static_assert(std::is_same<at<list1, 0>, char>::value, "");
    static_assert(std::is_same<at<list1, -3>, long>::value, "");

    static_assert(index_of<list1, int>::value == 2, "");
    static_assert(index_of<list1, bool>::value == -1, "");

    using list2 = type_list<char, short, long, float, double>;
    static_assert(std::is_same<insert_t<list2, 2, int>, list1>::value, "");
    static_assert(std::is_same<insert_t<type_list<int, float>, 0, char>, type_list<char, int, float>>::value, "");
    static_assert(std::is_same<insert_t<type_list<int, float>, 2, char>, type_list<int, float, char>>::value, "");
    static_assert(std::is_same<insert_t<type_list<int, float>, -1, char>, type_list<int, char, float>>::value, "");
    static_assert(std::is_same<insert_t<type_list<int, float>, -3, char>, type_list<char, int, float>>::value, "");

    using list3 = type_list<char, int, char, float>;
    static_assert(std::is_same<remove_at<list3, 1>::type, type_list<char, char, float>>::value, "");
    static_assert(std::is_same<remove_at<list3, -2>::type, type_list<char, int, float>>::value, "");
    static_assert(std::is_same<remove_at<list3, 4>::type, list3>::value, "");
    static_assert(std::is_same<remove_at<list3, -5>::type, list3>::value, "");
    static_assert(std::is_same<remove_t<list3, char>, type_list<int, char, float>>::value, "");
    static_assert(std::is_same<remove_all<list3, char>::type, type_list<int, float>>::value, "");
    static_assert(std::is_same<remove_all<list3, float>::type, type_list<char, int, char>>::value, "");
    static_assert(std::is_same<remove_all<list3, double>::type, list3>::value, "");

    static_assert(std::is_same<replace_t<list3, char, bool>, type_list<bool, int, char, float>>::value, "");
    static_assert(std::is_same<replace_all<list3, char, bool>::type, type_list<bool, int, bool, float>>::value, "");
    static_assert(std::is_same<replace_all<list3, int, bool>::type, type_list<char, bool, char, float>>::value, "");

    static_assert(std::is_same<reverse_t<list3>, type_list<float, char, int, char>>::value, "");

    static_assert(std::is_same<deduplication_t<list3>, type_list<char, int, float>>::value, "");

    return 0;
}
