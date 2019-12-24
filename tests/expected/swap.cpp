#include "expected.hpp"

struct no_throw {
    no_throw(std::string i) : i(i) {}
    std::string i;
};
struct canthrow_move {
    canthrow_move(std::string i) : i(i) {}
    canthrow_move(canthrow_move const&) = default;
    canthrow_move(canthrow_move&& other) noexcept(false) : i(other.i) {}
    canthrow_move& operator=(canthrow_move&&) = default;
    std::string i;
};

bool should_throw = false;
struct willthrow_move {
    willthrow_move(std::string i) : i(i) {}
    willthrow_move(willthrow_move const&) = default;
    willthrow_move(willthrow_move&& other) : i(other.i) {
        if (should_throw)
            throw 0;
    }
    willthrow_move& operator=(willthrow_move&&) = default;
    std::string i;
};
static_assert(eu::detail::is_swappable<no_throw>::value, "");

template<class T1, class T2>
void swap_test() {
    std::string s1 = "abcdefghijklmnopqrstuvwxyz";
    std::string s2 = "zyxwvutsrqponmlkjihgfedcba";

    eu::expected<T1, T2> a{s1};
    eu::expected<T1, T2> b{s2};
    swap(a, b);
    ASSERT_TRUE(a->i == s2);
    ASSERT_TRUE(b->i == s1);

    a = s1;
    b = eu::unexpected<T2>(s2);
    swap(a, b);
    ASSERT_TRUE(a.error().i == s2);
    ASSERT_TRUE(b->i == s1);

    a = eu::unexpected<T2>(s1);
    b = s2;
    swap(a, b);
    ASSERT_TRUE(a->i == s2);
    ASSERT_TRUE(b.error().i == s1);

    a = eu::unexpected<T2>(s1);
    b = eu::unexpected<T2>(s2);
    swap(a, b);
    ASSERT_TRUE(a.error().i == s2);
    ASSERT_TRUE(b.error().i == s1);

    a = s1;
    b = s2;
    a.swap(b);
    ASSERT_TRUE(a->i == s2);
    ASSERT_TRUE(b->i == s1);

    a = s1;
    b = eu::unexpected<T2>(s2);
    a.swap(b);
    ASSERT_TRUE(a.error().i == s2);
    ASSERT_TRUE(b->i == s1);

    a = eu::unexpected<T2>(s1);
    b = s2;
    a.swap(b);
    ASSERT_TRUE(a->i == s2);
    ASSERT_TRUE(b.error().i == s1);

    a = eu::unexpected<T2>(s1);
    b = eu::unexpected<T2>(s2);
    a.swap(b);
    ASSERT_TRUE(a.error().i == s2);
    ASSERT_TRUE(b.error().i == s1);
}

TEST(expected_swap, all) {

    swap_test<no_throw, no_throw>();
    swap_test<no_throw, canthrow_move>();
    swap_test<canthrow_move, no_throw>();

    std::string s1 = "abcdefghijklmnopqrstuvwxyz";
    std::string s2 = "zyxwvutsrqponmlkjihgfedcbaxxx";
    eu::expected<no_throw, willthrow_move> a{s1};
    eu::expected<no_throw, willthrow_move> b{eu::unexpect, s2};
    should_throw = 1;

#ifdef _MSC_VER
    // this seems to break catch on GCC and Clang
    ASSERT_TRUE_THROWS(swap(a, b));
#endif

    ASSERT_TRUE(a->i == s1);
    ASSERT_TRUE(b.error().i == s2);
}
