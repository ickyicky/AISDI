#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  class node;
  node* first;
  node* last;
  size_type _size;

  LinkedList():
      first(nullptr),
      last(nullptr),
      _size(0)
  {}

  LinkedList(std::initializer_list<Type> l)
  {
    (void)l; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  LinkedList(const LinkedList& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  LinkedList(LinkedList&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  ~LinkedList()
  {}

  LinkedList& operator=(const LinkedList& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  LinkedList& operator=(LinkedList&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    return _size == 0;
  }

  size_type getSize() const
  {
    return _size;
  }

  void append(const Type& item)
  {
    if(isEmpty())
        first = last = new node(item);
    else
    {
        new_node = new node(item, previous = last);
        last->next = new_node;
        last = new_node;
    }
    _size++;
  }

  void prepend(const Type& item)
  {
    if(isEmpty())
        first = last = new node(item);
    else
    {
        new_node = new node(item, next = first);
        first->previous = new_node;
        first = new_node;
    }
    _size++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    (void)insertPosition;
    (void)item;
    throw std::runtime_error("TODO");
  }

  Type popFirst()
  {
    throw std::runtime_error("TODO");
  }

  Type popLast()
  {
    throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& possition)
  {
    (void)possition;
    throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    (void)firstIncluded;
    (void)lastExcluded;
    throw std::runtime_error("TODO");
  }

  iterator begin()
  {
    throw std::runtime_error("TODO");
  }

  iterator end()
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
    throw std::runtime_error("TODO");
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

template <typename Type>
class LinkedList<Type>::node
{
public:
    Type item;
    using pointer = node*;
    pointer next;
    pointer previous;

    node(Type item; pointer previous = nullptr, pointer next = nullptr):
        item(item), previous(previous), next(next)
        {}
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  explicit ConstIterator()
  {}

  reference operator*() const
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator++()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator+(difference_type d) const
  {
    (void)d;
    throw std::runtime_error("TODO");
  }

  ConstIterator operator-(difference_type d) const
  {
    (void)d;
    throw std::runtime_error("TODO");
  }

  bool operator==(const ConstIterator& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const ConstIterator& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator()
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

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
