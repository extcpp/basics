// Copyright - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_STRUCTURES_TRIE_HEADER
#define EXT_STRUCTURES_TRIE_HEADER

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include <ext/meta/concepts.hpp>

namespace ext::structures {
namespace detail_trie {

inline std::vector<char> str2vec(std::string const& str) {
    std::vector<char> rv;
    for (auto const& c : str) rv.push_back(c);
    return rv;
}

template <typename T>
std::string vec2str(std::vector<T> const& vec) {
    std::string rv;
    for (auto const& c : vec) rv.push_back(c);
    return rv;
}

template <typename Key, typename Value>
Value* find_or_null(std::unique_ptr<std::map<Key, Value>>& map, Key const& key) {
    if (map == nullptr)
        return nullptr;
    auto it = map->find(key);
    return it != map->end() ? &(it->second) : nullptr;
}

// node in trie
template <typename Label, typename Value>
class node {
    public:
    using value_t = Value;
    using label_t = Label;
    using key_t = std::vector<label_t>;
    using children_t = std::map<label_t, std::unique_ptr<node>>;

    key_t prefix;
    std::unique_ptr<children_t> children;
    std::unique_ptr<value_t> value;
    bool is_word = false;

    void ensure_children() {
        if (children == nullptr)
            this->children = std::make_unique<children_t>();
    }

    void assign_data(key_t& prefix, std::unique_ptr<value_t>& value, bool isw = true) {
        this->prefix = std::move(prefix);
        this->is_word = isw;
        this->value = std::move(value);
    }

    std::pair<node*, bool>
        insert_node(label_t const& label, key_t& prefix, std::unique_ptr<value_t>& value, bool is_word) {
        auto new_node = std::make_unique<node>();
        ensure_children();
        auto [it, ok] = this->children->try_emplace(label, std::move(new_node));
        if (!ok)
            return {this, false};
        auto rv = it->second.get();
        rv->assign_data(prefix, value, is_word);
        return {rv, true};
    }
};

// description of how a node will be splitted
template <typename Key>
struct split {
    using key_t = Key;
    using label_t = typename key_t::value_type;

    key_t parent_prefix;
    std::optional<label_t> split_label;
    key_t split_prefix;
    std::optional<label_t> insert_label;
    key_t insert_prefix;
};

template <typename Key>
[[nodiscard]] constexpr std::size_t find_split_point(Key const& a, Key const& b) {
    std::size_t rv = 0;
    while (rv < std::min(a.size(), b.size())) {
        if (a[rv] == b[rv])
            ++rv;
        else
            break;
    }
    return rv;
}

// function to calculate the spilts
template <typename Key>
[[nodiscard]] split<Key> split_info(Key const& parent_prefix, Key const& insert_prefix) {
    std::size_t sp = find_split_point(parent_prefix, insert_prefix);

    auto in_beg = insert_prefix.begin();
    auto in_sp = in_beg + sp;
    auto in_sp_cpy = in_sp;
    auto in_end = insert_prefix.end();

    auto pa_beg = parent_prefix.begin();
    auto pa_sp = pa_beg + sp;
    auto pa_end = parent_prefix.end();

    std::optional<typename Key::value_type> split_label;
    if (pa_sp != pa_end)
        split_label = *(pa_sp++);

    std::optional<typename Key::value_type> insert_label;
    if (in_sp != in_end)
        insert_label = *(in_sp++);

    return split<Key>{
        .parent_prefix = Key(in_beg, in_sp_cpy),
        .split_label = std::move(split_label),
        .split_prefix = Key(pa_sp, pa_end),
        .insert_label = std::move(insert_label),
        .insert_prefix = Key(in_sp, in_end),
    };
}


} // namespace detail_trie

template <typename Label, typename Value = std::uint8_t>
class trie {
    public:
    using label_t = Label;
    using value_t = Value;

    using node_t = detail_trie::node<label_t, value_t>;
    using key_t = typename node_t::key_t;
    using children_t = typename node_t::children_t;

    [[nodiscard]] std::pair<node_t*, bool> insert(key_t const& key, std::unique_ptr<value_t> value = nullptr) {
        using namespace detail_trie;

        auto [insert_parent, insert_prefix] = find_insert_parent(root.get(), key);
        if (insert_parent == nullptr)
            return {nullptr, false};

        auto& ip = *insert_parent;

        if (insert_prefix.empty()) {
            // must be inserted into ip
            if (ip.is_word)
                return {insert_parent, false};

            ip.assign_data(insert_prefix, value);
            return {insert_parent, true};
        } else if (!ip.is_word && ip.prefix.empty() && (ip.children == nullptr || ip.children->empty())) [[unlikely]] {
            ip.assign_data(insert_prefix, value);
            return {insert_parent, true};
        }

        split<key_t> si = split_info(insert_parent->prefix, insert_prefix);
        if (!si.insert_label.has_value())
            return {insert_parent, false};

        // create split if neccessary
        if (si.split_label.has_value()) {
            auto split_children = std::move(ip.children);
            ip.children = std::make_unique<children_t>();

            auto [split_node, ok] = ip.insert_node(si.split_label.value(), si.split_prefix, ip.value, ip.is_word);
            if (!ok) {
                ip.children = std::move(ip.children); // restore state
                throw std::logic_error("this insert into the trie not fail");
            }

            split_node->children = std::move(split_children);
            ip.prefix = si.parent_prefix;
            ip.is_word = false;
            ip.value = nullptr;
        }

        return ip.insert_node(si.insert_label.value(), si.insert_prefix, value, true);
    }

#ifndef EXT_TEST
    private:
#endif

    [[nodiscard]] static std::pair<node_t*, key_t> find_insert_parent(node_t* start, key_t key) {
        std::pair<node_t*, key_t> rv(start, {});

        if (start == nullptr || key.empty())
            return rv;

        auto key_it = key.begin();
        auto key_end = key.end();
        do {
            // first check the prefix
            // return if the key to insert is shorter or does not match
            {
                auto key_it_copy = key_it; // use this
                for (auto const& label : rv.first->prefix) {
                    if (key_it == key_end || label != *key_it) {
                        std::move(key_it_copy, key_end, std::back_inserter(rv.second));
                        return rv;
                    }
                    ++key_it;
                }
            }
            // check if there is a child that matches or break
            {
                auto next = find_or_null(rv.first->children, *key_it);
                if (next != nullptr) {
                    rv.first = next->get();
                    ++key_it;
                } else
                    break;
            }
        } while (key_it != key_end);

        std::move(key_it, key_end, std::back_inserter(rv.second));
        return rv;
    }

    // member vars
    std::unique_ptr<node_t> root = std::make_unique<node_t>();
};

} // namespace ext::structures
#endif
