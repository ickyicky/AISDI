#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H
#define MAX_HASH 2047

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  class Node
  {
  public:
    value_type data;
    size_type hash;
    Node* previous;
    Node* next;
    Node(value_type data, size_type hash, Node* previous=nullptr, Node* next=nullptr):
          data(data), hash(hash), previous(previous), next(next)
    {}
  };

  Node** buffer;
  size_type size;
  size_type biggest_hash;
  size_type smallest_hash;
  size_type max_hash;

  size_type hashfun(key_type key) const
  {
    auto hash = std::hash<key_type>{}(key) % MAX_HASH;
    return hash;
  }

  Node* ffind(key_type key) const
  {
    size_type hash = hashfun(key);
    auto found = buffer[hash];
    while(1)
    {
      if(found == nullptr) return nullptr;
      if(found->data.first == key) break;
      found = found->next;
    }
    return found;
  }

  Node* insert(key_type key)
  {
    value_type value = value_type(key, mapped_type{});
    return insert(value);
  }

  Node* insert(value_type value)
  {
    size_type hash = hashfun(value.first);
    if(hash > biggest_hash) biggest_hash = hash;
    if(hash < smallest_hash) smallest_hash = hash;
    Node* newnode = new Node(value,hash,nullptr,nullptr);
    if(buffer[hash] != nullptr)
    {
      buffer[hash]->previous = newnode;
      newnode->next = buffer[hash];
    }
    buffer[hash] = newnode;
    size++;
    return newnode;
  }

  void clear()
  {
    for(size_type i = 0; i < MAX_HASH; i++)
    {
        Node* current = buffer[i];
        while(1)
        {
            if(current == nullptr) break;
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        buffer[i] = nullptr;
    }
    size = biggest_hash = 0;
    smallest_hash = MAX_HASH-1;
  }

public:
  HashMap()
  {
    size = 0;
    biggest_hash = 0;
    smallest_hash = MAX_HASH-1;
    max_hash = MAX_HASH;
    buffer = new Node*[max_hash];
    for(size_type i =0;i<max_hash;i++)
      buffer[i] = nullptr;
  }

  HashMap(std::initializer_list<value_type> list): size(0), biggest_hash(0),smallest_hash(MAX_HASH-1), max_hash(MAX_HASH)
  {
    buffer = new Node*[max_hash];
    for(size_type i =0;i<max_hash;i++)
      buffer[i] = nullptr;
    for(auto i=list.begin(); i!=list.end(); i++ )
      insert(*i);
  }

  HashMap(const HashMap& other): size(0), biggest_hash(0),smallest_hash(MAX_HASH-1), max_hash(MAX_HASH)
  {
    buffer = new Node*[max_hash];
    for(size_type i =0;i<max_hash;i++)
      buffer[i] = nullptr;
    for(auto i=other.begin(); i!=other.end(); i++ )
      insert(*i);
  }

  HashMap(HashMap&& other):
  buffer(other.buffer), size(other.size), biggest_hash(other.biggest_hash),smallest_hash(other.smallest_hash), max_hash(other.max_hash)
  {
    other.buffer = nullptr;
    other.size = other.max_hash = other.biggest_hash = other.smallest_hash = 0;
  }

  HashMap& operator=(const HashMap& other)
  {
    if(&other == this) return *this;
    clear();
    for(auto i=other.begin(); i!=other.end(); i++ )
      insert(*i);
    return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
    if(&other != this)
    {
        clear();
        buffer = other.buffer;
        size= other.size;
        biggest_hash = other.biggest_hash;
        smallest_hash = other.smallest_hash;
    }
    other.buffer = nullptr;
    other.size = other.max_hash = other.biggest_hash = other.smallest_hash = 0;

    return *this;
  }

  bool isEmpty() const
  {
    return size==0;
  }

  mapped_type& operator[](const key_type& key)
  {
    auto found = ffind(key);
    if(found == nullptr)
      found = insert(key);

    return found->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    auto found = ffind(key);
    if(found == nullptr)
      throw std::out_of_range("out");

    return found->data.second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    auto found = ffind(key);
    if(found == nullptr)
      throw std::out_of_range("out");

    return found->data.second;
  }

  const_iterator find(const key_type& key) const
  {
    return const_iterator(ffind(key), buffer);
  }

  iterator find(const key_type& key)
  {
    return iterator(ffind(key), buffer);
  }

  void remove(const key_type& key)
  {
    auto to_remove = ffind(key);
    if(to_remove == nullptr)
      throw std::out_of_range("out");

    if(to_remove->previous == nullptr)
      buffer[to_remove->hash] = to_remove->next;
    else
      to_remove->previous = to_remove->next;

    if(to_remove->next != nullptr)
      to_remove->next = to_remove->previous;

    delete to_remove;
    size--;
  }

  void remove(const const_iterator& it)
  {
    remove(it->first);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    auto i1 = begin();
    auto i2 = other.begin();
    while(1)
    {
      if(i1 != i2) return 0;
      if((i1 == end()) && (i2 == other.end())) return 1;
      if((i1 == end()) || (i2 == other.end())) return 0;
      i1++;
      i2++;
    }
    return 0;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    if(isEmpty()) return end();
    return iterator(buffer[smallest_hash],buffer);
  }

  iterator end()
  {
    return iterator(nullptr,buffer);
  }

  const_iterator cbegin() const
  {
    if(isEmpty()) return cend();
    return const_iterator(buffer[smallest_hash],buffer) ;
  }

  const_iterator cend() const
  {
    return const_iterator(nullptr,buffer) ;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

private:
  Node* node;
  friend class HashMap<KeyType, ValueType>;
  Node** buffer;

public:

  explicit ConstIterator(Node* node=nullptr, Node** buffer=nullptr): node(node), buffer(buffer)
  {}

  ConstIterator(const ConstIterator& other)
  {
    node = other.node;
    buffer = other.buffer;
  }

  ConstIterator& operator++()
  {
    if(node == nullptr)
      throw std::out_of_range("out");

    if(node->next == nullptr)
    {
      auto hash = node->hash + 1;
      while(buffer[hash] == nullptr)
      {
        if(hash >= MAX_HASH-1)
        {
          node = nullptr;
          return *this;
        }
        hash++;
      }
      node = buffer[hash];
    }
    else node = node->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto old = ConstIterator(node, buffer);
    operator++();
    return old;
  }

  ConstIterator& operator--()
  {
    if(node == nullptr)
    {
      size_t i = MAX_HASH - 1;
      while(i-- > 0)
      {
        if(buffer[i] == nullptr)
          continue;
        auto x = buffer[i];
        while(x->next != nullptr)
          x = x->next;
        node = x;
        return *this;
      }
      throw std::out_of_range("out");
    }

    if(node->previous == nullptr)
    {
      auto hash = node->hash - 1;
      while(buffer[hash] == nullptr)
      {
        if(hash<=0)
        {
          hash = 0;
          break;
        }
        hash--;
      }
      node = buffer[hash];
    }
    else node = node->previous;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto old = ConstIterator(node, buffer);
    operator--();
    return old;
  }

  reference operator*() const
  {
    if(node == nullptr)
      throw std::out_of_range("out_of_range");
    return node->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    bool iif = 1;
    if(node == nullptr || other.node == nullptr)
    {
      if(!(node == nullptr && other.node == nullptr))
        iif = 0;
    }
    else
      iif = node->data == other.node->data;

    return iif;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator(Node* node=nullptr, Node** buffer=nullptr) : ConstIterator(node, buffer)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
