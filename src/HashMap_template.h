template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashNode::HashNode() = default;

template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashNode::HashNode(const node_pointer new_node)
{
    node_ = new_node;
}


template<class Key, class T, class Hash, class Allocator>
Key HashMap<Key, T, Hash, Allocator>::HashNode::get_key() const
{
    return node_->first;
}


template<class Key, class T, class Hash, class Allocator>
T HashMap<Key, T, Hash, Allocator>::HashNode::get_value() const
{
    return node_->second;
}


template<class Key, class T, class Hash, class Allocator>
typename HashMap<Key, T, Hash, Allocator>::HashNode* HashMap<Key, T, Hash, Allocator>::HashNode::get_next() const
{
    return next_ptr_;
}


template<class Key, class T, class Hash, class Allocator>
bool HashMap<Key, T, Hash, Allocator>::HashNode::check_key(const Key &key) const
{
    return node_->first == key;
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::HashNode::set_value(const T &value)
{
    node_->second = value;
}


template<class Key, class T, class Hash, class Allocator>
void HashMap<Key, T, Hash, Allocator>::HashNode::set_next(HashNode* next)
{
    next_ptr_ = next;
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashNode::~HashNode()
{
    delete node_;
    delete next_ptr_;
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashMap(const Allocator &node_allocator) :
        buffer_size_(0),
        size_(0),
        arr_(nullptr),
        allocator(node_allocator)
{
}


template<class Key, class T, class Hash, class Allocator>
HashMap<Key, T, Hash, Allocator>::HashMap(size_t capacity) :
        buffer_size_(capacity),
        size_(0),
        allocator(Allocator{})
{
    arr_ = new HashNode* [buffer_size_];
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

    arr_ = new HashNode* [buffer_size_];

}









