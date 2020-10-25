#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional> // std::hash<int>

#include <exception>  // std::runtime_error
#include <cstddef>    // size_t (but can do without it)
#include <cassert>    // assert


template< class Key, class T, class Hash = std::hash<Key> >
class HashMap{
private:

    constexpr static double rehash_ratio_ = 0.75;
    class Node{
    public:
        Key key_;
        T mapped_type_;
        Node* ptr_ = nullptr;

        ~Node();
    };

    size_t buffer_size_;
    size_t size_;
    Hash hasher_;
    Node **arr_;

public:

    HashMap();
    HashMap(size_t size);

    ~HashMap();

};

template<class Key, class T, class Hash>
HashMap<Key, T, Hash>::HashMap() :
    buffer_size_(0),
    size_(0),
    hasher_(Hash()),
    arr_(nullptr)
{
}

template<class Key, class T, class Hash>
HashMap<Key, T, Hash>::HashMap(size_t capacity) :
    buffer_size_(capacity),
    size_(0),
    hasher_(Hash())
{
    arr_ = new Node* [buffer_size_];
}

template<class Key, class T, class Hash>
HashMap<Key, T, Hash>::~HashMap()
{
    delete[] arr_;
}

template<class Key, class T, class Hash>
HashMap<Key, T, Hash>::Node::~Node()
{
    delete ptr_;
}

