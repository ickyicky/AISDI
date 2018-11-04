#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cmath>
#define VECTOR_STARTINGvector_size 4
#define VECTOR_SCALE 2

namespace aisdi
{

template <typename Type>
class Vector
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
  size_type vector_size;
  size_type buffer_size;
  pointer buffer;

  void reserve(size_type multipler = VECTOR_SCALE)
  {
      buffer_size *= multipler;
      pointer new_buffer = new value_type[buffer_size];
      for(size_type i=0; i<vector_size; i++)
        new_buffer[i] = buffer[i];

      delete[] buffer;
      buffer = new_buffer;
  }

public:
  Vector():
    vector_size(0), buffer_size(VECTOR_STARTINGvector_size)
  {
    buffer = new value_type[buffer_size];
  }

  Vector(std::initializer_list<Type> l)
  {
    vector_size = l.size();
    buffer_size = vector_size;
    buffer = new value_type[ buffer_size ];
    auto to_be_copied = l.begin();
    for( size_type i = 0; i < vector_size; i++ )
      buffer[i] = *(to_be_copied++);
  }

  Vector(const Vector& other)
  {
      vector_size = other.vector_size;
      buffer_size = other.buffer_size;
      buffer = new value_type[buffer_size];
      for(size_type i=0; i< vector_size; i++)
        buffer[i] = other.buffer[i];
  }

  Vector(Vector&& other)
  {
      vector_size = other.vector_size;
      buffer_size = other.buffer_size;
      buffer = other.buffer;
      other.buffer = nullptr;
      other.buffer_size = 0;
      other.vector_size = 0;
  }

  ~Vector()
  {
    delete[] buffer;
  }

  Vector& operator=(const Vector& other)
  {
    if(this == &other)
      return *this;
    delete[] buffer;
    vector_size = other.vector_size;
    buffer_size = other.buffer_size;
    buffer = new value_type[buffer_size];
    for(size_type i=0; i< vector_size; i++)
        buffer[i] = other.buffer[i];
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    if(this == &other)
      return *this;
    vector_size = other.vector_size;
    buffer_size = other.buffer_size;
    delete[] buffer;
    buffer = other.buffer;
    other.buffer = nullptr;
    other.buffer_size = 0;
    other.vector_size = 0;
    return *this;
  }

  bool isEmpty() const
  {
    return vector_size==0;
  }

  size_type getSize() const
  {
    return vector_size;
  }

  void append(const Type& item)
  {
    if(buffer_size == vector_size) reserve();

    buffer[vector_size++] = item;
  }

  void prepend(const Type& item)
  {
    if(buffer_size == vector_size) reserve();
    //when there's some space left, we just move items to next
    //position and insert prepended item as a first one
    for(size_type i = vector_size; i > 0; i--)
        buffer[i] = buffer[i-1];

    buffer[0] = item;
    vector_size++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {

    if(insertPosition == end())
    {
      append(item);
    }
    else if(insertPosition == begin())
    {
      prepend(item);
    }
    else
    {
      if(buffer_size == vector_size) reserve();
      iterator position = Iterator(insertPosition);
      auto current = end();
      vector_size++;
      position = Iterator(insertPosition);

      while(current != position)
         {
             *current = *(current-1);
             current--;
         }

      buffer[insertPosition.getIndex()] = item;
    }
  }

  Type popFirst()
  {
    if(isEmpty()) throw std::logic_error("EMPTY VECTOR");
    value_type temp = buffer[0];
    for(size_type i = 1; i < vector_size; i++)
      buffer[i-1] = buffer[i];
    vector_size--;
    return temp;
  }

  Type popLast()
  {
    if(isEmpty()) throw std::logic_error("EMPTY VECTOR");
    value_type temp = buffer[--vector_size];
    return temp;
  }

  void erase(const const_iterator& possition)
  {
    auto current = Iterator(possition);
    while(current != end() - 1)
    {
      *current = *(current+1);
      current ++;
    }

    vector_size--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if( firstIncluded == begin() && lastExcluded == end() )
      vector_size = 0;
    else if ( firstIncluded == lastExcluded )
    {}
    else
    {
      //moving items
      iterator where_to_copy = Iterator(lastExcluded);
      iterator where_to_paste = Iterator(firstIncluded);

      while(where_to_copy != end())
        *(where_to_paste++) = *(where_to_copy++);

      where_to_paste = Iterator(firstIncluded);
      size_type removed = 0;
      while(where_to_paste != lastExcluded)
      {
        removed++;
        where_to_paste++;
      }
      vector_size-=removed;
    }
  }

  iterator begin()
  {
    return Iterator(buffer,0,&vector_size);
  }

  iterator end()
  {
    return Iterator(buffer + vector_size,vector_size,&vector_size);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(buffer,0,&vector_size);
  }

  const_iterator cend() const
  {
    return ConstIterator(buffer + vector_size,vector_size,&vector_size);
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
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
  using size_type = typename Vector::size_type;

private:
  pointer position;
  size_type index;
  const size_type* max_index;



public:
  explicit ConstIterator(pointer init_position = nullptr, size_type index = 0, const size_type* maxindex = nullptr):
      position(init_position), index(index), max_index(maxindex)
  {}

  size_type getIndex() const
  {
    return index;
  }

  reference operator*() const
  {
    if(index>=*max_index) throw std::out_of_range("OUT OF RANGE");
    return *position;
  }

  ConstIterator& operator++()
  {
    if(index==*max_index) throw std::out_of_range("OUT OF RANGE");
    position++;
    index++;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if(index==*max_index) throw std::out_of_range("OUT OF RANGE");
    ConstIterator old = ConstIterator(position,index,max_index);
    index++;
    position++;
    return old;
  }

  ConstIterator& operator--()
  {
    if(index==0) throw std::out_of_range("OUT OF RANGE");
    index--;
    position--;
    return *this;
  }

  ConstIterator operator--(int)
  {
    if(index==0) throw std::out_of_range("OUT OF RANGE");
    ConstIterator old = ConstIterator(position,index,max_index);
    index--;
    position--;
    return old;
  }

  ConstIterator operator+(difference_type d) const
  {
    if(index + d > *max_index) throw std::out_of_range("OUT OF RANGE");
    return ConstIterator(position + d,index + d, max_index);
  }

  ConstIterator operator-(difference_type d) const
  {
    if(index - d < 0) throw std::out_of_range("OUT OF RANGE");
    return ConstIterator(position - d,index - d, max_index);
  }

  bool operator==(const ConstIterator& other) const
  {
    return index == other.index;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return index != other.index;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(pointer init_position = nullptr, size_type index = 0, const size_type* maxindex = nullptr)
    : ConstIterator(init_position,index,maxindex)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  size_type getIndex()
  {
    return ConstIterator::getIndex();
  }

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

#endif // AISDI_LINEAR_VECTOR_H
