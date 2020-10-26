#pragma once


#include <functional> // std::hash<int>
#include <memory>     // std::allocator

#include <exception>  // std::runtime_error
#include <cstddef>    // size_t (but can do without it)
#include <cassert>    // assert


template<class Key, class T>
class HashNode
{
private:
    Key key_;
    T value_;
    HashNode* next_ptr_ = nullptr;
public:
    HashNode() = delete;
    HashNode(const Key &key, const T &value);

    Key get_key() const;
    T get_value() const;
    HashNode* get_next() const;
    bool check_key(const Key &key) const;
    void set_value(const T& value);
    void set_next(const HashNode* next);

    ~HashNode();
};


template<
        class Key,
        class T,
        class Hash = std::hash<Key>,
        class Allocator = std::allocator<HashNode<Key, T>> >
class HashMap {
private:
    constexpr static double rehash_ratio_ = 0.75;

    size_t buffer_size_;
    size_t size_;
    Hash hasher_;
    HashNode<Key, T>** arr_;

public:
    HashMap();
    explicit HashMap(size_t size);
    HashMap(const HashMap<Key, T, Hash, Allocator> &other);
    ~HashMap();
};

#include "HashMap_template.h"