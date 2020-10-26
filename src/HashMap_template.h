template< class Key, class T>
HashNode<Key, T>::HashNode(const Key &key, const T &value) :
        key_(key),
        value_(value)
{

}


template<class Key, class T>
bool HashNode<Key, T>::check_key(const Key &key) const
{
    return key_ == key;
}


template<class Key, class T>
Key HashNode<Key, T>::get_key() const
{
    return key_;
}


template<class Key, class T>
T HashNode<Key, T>::get_value() const
{
    return value_;
}


template<class Key, class T>
HashNode<Key, T>* HashNode<Key, T>::get_next() const
{
    return next_ptr_;
}


template<class Key, class T>
void HashNode<Key, T>::set_value(const T &value)
{
    value_ = value;
}


template<class Key, class T>
void HashNode<Key, T>::set_next(const HashNode<Key, T>* next)
{
    next_ptr_ = next;
}


template<class Key, class T>
HashNode<Key, T>::~HashNode()
{
    delete next_ptr_;
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashMap() :
        buffer_size_(0),
        size_(0),
        arr_(nullptr)
{
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashMap(size_t capacity) :
        buffer_size_(capacity),
        size_(0)
{
    arr_ = new HashNode<Key, T>* [buffer_size_];
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::~HashMap()
{
    delete[] arr_;
}

template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashMap(const HashMap<Key, T, Hash, Allocator> &other)
{
    buffer_size_ = other.buffer_size_;
    size_        = other.size_;
    hasher_      = other.hasher_;
}









