#include <iostream>
#include <type_list.hpp>

#define EXPECT_SIZE(T, size) \
{\
    std::cout << "line number: " << __LINE__                               \
              << "\n  expect: " << size\
              << "\n  actual: " << T::numberOfTypes\
              << "\n  result: " << ((T::numberOfTypes == size)? "pass" : "fail") << std::endl; \
}

#define EXPECT_TRUE(t)                                                         \
{\
    std::cout << "line number: " << __LINE__                               \
              << "\n  expect: true "\
              << "\n  actual: " << std::boolalpha << t                         \
              << "\n  result: " << (t? "pass" : "fail") << std::endl; \
}


int main(int argc, char *argv[])
{
    using tl0 = type_list<>;
    EXPECT_SIZE(tl0, 0);
    EXPECT_TRUE(!(is_first_type<int, tl0>::value));

    using tl1 = push<int, tl0>::type;
    EXPECT_SIZE(tl1, 1);
    EXPECT_TRUE((is_first_type<int, tl1>::value));

    using tl2 = push<char, tl1>::type;
    EXPECT_SIZE(tl2, 2);
    EXPECT_TRUE((is_first_type<int, tl2>::value));
    EXPECT_TRUE((has_type<int, tl2>::value));
    EXPECT_TRUE((has_type<char, tl2>::value));

    using tl3 = pop<tl2>::type;
    EXPECT_SIZE(tl3, 1);
    EXPECT_TRUE((is_first_type<char, tl3>::value));

    using tl4 = pop<tl3>::type;
    EXPECT_SIZE(tl4, 0);

    using tl5 = pop<tl4>::type;
    EXPECT_SIZE(tl5, 0);

    using tl6 = type_list<int, char>;
    using tl7 = remove_head_type_if<char, tl6>::type;
    EXPECT_TRUE((is_first_type<int, tl7>::value));
    EXPECT_SIZE(tl7, 2);

    using tl8 = remove_head_type_if<int, tl7>::type;
    EXPECT_TRUE((is_first_type<char, tl8>::value));
    EXPECT_SIZE(tl8, 1);

    using tl9 = remove_head_type_if<char, tl8>::type;
    EXPECT_SIZE(tl9, 0);
    return 0;
}
