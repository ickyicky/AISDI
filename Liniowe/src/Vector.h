#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#define VECTOR_STARTING_SIZE 4
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
  size_type _size;
  size_type _buffer_size;
  pointer buffer;
  void reserce(size_type);

public:





  void reserve(size_type multipler = VECTOR_SCALE)
  {
      _buffer_size *= multipler;
      pointer new_buffer = new value_type[_buffer_size];
      for(int i=0; i<_size; i++)
        new_buffer[i] = buffer[i];

      delete[] buffer;
      buffer = new_buffer;
  }



  Vector():
    _size(0), _buffer_size(VECTOR_STARTING_SIZE)
  {
    buffer = new value_type[_buffer_size];
  }

  Vector(std::initializer_list<Type> l)
  {
    (void)l; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  Vector(const Vector& other)
  {
      _size = other._size;
      _buffer_size = other._buffer_size;
      buffer = new value_type[_buffer_size];
      for(int i=0; i< _size; i++)
        buffer[i] = other.buffer[i];
  }

  Vector(Vector&& other)
  {
      _size = other._size;
      _buffer_size = other._buffer_size;
      buffer = other.buffer;
      other.buffer = nullptr;
      other._buffer_size = 0;
      other._size = 0;
  }

  ~Vector()
  {
      delete[] buffer;
  }

  Vector& operator=(const Vector& other)
  {
    delete[] buffer;
    _size = other._size;
    _buffer_size = other._buffer_size;
    buffer = new value_type[_buffer_size];
    for(int i=0; i< _size; i++)
      buffer[i] = other.buffer[i];
  }

  Vector& operator=(Vector&& other)
  {
      _size = other._size;
      _buffer_size = other._buffer_size;
      delete[] buffer;
      buffer = other.buffer;
      other.buffer = nullptr;
      other._buffer_size = 0;
      other._size = 0;
  }

  bool isEmpty() const
  {
    return _size==0;
  }

  size_type getSize() const
  {
    return _size;
  }

  void append(const Type& item)
  {
    if(_buffer_size == _size) reserve();

    buffer[_size++] = item;
  }

  void prepend(const Type& item)
  {
    if(_buffer_size == _size) reserve();
    //when there's some space left, we just move items to next
    //position and insert prepended item as a first one
    for(int i=_size; i>0; i--)
        buffer[i] = buffer[i-1];

    buffer[0] = item;
    _size++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
      if(_buffer_size == _size) reserve();
      iterator position = Iterator(insertPosition);
      *position = item;
      position++;
      while(position != end()+1)
        {
            *(position+1) = *position;
            position++;
        }
    _size++;
  }

  Type popFirst()
  {
    if(isEmpty()) throw std::logic_error("EMPTY VECTOR");
    value_type temp = buffer[0];
    iterator position = begin();
    while(position != end())
    {
        *position = *(position+1);
        position++;
    }
    _size--;
    return temp;
  }

  Type popLast()
  {
    if(isEmpty()) throw std::logic_error("EMPTY VECTOR");
    value_type temp = buffer[--_size];
    return temp;
  }

  void erase(const const_iterator& possition)
  {
    iterator position = Iterator(possition);
    while(position != end())
    {
        *position = *(position + 1);
        position++;
    }

    _size--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    iterator position = Iterator(firstIncluded);
    iterator to_erase = Iterator(lastExcluded);
    while(position != lastExcluded)
    {
        if(to_erase != end())
        {
            *position = *(to_erase);
            to_erase++;
        }
        position++;
        _size--;
    }
  }

  iterator begin()
  {
    return Iterator(buffer);
  }

  iterator end()
  {
    return Iterator(buffer + _size);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(buffer);
  }

  const_iterator cend() const
  {
    return ConstIterator(buffer + _size);
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

  pointer position;

  explicit ConstIterator(pointer init_position):
      position(init_position)
  {}

  reference operator*() const
  {
    return *position;
  }

  ConstIterator& operator++()
  {
    position++;
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator old = ConstIterator(this->position);
    position++;
    return old;
  }

  ConstIterator& operator--()
  {
    position--;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator old = ConstIterator(this->position);
    position--;
    return old;
  }

  ConstIterator operator+(difference_type d) const
  {
    return ConstIterator(position + d);
  }

  ConstIterator operator-(difference_type d) const
  {
    return ConstIterator(position - d);
  }

  bool operator==(const ConstIterator& other) const
  {
    return position == other.position;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return position != other.position;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(pointer position)
    : ConstIterator(position)
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

#endif // AISDI_LINEAR_VECTOR_H
