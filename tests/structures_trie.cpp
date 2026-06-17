// Copyright - 2023 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <algorithm>
#include <ext/meta/concepts.hpp>
#include <gtest/gtest.h>

#define EXT_TEST
#include <ext/structures/trie.hpp>


[[nodiscard]] auto insert(auto& t, std::string s) {
    using namespace ext::structures::detail_trie;
    return t.insert(str2vec(s));
}

[[nodiscard]] auto get_child(auto& n, char c) {
    using namespace ext::structures::detail_trie;
    decltype(n->children->begin()->second.get()) rv = nullptr;

    if (n->children == nullptr)
        return rv;

    auto* map = n->children.get();
    auto it = map->find(c);
    if (it != map->end())
        if (it->second != nullptr)
            rv = it->second.get();

    return rv;
}

[[nodiscard]] auto get_prefix(auto& n) {
    using namespace ext::structures::detail_trie;
    return vec2str(n->prefix);
}

[[nodiscard]] auto opt2str(std::optional<char>& opt) {
    std::string rv;
    if (opt.has_value())
        rv.push_back(opt.value());
    return rv;
}

TEST(structures_trie, split_point) {
    using namespace ext::structures::detail_trie;
    {
        auto actual = find_split_point(str2vec("123aa"), str2vec("123b"));
        EXPECT_EQ(actual, 3);
    }
    {
        auto actual = find_split_point(str2vec("a"), str2vec(""));
        EXPECT_EQ(actual, 0);
    }
    {
        auto actual = find_split_point(str2vec(""), str2vec("a"));
        EXPECT_EQ(actual, 0);
    }
    {
        auto actual = find_split_point(str2vec("a"), str2vec("b"));
        EXPECT_EQ(actual, 0);
    }
}

TEST(structures_trie, splits) {
    using namespace ext::structures::detail_trie;
    {
        auto actual = split_info(str2vec("123aa"), str2vec("123b"));
        auto expected = split<typename std::vector<typename std::string::value_type>>{
            .parent_prefix = str2vec("123"),
            .split_label = 'a',
            .split_prefix = str2vec("a"),
            .insert_label = 'b',
            .insert_prefix = str2vec(""),
        };

        EXPECT_EQ(vec2str(expected.parent_prefix), vec2str(actual.parent_prefix));
        EXPECT_EQ(opt2str(expected.split_label), opt2str(actual.split_label));
        EXPECT_EQ(vec2str(expected.split_prefix), vec2str(actual.split_prefix));
        EXPECT_EQ(vec2str(expected.insert_prefix), vec2str(actual.insert_prefix));
        EXPECT_EQ(opt2str(expected.insert_label), opt2str(actual.insert_label));
    }
    {
        auto actual = split_info(str2vec(""), str2vec(""));
        auto expected = split<typename std::vector<typename std::string::value_type>>{};

        EXPECT_EQ(vec2str(expected.parent_prefix), vec2str(actual.parent_prefix));
        EXPECT_EQ(opt2str(expected.split_label), opt2str(actual.split_label));
        EXPECT_EQ(vec2str(expected.split_prefix), vec2str(actual.split_prefix));
        EXPECT_EQ(vec2str(expected.insert_prefix), vec2str(actual.insert_prefix));
        EXPECT_EQ(opt2str(expected.insert_label), opt2str(actual.insert_label));
    }
    {
        auto actual = split_info(str2vec(""), str2vec("a"));
        auto expected = split<typename std::vector<typename std::string::value_type>>{
            .insert_label = 'a',
        };

        EXPECT_EQ(vec2str(expected.parent_prefix), vec2str(actual.parent_prefix));
        EXPECT_EQ(opt2str(expected.split_label), opt2str(actual.split_label));
        EXPECT_EQ(vec2str(expected.split_prefix), vec2str(actual.split_prefix));
        EXPECT_EQ(vec2str(expected.insert_prefix), vec2str(actual.insert_prefix));
        EXPECT_EQ(opt2str(expected.insert_label), opt2str(actual.insert_label));
    }
    {
        auto actual = split_info(str2vec("a"), str2vec(""));
        auto expected = split<typename std::vector<typename std::string::value_type>>{
            .split_label = 'a',
        };

        EXPECT_EQ(vec2str(expected.parent_prefix), vec2str(actual.parent_prefix));
        EXPECT_EQ(opt2str(expected.split_label), opt2str(actual.split_label));
        EXPECT_EQ(vec2str(expected.split_prefix), vec2str(actual.split_prefix));
        EXPECT_EQ(vec2str(expected.insert_prefix), vec2str(actual.insert_prefix));
        EXPECT_EQ(opt2str(expected.insert_label), opt2str(actual.insert_label));
    }
    {
        auto actual = split_info(str2vec("a"), str2vec("b"));
        auto expected = split<typename std::vector<typename std::string::value_type>>{
            .split_label = 'a',
            .insert_label = 'b',
        };

        EXPECT_EQ(vec2str(expected.parent_prefix), vec2str(actual.parent_prefix));
        EXPECT_EQ(opt2str(expected.split_label), opt2str(actual.split_label));
        EXPECT_EQ(vec2str(expected.split_prefix), vec2str(actual.split_prefix));
        EXPECT_EQ(vec2str(expected.insert_prefix), vec2str(actual.insert_prefix));
        EXPECT_EQ(opt2str(expected.insert_label), opt2str(actual.insert_label));
    }
}

TEST(structures_trie, simple_inserts) {
    using namespace ext::structures::detail_trie;

    ext::structures::trie<char, std::uint8_t> trie;
    auto root = trie.root.get();

    {
        auto [ip, ok] = insert(trie, "aaacc");
        ASSERT_TRUE(ok);
        ASSERT_EQ(root, ip);
        //  root [aaacc]
        ASSERT_EQ(get_prefix(root), "aaacc");
    }
    {
        auto [_, ok] = trie.insert(str2vec("aaab"));
        ASSERT_TRUE(ok);
        //   root(aaa)
        //   c/    \b
        //  n[c]   n[]
        ASSERT_EQ(get_prefix(root), "aaa");

        auto aaacc = get_child(root, 'c');
        ASSERT_NE(aaacc, nullptr);
        ASSERT_EQ(get_prefix(aaacc), "c");

        auto aaab = get_child(root, 'b');
        ASSERT_NE(aaab, nullptr);
        ASSERT_EQ(get_prefix(aaab), "");
    }
    {
        auto [insert_node, ok] = insert(trie, "bab");
        ASSERT_TRUE(ok);
        //       root()
        //      a/    \b
        //    n(aa)  n[ab]
        //   c/   \b
        //  n[c]  n[]
        //
        EXPECT_EQ(get_prefix(root), "");
        {
            auto aaa = get_child(root, 'a');
            ASSERT_NE(aaa, nullptr);
            EXPECT_EQ(get_prefix(aaa), "aa");
            {
                {
                    auto aaacc = get_child(aaa, 'c');
                    ASSERT_NE(aaacc, nullptr);
                    EXPECT_EQ(get_prefix(aaacc), "c");
                }
                {
                    auto aaab = get_child(aaa, 'b');
                    ASSERT_NE(aaab, nullptr);
                    EXPECT_EQ(get_prefix(aaab), "");
                }
            }
        }
        {
            auto bab = get_child(root, 'b');
            ASSERT_NE(bab, nullptr);
            EXPECT_EQ(bab, insert_node);
            EXPECT_EQ(get_prefix(bab), "ab");
        }
    }
}
