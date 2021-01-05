#pragma once


#include <functional> // std::hash<int>
#include <memory>     // std::allocator, std::allocator_traits
#include <utility>    // std::pair
#include <optional>   // std::optional

#include <exception>  // std::runtime_error
#include <cstddef>    // size_t (but can do without it)
#include <cassert>    // assert

#include <mutex>
#include <shared_mutex>

template<
        class Key,
        class T,
        class Hash = std::hash<Key>,
        class Allocator = std::allocator<std::pair<const Key, T>>
        >
class HashMap{
public:
    using size_type      = std::size_t;
    using key_type       = Key;
    using mapped_type    = T;
    using value_type     = std::pair<const Key, T>;
    using hasher         = Hash;

private:
    class HashNode
    {
    private:
        using node_type    = value_type;
        using node_pointer = node_type*;
        node_type node_;
        HashNode* next_ptr_ = nullptr;

    public:
        HashNode() = delete;
        HashNode(const node_type& node);
        node_type& get_value();
        key_type get_key() const noexcept;
        mapped_type get_mapped() const noexcept;
        mapped_type& get_ref_mapped();
        HashNode* get_next() const;
        bool check_key(const key_type &key) const;
        void set_mapped(const mapped_type& value);
        void set_next(HashNode* const next);
        void clear();

        ~HashNode();
    };
    class HashBucket
    {
    private:
        HashNode* head_ = nullptr;

    public:
        HashBucket() : head_(nullptr){};
        HashBucket(const HashBucket&) = delete;
        HashBucket(HashBucket&&)      = delete;
        HashBucket& operator=(const HashBucket&) = delete;
        HashBucket& operator=(HashBucket&&)      {head_ = nullptr; return *this;}
        ~HashBucket(){
            delete head_;
        }

        void set(const key_type& key, const mapped_type& mapped);
        std::optional<mapped_type> get(const key_type &key);
        mapped_type& get_ref(const key_type &key, size_type &size);
        HashNode* get_node(const key_type &key, size_type &size);
        HashNode* get_head();
    };

    using allocator_value  = Allocator;
    using allocator_node   = typename std::allocator_traits<Allocator>::template rebind_alloc<HashNode>;
    using allocator_bucket = typename std::allocator_traits<Allocator>::template rebind_alloc<HashBucket>;

    double rehash_ratio_ = 0.75;
    allocator_value allocator_;
    allocator_bucket alloc_bucket_;
    allocator_node alloc_node_;
    size_type capacity_;
    size_type size_;
    hasher hasher_;
    typename std::allocator_traits<allocator_bucket>::pointer arr_;

    mutable std::shared_timed_mutex mutex_;
    mutable std::shared_mutex mtx_;

public:
    explicit HashMap(const allocator_value &node_allocator_type);
    explicit HashMap(size_type capacity = 4,
                     const hasher& hash_function = hasher(),
                     const allocator_value& allocator = allocator_value());

    HashMap(const HashMap<key_type, mapped_type, hasher, allocator_value> &other);
    HashMap& operator=(const HashMap<key_type, mapped_type, hasher, allocator_value> &right);
    HashMap& operator=(HashMap<key_type, mapped_type, hasher, allocator_value> &&right);

    size_t get_hash(const key_type &key) const;
    std::optional<mapped_type> get(const key_type &key);

    size_type size() const noexcept;
    bool empty() const noexcept;
    
    mapped_type& operator[](const key_type &key);

    class iterator;
    class const_iterator;

    iterator begin() noexcept;
    iterator end() noexcept;

    iterator insert(const key_type& key, const mapped_type& value) noexcept;
    void rehash();
    void clear();
    virtual ~HashMap();
};



template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashNode::HashNode(const node_type& node) : 
    node_(node)
{

}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::value_type& HashMap<Key, T, Hash, Allocator>::HashNode::get_value()
{
    return node_;
}


template<class Key, class T, class Hash, class Allocator>
Key HashMap<Key, T, Hash, Allocator>::HashNode::get_key() const noexcept
{
    return node_.first;
}


template<class Key, class T, class Hash, class Allocator>
T HashMap<Key, T, Hash, Allocator>::HashNode::get_mapped() const noexcept
{
    return node_.second;
}


template<class Key, class T, class Hash, class Allocator>
T& HashMap<Key, T, Hash, Allocator>::HashNode::get_ref_mapped()
{
    return node_.second;
}


template<class Key, class T, class Hash, class Allocator>
typename HashMap<Key, T, Hash, Allocator>::HashNode* HashMap<Key, T, Hash, Allocator>::HashNode::get_next() const
{
    return next_ptr_;
}


template<class Key, class T, class Hash, class Allocator>
bool HashMap<Key, T, Hash, Allocator>::HashNode::check_key(const key_type &key) const
{
    return node_.first == key;
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::HashNode::set_mapped(const mapped_type &value)
{
    node_.second = value;
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::HashNode::set_next(HashNode* const next)
{
    next_ptr_ = next;
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::HashNode::clear(){
    delete next_ptr_;
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashNode::~HashNode()
{
    clear();
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::HashBucket::set(const key_type& key, const mapped_type& mapped)
{
    auto node_ptr = head_;
    while (node_ptr != nullptr)
    {
        if (node_ptr->check_key(key)){
            node_ptr->set_mapped(mapped);
            return;
        }
        node_ptr = node_ptr->get_next();
    }
    node_ptr = new HashNode(value_type{key, mapped});
}


template<class Key, class T, class Hash, class Allocator>
std::optional<T> HashMap<Key, T, Hash, Allocator>::HashBucket::get(const key_type& key)
{
    auto node_ptr = head_;
    while (node_ptr != nullptr)
    {
        if (node_ptr->check_key(key)){
            return node_ptr->get_mapped();
        }
        node_ptr = node_ptr->get_next();
    }
    return {};
}


template<class Key, class T, class Hash, class Allocator>
T& HashMap<Key, T, Hash, Allocator>::HashBucket::get_ref(const key_type &key, size_type &size){
    auto node_ptr = head_;
    while (node_ptr != nullptr)
    {
        if (node_ptr->check_key(key)){
            return node_ptr->get_ref_mapped();
        }
        node_ptr = node_ptr->get_next();
    }
    node_ptr = new HashNode(std::make_pair(key, mapped_type{}));
    if (head_ == nullptr){
        head_ = node_ptr;
    }
    ++size;
    return node_ptr->get_ref_mapped();
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashNode* HashMap<Key, T, Hash, Allocator>::HashBucket::get_node(const key_type &key, size_type &size){
    auto node_ptr = head_;
    while (node_ptr != nullptr)
    {
        if (node_ptr->check_key(key)){
            return node_ptr;
        }
        node_ptr = node_ptr->get_next();
    }
    node_ptr = new HashNode(std::make_pair(key, mapped_type{}));
    if (head_ == nullptr){
        head_ = node_ptr;
    }
    ++size;
    return node_ptr;
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashNode* HashMap<Key, T, Hash, Allocator>::HashBucket::get_head(){
    return head_;
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashMap(
        size_type capacity,
        const hasher& hash_function,
        const allocator_value& allocator) :
    size_(0),
    hasher_(hash_function),
    allocator_(allocator),
    alloc_bucket_(allocator_bucket{}),
    alloc_node_(allocator_node{})
{
    if (capacity < 4){
        capacity_ = 4;
    } else{
        capacity_ = capacity;
    }
    // arr_ = new HashBucket[capacity_];
    arr_ = std::allocator_traits<allocator_bucket>::allocate(alloc_bucket_, capacity_);
    for (int i = 0; i < capacity_; ++i){
        arr_[i] = HashBucket();
    }
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashMap(const HashMap<key_type, T, Hash, Allocator> &other)
{
    capacity_ = other.capacity_;
    size_     = other.size_;
    hasher_   = other.hasher_;

    arr_ = std::allocator_traits<allocator_bucket>::allocate(alloc_bucket_, capacity_);
    for (int i = 0; i < capacity_; ++i){
        arr_[i] = HashBucket();
    }
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::~HashMap()
{
    std::unique_lock<std::shared_timed_mutex> lock(mutex_);
    clear();
}


template<class Key, class T, class Hash, class Allocator>
size_t HashMap<Key, T, Hash, Allocator>::get_hash(const key_type &key) const{
    return hasher_(key) % capacity_;
}

template<class Key, class T, class Hash, class Allocator>
std::optional<T> HashMap<Key, T, Hash, Allocator>::get(const key_type &key)
{
    std::shared_lock<std::shared_timed_mutex> lock(mutex_);
    return arr_[get_hash(key)].get(key);
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::size_type HashMap<Key, T, Hash, Allocator>::size() const noexcept
{
    return size_;
}


template<class Key, class T, class Hash, class Allocator>
bool HashMap<Key, T, Hash, Allocator>::empty() const noexcept
{
    return size_ == 0;
}


template<class Key, class T, class Hash, class Allocator>
T& HashMap<Key, T, Hash, Allocator>::operator[](const key_type &key){
    std::unique_lock<std::shared_timed_mutex> lock(mutex_);
    mapped_type& result = arr_[get_hash(key)].get_ref(key, size_);
    return result;
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::iterator HashMap<Key, T, Hash, Allocator>::begin() noexcept{
    size_t index = 0;
    HashNode* node_ptr;
    while (index < capacity_){
        node_ptr = arr_[index].get_head();
        if (node_ptr != nullptr){
            return iterator(node_ptr, index, this);
        } else{
            ++index;
        }
    }
    return iterator(node_ptr, index, this);
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::iterator HashMap<Key, T, Hash, Allocator>::end() noexcept{
    return iterator(nullptr, capacity_, this);
}


template<class Key, class T, class Hash, class Allocator>
class HashMap<Key, T, Hash, Allocator>::iterator{
private:
    HashNode* node_;
    size_t index_;
    friend class HashMap<Key, T, Hash, Allocator>;
    HashMap<Key, T, Hash, Allocator>* map_;

public:
    iterator() = delete;
    explicit iterator(HashNode* node, size_t index, HashMap<Key, T, Hash, Allocator>* map) : node_(node), index_(index), map_(map){}
    explicit iterator(const iterator &it):
        node_(it.node_),
        index_(it.index_),
        map_(it.map_){}
    
    value_type& operator*() noexcept{
        return node_->get_value();
    }

    iterator& operator++(){
        node_ = node_->get_next();
        if (node_ == nullptr){
            ++index_;
            while (index_ < map_->capacity_){
                node_ = map_->arr_[index_].get_head();
                if (node_ == nullptr){
                    ++index_;
                } else{
                    break;
                }
            }
        }
        return *this;
    }
    bool operator==(const iterator &it){
        return node_ == it.node_ && index_ == it.index_;
    }

    bool operator!=(const iterator &it){
        return !(node_ == it.node_ && index_ == it.index_);
    }
};

template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::iterator HashMap<Key, T, Hash, Allocator>::insert(const key_type& key, const mapped_type& value) noexcept{
    // std::unique_lock lock(mutex_);
    std::unique_lock lock(mtx_);
    size_t index = get_hash(key);
    auto result = arr_[index].get_node(key, size_);
    result->set_mapped(value);

    if (size_ > capacity_ * rehash_ratio_){
        rehash();
    }
    result = arr_[get_hash(key)].get_node(key, size_);
    return iterator(result, get_hash(key), this);
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::rehash(){
    auto new_capacity = capacity_ * 2;
    auto new_alloc = allocator_bucket{};
    HashBucket* new_arr = std::allocator_traits<allocator_bucket>::allocate(new_alloc, new_capacity);
    for (int i = 0; i < new_capacity; ++i){
        new_arr[i] = HashBucket();
    }
    size_t new_size = 0;
    for (auto it = begin(); it != end(); ++it){
        auto key = (*it).first;
        auto value = (*it).second;
        size_t index = get_hash(key);
        new_arr[index].get_node(key, new_size)->set_mapped(value);
    }
    
    clear();
    arr_ = new_arr;
    capacity_ = new_capacity;
    size_ = new_size;
    alloc_bucket_ = new_alloc;
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::clear(){
    // std::allocator_traits<allocator_bucket>::destroy(alloc_bucket_, arr_);
    std::allocator_traits<allocator_bucket>::deallocate(alloc_bucket_, arr_, capacity_);
    // delete[] arr_;
}