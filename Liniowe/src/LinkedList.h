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
    first = last = new Node();
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
      append( *position++ );
  }

  LinkedList(const LinkedList& other)
  {
    init();
    auto position = other.begin();
    while( position != other.end() )
      append( *position++ );
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
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(this == &other)
      return *this;
    while(first != nullptr)
      {
        Node* temp = first;
        first = first->next;
        delete temp;
      }
    list_size = 0;
    auto position = other.begin();
    while( position != other.end() )
      append( *position++ );
    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    if(this == &other)
      return *this;
    while(first != nullptr)
      {
        Node* temp = first;
        first = first->next;
        delete temp;
      }
    list_size = other.list_size;
    first = other.first;
    last = other.last;
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
    Node* new_node = new Node(item);

    if(list_size == 0)
      first = new_node;
    else
    {
      last->previous->next = new_node;
      new_node->previous = last->previous;
    }

    last->previous = new_node;
    new_node->next = last;
    list_size++;
  }

  void prepend(const Type& item)
  {
    Node* new_node = new Node(item, nullptr, first);

    if(list_size == 0)
      last->previous = new_node;
    else
      first->next->previous = new_node;

    first = new_node;
    list_size++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    Node* new_node = new Node(item, insertPosition.pointed->previous, insertPosition.pointed);
    if(insertPosition.pointed->previous == nullptr)
      first = new_node;
    else
      insertPosition.pointed->previous->next = new_node;
    insertPosition.pointed->previous = new_node;
    list_size++;
  }

  Type popFirst()
  {
    if( list_size == 0 ) throw std::logic_error("EMPTY LINKED LIST");

    value_type value = first->item;
    Node* node_to_delete = first;
    if(list_size == 1)
      first = last;
    else
      first = first->next;

    first->previous = nullptr;
    delete node_to_delete;
    list_size--;
    return value;
  }

  Type popLast()
  {
    if( list_size == 0 ) throw std::logic_error("EMPTY LINKED LIST");

    value_type value = last->previous->item;
    Node* node_to_delete = last->previous;

    if(list_size == 1)
    {
      first = last;
      last->previous = nullptr;
    }
    else
    {
      last->previous = last->previous->previous;
      last->previous->previous->next = last;
    }
    delete node_to_delete;
    list_size--;
    return value;
  }

  void erase(const const_iterator& possition)
  {
    Node* node_to_delete = possition.pointed;

    if( possition.pointed == first )
        first = first->next;
    else
      possition.pointed->previous->next = possition.pointed->next;

    possition.pointed->next->previous = possition.pointed->previous;
    delete node_to_delete;
    list_size--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    Node* node_to_delete = firstIncluded.pointed;

    if( firstIncluded.pointed == first )
        first = lastExcluded.pointed;
    else
      firstIncluded.pointed->previous->next = lastExcluded.pointed;

    while(node_to_delete != lastExcluded.pointed)
      {
        Node* temp = node_to_delete;
        node_to_delete = node_to_delete->next;
        delete temp;
      }

    firstIncluded.pointed->next->previous = firstIncluded.pointed->previous;
    delete node_to_delete;
    list_size--;
  }

  iterator begin()
  {
    return Iterator(first);
  }

  iterator end()
  {
    return Iterator(last);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(first);
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
    using npointer = Node*;
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
  Node* pointed;
public:
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
    if(pointed->next == nullptr) throw std::out_of_range("OUT OF RANGE");
    return pointed->item;
  }

  ConstIterator& operator++()
  {
    if(pointed->next == nullptr) throw std::out_of_range("OUT OF RANGE");
    pointed = pointed->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if(pointed->next == nullptr) throw std::out_of_range("OUT OF RANGE");
    ConstIterator old = ConstIterator(pointed);
    pointed = pointed->next;
    return old;

  }

  ConstIterator& operator--()
  {
    if(pointed->previous == nullptr) throw std::out_of_range("OUT OF RANGE");
    pointed = pointed->previous;
    return *this;
  }

  ConstIterator operator--(int)
  {
    if(pointed->previous == nullptr) throw std::out_of_range("OUT OF RANGE");
    ConstIterator old = ConstIterator(pointed);
    pointed = pointed->previous;
    return old;
  }

  ConstIterator operator+(difference_type d) const
  {
    Node* temp_position = pointed;
    while(d > 0)
    {
      temp_position = temp_position->next;
      if(temp_position == nullptr) throw std::out_of_range("OUT OF RANGE");
      d--;
    }
    return ConstIterator(temp_position);
  }

  ConstIterator operator-(difference_type d) const
  {
    Node* temp_position = pointed;
    while(d > 0)
    {
      temp_position = temp_position->previous;
      if(temp_position == nullptr) throw std::out_of_range("OUT OF RANGE");
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
