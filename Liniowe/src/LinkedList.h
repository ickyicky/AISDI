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

private:
  class Node;
  Node* first;
  Node* last;
  size_type list_size;

  void init()
  {
    list_size = 0;
    first = new Node();
    last = new Node(first,nullptr);
    first->next = last;
  }

public:
  LinkedList()
  {
    init();
  }

  LinkedList(std::initializer_list<Type> l)
  {
    init();
    auto position = l.begin();
    while( position != l.end() )
      append( *(position++) );
  }

  LinkedList(const LinkedList& other)
  {
    init();
    auto position = other.begin();
    while( position != other.end() )
      append( *(position++) );
  }

  LinkedList(LinkedList&& other)
  {
    list_size = other.list_size;
    first = other.first;
    last = other.last;
    other.list_size = 0;
    other.first = nullptr;
    other.last = nullptr;
  }

  ~LinkedList()
  {
    while(first != nullptr)
      {
        Node* temp = first;
        first = first->next;
        delete temp;
      }
      first = last = nullptr;
      list_size = 0;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(this == &other)
      return *this;
    Node* position = first->next;
    while(position != last)
    {
      Node* temp = position;
      position = position->next;
      delete temp;
    }
    last->previous = first;
    first->next = last;

    list_size = 0;
    auto other_position = other.begin();
    while( other_position != other.end() )
      append( *(other_position++) );
    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    if(this == &other)
      return *this;

    Node* position = first->next;
    while(position != last)
    {
      Node* temp = position;
      position = position->next;
      delete temp;
    }

    list_size = other.list_size;
    first = other.first;
    last = other.last;
    other.list_size = 0;
    other.first = other.last = nullptr;
    return *this;
  }

  bool isEmpty() const
  {
    return list_size == 0;
  }

  size_type getSize() const
  {
    return list_size;
  }

  void append(const Type& item)
  {
    Node* new_node = new Node(item,last->previous,last);
    last->previous->next = new_node;
    last->previous = new_node;

    list_size++;
  }

  void prepend(const Type& item)
  {
    Node* new_node = new Node(item, first, first->next);
    first->next->previous = new_node;
    first->next = new_node;

    list_size++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    Node* new_node = new Node(item, insertPosition.pointed->previous, insertPosition.pointed);
    Iterator new_iterator = insertPosition;
    new_iterator.pointed->previous->next = new_node;
    new_iterator.pointed->previous = new_node;

    list_size++;
  }

  Type popFirst()
  {
    if( list_size == 0 ) throw std::logic_error("EMPTY LINKED LIST");

    value_type value = first->next->item;
    Node* to_delete = first->next;
    first->next = to_delete->next;
    to_delete->next->previous = first;
    delete to_delete;

    list_size--;
    return value;
  }

  Type popLast()
  {
    if( list_size == 0 ) throw std::logic_error("EMPTY LINKED LIST");

    value_type value = last->previous->item;
    Node* to_delete = last->previous;
    last->previous = to_delete->previous;
    to_delete->previous->next = last;
    delete to_delete;

    list_size--;
    return value;
  }

  void erase(const const_iterator& possition)
  {
    if(list_size == 0 || possition.pointed == last
                      || possition.pointed == first)
                          throw std::out_of_range("OUT OF RANGE");

    Node* to_delete = possition.pointed;
    to_delete->previous->next = to_delete->next;
    to_delete->next->previous = to_delete->previous;
    delete to_delete;

    list_size--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(list_size == 0 || firstIncluded.pointed == last
                      || firstIncluded.pointed == first
                      || lastExcluded.pointed == first)
                          throw std::out_of_range("OUT OF RANGE");
    if(firstIncluded == lastExcluded) return;

    firstIncluded.pointed->previous->next = lastExcluded.pointed;
    lastExcluded.pointed->previous = firstIncluded.pointed->previous;

    Node* to_delete = firstIncluded.pointed;
    while(to_delete != lastExcluded.pointed && to_delete != last && to_delete!=nullptr)
    {
      Node* temp = to_delete;
      to_delete = to_delete->next;
      delete temp;
      list_size--;
    }
  }

  iterator begin()
  {
    return Iterator(first->next);
  }

  iterator end()
  {
    return Iterator(last);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(first->next);
  }

  const_iterator cend() const
  {
    return ConstIterator(last);
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
class LinkedList<Type>::Node
{
public:
    using npointer = Node*; //n pointer stands for node pointer
    Type item;
    npointer previous;
    npointer next;

    Node(npointer previous = nullptr, npointer next = nullptr):
        previous(previous), next(next)
        {}

    Node(const Type& item, npointer previous = nullptr, npointer next = nullptr):
        item(item), previous(previous), next(next)
        {}

    ~Node()
        {}
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
private:

public:
  Node* pointed;
  friend class LinkedList<Type>;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  explicit ConstIterator(Node* init_pointed = nullptr):
      pointed( init_pointed )
  {}

  reference operator*() const
  {
    if(pointed == nullptr || pointed->next == nullptr || pointed->previous == nullptr)
    {
        throw std::out_of_range("OUT OF RANGE");
    }
    else
    {
        return pointed->item;
    }
  }

  ConstIterator& operator++()
  {
    if(pointed == nullptr || pointed->next == nullptr)
        throw std::out_of_range("OUT OF RANGE");

    pointed = pointed->next;
    return *this;

  }

  ConstIterator operator++(int)
  {
    if(pointed == nullptr || pointed->next == nullptr)
        throw std::out_of_range("OUT OF RANGE");

    ConstIterator old = ConstIterator(pointed);
    pointed = pointed->next;
    return old;

  }

  ConstIterator& operator--()
  {
    if(pointed->previous->previous == nullptr)
        throw std::out_of_range("OUT OF RANGE");

    pointed = pointed->previous;
    return *this;
  }

  ConstIterator operator--(int)
  {
    if(pointed->previous->previous == nullptr)
        throw std::out_of_range("OUT OF RANGE");

    ConstIterator old = ConstIterator(pointed);
    pointed = pointed->previous;
    return old;
  }

  ConstIterator operator+(difference_type d) const
  {
    Node* temp_position = pointed;
    while(d > 0)
    {
      if(temp_position->next == nullptr)
          throw std::out_of_range("OUT OF RANGE");
      temp_position = temp_position->next;
      d--;
    }
    return ConstIterator(temp_position);
  }

  ConstIterator operator-(difference_type d) const
  {
    Node* temp_position = pointed;
    while(d > 0)
    {
      if(temp_position->previous->previous == nullptr)
          throw std::out_of_range("OUT OF RANGE");
      temp_position = temp_position->previous;
      d--;
    }
    return ConstIterator(temp_position);
  }

  bool operator==(const ConstIterator& other) const
  {

    return pointed == other.pointed;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return pointed != other.pointed;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node* node=nullptr):
    ConstIterator(node)
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
