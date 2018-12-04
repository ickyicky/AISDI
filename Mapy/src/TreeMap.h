#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
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
  class node
  {
  public:
    value_type data;
    node* left;
    node* right;
    node* parent;

    node(value_type data, node* left=nullptr, node* right=nullptr, node* parent=nullptr):
          data(data), left(left), right(right), parent(parent)
    {}

  };

  node* root;
  size_type size;
  node* ffind(const value_type v)
  {
    if(isEmpty())
    {
      root = new node(v);
      size++;
      return root;
    }

    auto current = root;
    while(current != nullptr)
    {
      auto backup = current;
      if(current->data.first == v.first) break;
      if(current->data.first > v.first)
      {
        current = current->left;
        if(current == nullptr)
        {
          current = new node(v,nullptr,nullptr,backup);
          backup->left = current;
          size++;
          break;
        }
      }
      else
      {
        current = current->right;
        if(current == nullptr)
        {
          current = new node(v,nullptr,nullptr,backup);
          backup->right = current;
          size++;
          break;
        }
      }
    }

    return current;
  }

  node* ffind(const key_type key)
  {
    mapped_type map{};
    value_type v(key,map);
    return ffind(v);
  }

  node* ofind(const key_type key) const
  {
    auto current = root;
    while(current != nullptr)
    {
      if(current->data.first == key) break;
      if(current->data.first > key)
        current = current->left;
      else
        current = current->right;
    }
    return current;
  }

  void balance(node* x)
  {
    (void) x;
  }

  node* minimum(node* x) const
  {
    if(isEmpty()) return nullptr;
    while(x->left != nullptr)
      x = x->left;
    return x;
  }

  node* maximum(node* x) const
  {
    if(isEmpty()) return nullptr;
    while(x->right != nullptr)
      x = x->right;
    return x;
  }

  void clear()
  {
    remover(root);
    size = 0;
    root = nullptr;
  }

  void remover(node* x)
  {
    if(x == nullptr) return;
    remover(x->left);
    remover(x->right);
    delete x;
  }

public:

  TreeMap(): root(nullptr), size(0)
  {}

  TreeMap(std::initializer_list<value_type> list): root(nullptr), size(0)
  {
    for(auto i = list.begin(); i != list.end(); i++)
      ffind(*i);
  }

  TreeMap(const TreeMap& other): root(nullptr), size(0)
  {
    for(auto i = other.begin(); i != other.end(); i++)
      ffind(*i);
  }

  TreeMap(TreeMap&& other)
  {
    root = other.root;
    other.root = nullptr;
    size = other.size;
    other.size = 0;
  }

  TreeMap& operator=(const TreeMap& other)
  {
    clear();
    for(auto i = other.begin(); i != other.end(); i++)
      ffind(*i);
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    clear();
    root = other.root;
    other.root = nullptr;
    size = other.size;
    other.size = 0;
    return *this;
  }

  bool isEmpty() const
  {
    return root==nullptr;
  }

  mapped_type& operator[](const key_type& key)
  {
    auto found = ffind(key);
    return found->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    return valueOf(key);
  }

  mapped_type& valueOf(const key_type& key)
  {
    auto found = ofind(key);
    if(found == nullptr)
      throw std::out_of_range("out_of_range");
    return found->data.second;
  }

  const_iterator find(const key_type& key) const
  {
    return const_iterator(ofind(key));
  }

  iterator find(const key_type& key)
  {
    return iterator(ofind(key));
  }

  void remove(const key_type& key)
  {
    node* to_delete = ffind(key);
    node* right = to_delete->right;
    node* left = to_delete->left;

    if(right != nullptr)
    {
      if(to_delete == root) root = right;
      else if(to_delete->parent->right == to_delete) to_delete->parent->right = right;
      else to_delete->parent->left = right;

      if(left != nullptr)
      {
        maximum(left)->right = right->left;
        right->left = left;
        left->parent = right;
        balance(right);
      }
    }
    else
    {
      if(to_delete == root) root = left;
      else if(to_delete->parent->right == to_delete) to_delete->parent->right = left;
      else to_delete->parent->left = left;
    }
  }

  void remove(const const_iterator& it)
  {
    remove(it.pointed->data.first);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const TreeMap& other) const
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

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return iterator(minimum(root));
  }

  iterator end()
  {
    return iterator(nullptr,maximum(root));
  }

  const_iterator cbegin() const
  {
    return const_iterator(minimum(root));
  }

  const_iterator cend() const
  {
    return const_iterator(nullptr,maximum(root));
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
private:
  friend class TreeMap<KeyType, ValueType>;
  node* pointed;
  node* maximal;
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  explicit ConstIterator(node* pointed, node* maximal=nullptr): pointed(pointed), maximal(maximal)
  {}

  ConstIterator(const ConstIterator& other)
  {
    pointed = other.pointed;
  }

  ConstIterator& operator++()
  {
    if(pointed == nullptr)
      throw std::out_of_range("out_of_range");
    if(pointed->right != nullptr)
    {
      pointed = pointed->right;
      while(pointed->right != nullptr)
        pointed = pointed->right;
      return *this;
    }
    while(pointed->parent != nullptr)
    {
      node* previous = pointed;
      pointed = pointed->parent;
      if(pointed->left == previous)
        return *this;
    }
    pointed = nullptr;
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto old = ConstIterator(pointed,maximal);
    operator++();
    return old;
  }

  ConstIterator& operator--()
  {
  if(pointed == nullptr)
  {
      pointed = maximal;
      return *this;
  }

  if(pointed->left != nullptr)
  {
    pointed = pointed->left;
    while(pointed->left != nullptr)
      pointed = pointed->left;
    return *this;
  }
  while(pointed->parent != nullptr)
  {
    node* previous = pointed;
    pointed = pointed->parent;
    if(pointed->right == previous)
      return *this;
  }

  throw std::out_of_range("out_of_range");

  }

  ConstIterator operator--(int)
  {
    auto old = ConstIterator(pointed,maximal);
    operator--();
    return old;
    }

    throw std::out_of_range("out_of_range");
  }

  reference operator*() const
  {
    if(pointed == nullptr)
      throw std::out_of_range("out_of_range");
    return pointed->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return pointed == other.pointed;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator(node* pointed, node* maximal=nullptr): ConstIterator(pointed, maximal)
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

#endif /* AISDI_MAPS_MAP_H */
