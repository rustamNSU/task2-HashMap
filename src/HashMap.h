#pragma once


#include <functional> // std::hash<int>
#include <memory>     // std::allocator
#include <utility>    // std::pair

#include <exception>  // std::runtime_error
#include <cstddef>    // size_t (but can do without it)
#include <cassert>    // assert


template<
        class Key,
        class T,
        class Hash = std::hash<Key>,
        class Allocator = std::allocator<std::pair<Key, T>>
        >
class HashMap {
private:
    double rehash_ratio_ = 0.75;
    Allocator allocator;

    class HashNode
    {
    private:
        using node_type    = std::pair<Key, T>;
        using node_pointer = std::pair<Key, T>*;
        node_pointer node_ = nullptr;
        HashNode* next_ptr_ = nullptr;
    public:
        HashNode();
        HashNode(const node_pointer new_node);

        Key get_key() const;
        T get_value() const;
        HashNode* get_next() const;
        bool check_key(const Key &key) const;
        void set_value(const T& value);
        void set_next(HashNode* next);

        ~HashNode();
    };

    size_t buffer_size_;
    size_t size_;
    Hash hasher_;
    HashNode** arr_;

public:
    explicit HashMap(const Allocator &node_allocator = Allocator{});
    explicit HashMap(size_t capacity);
    HashMap(const HashMap<Key, T, Hash, Allocator> &other);
    ~HashMap();
};


#include "HashMap_template.h"