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
    short balance_factor;

    node(value_type data, node* left=nullptr, node* right=nullptr, node* parent=nullptr):
          data(data), left(left), right(right), parent(parent), balance_factor(0)
    {}

  };

  node* root;
  size_type size;
  node* ffind(const value_type& v)
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
        current = current->left;
      else
        current = current->right;

      if(current == nullptr)
      {
        current = new node(v, nullptr, nullptr, backup);
        size++;

        if(backup->data.first > v.first)
          backup->left = current;
        else
          backup->right = current;

        while(backup != nullptr)
        {
          if(current == backup->right)
            ++backup->balance_factor;
          else
            --backup->balance_factor;

          if(backup->balance_factor == 2)
          {
            if(current->balance_factor == 1)
              rotate_left(backup);
            else
              rotate_right_left(backup);
            break;
          }
          if(backup->balance_factor == -2)
          {
            if(current->balance_factor == -1)
              rotate_right(backup);
            else
              rotate_left_right(backup);
            break;
          }

          current = backup;
          backup = backup->parent;
        }
        return ofind(v.first);
      }
    }
    return root;
  }

  node* ffind(const key_type& key)
  {
    mapped_type map{};
    value_type v(key,map);
    return ffind(v);
  }

  node* ofind(const key_type& key) const
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


  void rotate_right_left(node* node)
  {
    rotate_right(node->left);
    rotate_left(node);
  }

  void rotate_left_right(node* node)
  {
    rotate_left(node->right);
    rotate_right(node);
  }

  void rotate_left(node* current)
  {
    if(current == nullptr)
      return;

    node *right = current->right;
    current->right = right->left;
    if(right->left != nullptr)
      right->left->parent = current;
    if(current == root)
      root = right;
    else if(current == current->parent->left)
      current->parent->left = right;
    else
      current->parent->right = right;
    right->left = current;
    right->parent = current->parent;
    current->parent = right;

    if(right->balance_factor == 1)
      current->balance_factor = 0;
    else
      current->balance_factor = -1;

    right->balance_factor--;

  }

  void rotate_right(node* current)
  {
    if(current == nullptr)
      return;

    node* left = current->left;
    current->left = left->right;
    if(left->right != nullptr)
      left->right->parent = current;
    if(current == root)
      root = left;
    else if(current == current->parent->right)
      current->parent->right = left;
    else
      current->parent->left = left;
    left->left = current;
    left->parent = current->parent;
    current->parent = left;

    if(left->balance_factor == -1)
      current->balance_factor = 0;
    else
      current->balance_factor = 1;

    left->balance_factor++;
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

  const const_iterator& unattach(const const_iterator& it)
  {
    node* to_delete;
    node* replacement;
    node* nested;
    node* temp;

    bool nest;
    to_delete = it.pointed;

    if(to_delete->right != nullptr && to_delete->left != nullptr)
    {
      auto previous = it;
      replacement = unattach(--previous).pointed;
      nest = false;
    }
    else
    {
      if(to_delete->left == nullptr)
      {
        replacement = to_delete->right;
        to_delete->right = nullptr;
      }
      else
      {
        replacement = to_delete->left;
        to_delete->left = nullptr;
      }
      to_delete->balance_factor = 0;
      nest = true;
    }

    if(replacement != nullptr)
    {
      replacement->parent = to_delete->parent;
      replacement->left = to_delete->left;

      if(replacement->left)
        replacement->left->parent = replacement;

      replacement->right = to_delete->right;
      if(replacement->right)
        replacement->right->parent = replacement;

      replacement->balance_factor = to_delete->balance_factor;
    }

    if(to_delete->parent == nullptr)
      root = replacement;
    else
    {
      if(to_delete->parent->left == to_delete)
        to_delete->parent->left = replacement;
      else
        to_delete->parent->right = replacement;
    }

    if(nest)
    {
      nested = replacement;
      replacement = to_delete->parent;
      while(replacement != nullptr)
      {
        if(replacement->balance_factor != 0)
        {
          if(replacement->left == nested)
            replacement->balance_factor = 1;
          else
            replacement->balance_factor = -1;
          break;
        }
        else
        {
          if( ( (replacement->balance_factor == 1) && (replacement->left == nested) )
           || ( (replacement->balance_factor == -1) && (replacement->right == nested) ) )
          {
            replacement->balance_factor = 0;
            nested = replacement;
            replacement = replacement->parent;
          }
          else
          {
            if(replacement->left == nested)
              temp = replacement->right;
            else
              temp = replacement->left;

            if(temp->balance_factor != 0)
            {
              if(replacement->balance_factor == -1)
                rotate_left(replacement);
              else
                rotate_right(replacement);
              break;
            }
            else if(replacement->balance_factor == temp->balance_factor)
            {
              if(replacement->balance_factor == -1)
                rotate_left(replacement);
              else
                rotate_right(replacement);
              nested = temp;
              replacement = temp->parent;
            }
            else
            {
              if(replacement->balance_factor == -1)
                rotate_left_right(replacement);
              else
                rotate_right_left(replacement);
              nested = replacement->parent;
              replacement = nested->parent;
            }
          }
        }
      }
    }
    return it;
  }

public:

  TreeMap(): root(nullptr), size(0)
  {}

  TreeMap(std::initializer_list<value_type> list): root(nullptr), size(0)
  {
    for(auto &i: list)
      ffind(i);
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

  ~TreeMap()
  {
    clear();
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(&other == this) return *this;
    clear();
    for(auto i = other.begin(); i != other.end(); i++)
      ffind(*i);
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    if(&other != this)
    {
        clear();
        root = other.root;
        size = other.size;
    }
    other.root = nullptr;
    other.size = 0;
    return *this;
  }

  bool isEmpty() const
  {
    return size==0;
  }

  mapped_type& operator[](const key_type& key)
  {
    auto found = ffind(key);
    return found->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    auto found = ofind(key);
    if(found == nullptr)
      throw std::out_of_range("out_of_range");
    return found->data.second;
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
    remove(find(key));
  }

  void remove(const const_iterator& it)
  {
    if(isEmpty())
      throw std::out_of_range("cant remove from empty map");
    if(it.pointed == nullptr)
      throw std::out_of_range("invalid key");

    delete unattach(it).pointed;
    size--;
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
    if(pointed == nullptr) throw std::out_of_range("incrementing end");
    else
    {
      if(pointed->right == nullptr)
      {
        node* prev = pointed;
        while(pointed->parent!=nullptr)
        {
            pointed=pointed->parent;
            if(pointed->left == prev) return *this;
            prev = pointed;
        }
      }
      else
      {
        pointed = pointed->right;
        if(pointed!=nullptr)
          while(pointed->left!=nullptr)
            pointed=pointed->left;
        return *this;
      }
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
      if(maximal == nullptr) throw std::out_of_range("decrementing begin");
      pointed = maximal;
      return *this;
    }
    else
    {
      if(pointed->left == nullptr)
      {
        node* prev = pointed;
        while(pointed->parent!=nullptr)
        {
            pointed=pointed->parent;
            if(pointed->right == prev) return *this;
            prev = pointed;
        }
      }
      else
      {
        pointed = pointed->left;
        if(pointed!=nullptr)
          while(pointed->right!=nullptr)
            pointed=pointed->right;
        return *this;
      }
    }
    throw std::out_of_range("decrementing begin");
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto old = ConstIterator(pointed,maximal);
    operator--();
    return old;

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
    if(pointed == nullptr || other.pointed == nullptr)
      return pointed == nullptr && other.pointed == nullptr;

    return pointed->data == other.pointed->data;
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
