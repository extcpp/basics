// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_LRU_CACHE_HEADER
#define EXT_UTIL_LRU_CACHE_HEADER

#include <functional>
#include <optional>
#include <list>
#include <mutex>
#include <unordered_map>
#include <ext/macros/assert.hpp>

namespace ext::util {

template <typename Key, typename Value>
class lru_cache {
public:
    lru_cache(size_t max_size) : _max_size{max_size} { }
    lru_cache(lru_cache const&) = delete;

    static void default_update(Value&){};
    static bool default_remove(Value const&){ return false; };

    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> guard(_mut);

        auto found = _map.find(key);
        if (found == _map.end()) {
            if (_map.size() + 1 > _max_size) {
                auto last = _list.crbegin();
                _map.erase(last->first);
                _list.pop_back();
            }

            _list.emplace_front(key, value);
            _map[key] = _list.begin();
        } else {
            found->second->second = value;
            // pull element to beginning of list
            _list.splice(_list.cbegin(), _list, found->second);
        }

        EXT_ASSERT(_map.size() == _list.size());
    }

    void put(const Key& key,Value&& value) {
        std::lock_guard<std::mutex> guard(_mut);

        auto found = _map.find(key);
        if (found == _map.end()) {
            if (_map.size() + 1 > _max_size) {
                auto last = _list.crbegin();
                _map.erase(last->first);
                _list.pop_back();
            }

            _list.emplace_front(key, std::move(value));
            _map[key] = _list.begin();
        } else {
            found->second->second = value;
            // pull element to beginning of list
            _list.splice(_list.cbegin(), _list, found->second);
        }

        EXT_ASSERT(_map.size() == _list.size());
    }

    Value const* get(const Key& key) const {
        std::lock_guard<std::mutex> guard(_mut);

        auto found = _map.find(key);
        if (found == _map.end()) {
            return nullptr;
        } else {
            return std::addressof(found->second->second);
        }
    }

    Value* get_mut(const Key& key) {
        std::lock_guard<std::mutex> guard(_mut);

        auto found = _map.find(key);
        if (found == _map.end()) {
            return nullptr;
        } else {
            return std::addressof(found->second->second);
        }
    }

    template<typename UpdatePredicate = decltype(default_update)>
    Value const* get_update(const Key& key, UpdatePredicate update_pred = &default_update) {
        static_assert(std::is_convertible_v<UpdatePredicate, std::function<void(Value&)>>, "wrong singnature for UpdatePredicate");
        static_assert(std::is_same_v<decltype(update_pred(std::declval<Value&>())) ,void>, "update predicate should not return anything");
        std::lock_guard<std::mutex> guard(_mut);

        auto found = _map.find(key);
        if (found == _map.end()) {
            return nullptr;
        } else {
            Value& value = found->second->second;
            update_pred(value);
            _list.splice(_list.begin(), _list, found->second);
            return std::addressof(value);
        }
    }

    std::optional<Value> get_remove(const Key& key) {
        std::lock_guard<std::mutex> guard(_mut);
        auto rv = std::optional<Value>{};

        auto found = _map.find(key);
        if (found != _map.end()) {
            rv = std::optional<Value>{ std::move(found->second->second) };
            // move value to end of list to end of list
            _list.splice(_list.cend(), _list, found->second);
            //remove item from map
            _map.erase(found);
            //delete element
            _list.pop_back();
        }

        EXT_ASSERT(_map.size() == _list.size());
        return rv;
    }

    bool remove(const Key& key) {
        std::lock_guard<std::mutex> guard(_mut);

        auto found = _map.find(key);
        if (found != _map.end()) {
            // move value to end of list to end of list
            _list.splice(_list.cend(), _list, found->second);
            //remove item from map
            _map.erase(found);
            //delete element
            _list.pop_back();
            EXT_ASSERT(_map.size() == _list.size());
            return true;
        }
        return false;
    }

    template<typename RemovePredicate>
    std::size_t remove_all_matching(RemovePredicate remove_pred) {
        static_assert(std::is_convertible_v<RemovePredicate, std::function<bool(Value const&)>>, "wrong singnature for RemovePredicate");
        static_assert(std::is_same_v<decltype(remove_pred(std::declval<Value&>())) , bool>, "remove predicate should not return anything");
        std::lock_guard<std::mutex> guard(_mut);

        std::size_t n = 0;
        for (auto it = _list.begin(); it != _list.end();) {
            if (remove_pred(it->second) /* value */) {
                _map.erase(it->first /* key */);
                ++it;
                _list.erase(std::prev(it));
                ++n;
                EXT_ASSERT(_map.size() == _list.size());
            } else {
                ++it;
            }
        }

        EXT_ASSERT(_map.size() == _list.size());
        return n;
    }

    size_t size() const noexcept {
        std::lock_guard<std::mutex> guard(_mut);

        return _map.size();
    }

private:
    std::list<std::pair<Key, Value>> _list;
    std::unordered_map<Key, decltype(_list.begin())> _map;
    size_t _max_size;
    mutable std::mutex _mut;
};

} // namespace ext::util
#endif // EXT_UTIL_LRU_CACHE_HEADER
