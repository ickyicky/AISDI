#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <functional>//dla funkcji haszujacej
#include <vector>
#include <list>

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
  using list = std::list<value_type>;
  using vector = std::vector<list>;
  using vector_i = typename vector::const_iterator;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  vector hash_vector;
  size_type size;
  size_type max_hash;
  const double MAX_HASH_TO_SIZE = 1.5;
  const size_type START_HASH = 10;

  size_type hashfun(const key_type key) const
  {
    auto hash = std::hash<key_type>{}(key) % max_hash;
    return hash;
  }

  void resize()
  {
    if(size / max_hash <= MAX_HASH_TO_SIZE)
      return;

    //w innyn przypadku, analogicznie do 1 projektu, zwiekszam pojemnosc kontenera "hashy" dwukrotnie
    max_hash *= 2;
    vector new_hash_vector;
    new_hash_vector.resize(max_hash);
    for(auto& hash_list: hash_vector)
      for(auto& element: hash_list)
        new_hash_vector[element.first].push_back(element);

    hash_vector.clear();
    hash_vector = std::move(new_hash_vector);
  }

public:
  HashMap()
  {
    size = 0;
    max_hash = START_HASH;
    hash_vector.resize(max_hash);
  }

  HashMap(std::initializer_list<value_type> list)
  {
    size = 0;
    max_hash = list.size() > START_HASH? list.size() * 2: START_HASH;
    hash_vector.resize(max_hash);
    for(auto i = list.begin(); i != list.end(); i++)
      operator[]((*i).first) = (*i).second;
  }

  HashMap(const HashMap& other)
  {
    size = 0;
    max_hash = other.max_hash;
    hash_vector.resize(max_hash);
    for(auto i = other.begin(); i != other.end(); i++)
      operator[](i->first) = i->second;
  }

  HashMap(HashMap&& other)
  {
    size = 0;
    max_hash = START_HASH;
    hash_vector.resize(max_hash);
    std::swap(size, other.size);
    std::swap(max_hash, other.max_hash);
    std::swap(hash_vector, other.hash_vector);
  }

  HashMap& operator=(const HashMap& other)
  {
    if(this == &other)
      return *this;
    hash_vector.clear();
    max_hash = other.max_hash;
    hash_vector.resize(max_hash);
    size = 0;
    for(auto i = other.begin(); i != other.end(); i++)
      operator[](i->first) = i->second;

    return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
    if(this == &other)
      return *this;
    hash_vector.clear();
    size = 0;
    std::swap(size, other.size);
    std::swap(max_hash, other.max_hash);
    std::swap(hash_vector, other.hash_vector);
    return *this;
  }

  bool isEmpty() const
  {
    return size == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    auto found = find(key);
    if(found == end())
    {
      //jesli nie ma takiego elementu, wstawiamy:
      size_type hash = hashfun(key);

      hash_vector[hash].push_back(value_type{key, mapped_type{}});
      ++size;
      resize();
    }

    return valueOf(key);
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    auto found = find(key);
    if(found == cend())
      throw std::out_of_range("invalid key");
    return found->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    size_type hash = hashfun(key);
    for(auto& element: hash_vector[hash])
      if(element.first == key)
        return element.second;

    throw std::out_of_range("invalid key");
  }

  const_iterator find(const key_type& key) const
  {
    size_type hash = hashfun(key);

    for(auto i = hash_vector[hash].begin(); i != hash_vector[hash].end(); ++i)
      if((*i).first == key)
        return const_iterator(hash_vector.begin() + hash, i, hash_vector.begin(), hash_vector.end());

    return cend();
  }

  iterator find(const key_type& key)
  {
    size_type hash = hashfun(key);

    for(auto i = hash_vector[hash].begin(); i != hash_vector[hash].end(); ++i)
      if((*i).first == key)
        return iterator(hash_vector.begin() + hash, i, hash_vector.begin(), hash_vector.end());

    return end();
  }

  void remove(const key_type& key)
  {
    list found_list = hash_vector[hashfun(key)];
    for(auto i = found_list.begin(); i != found_list.end(); i++)
    {
      if((*i).first == key)
      {
        found_list.erase(i);
        --size;
        return;
      }
    }

  throw std::out_of_range("invalid key");
  }

  void remove(const const_iterator& it)
  {
    if(it == end())
      throw std::out_of_range("invalid iterator");
    remove(it->first);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    if(size != other.size)
      return false;

    auto i1 = cbegin();
    auto i2 = other.cbegin();
    while(true)
    {
      if(i1 != i2) return false;
      if((i1 == cend()) && (i2 == other.cend())) return true;
      if((i1 == cend()) || (i2 == other.cend())) return false;
      i1++;
      i2++;
    }
    return false;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    if(isEmpty())
      return end();

    iterator beggining = iterator(hash_vector.begin(), hash_vector[0].begin(), hash_vector.begin(), hash_vector.end());
    if(hash_vector[0].size() != 0)
      return beggining;
    return ++beggining;
  }

  iterator end()
  {
    return iterator(hash_vector.end(), hash_vector[max_hash-1].end(), hash_vector.begin(), hash_vector.end());
  }

  const_iterator cbegin() const
  {
    if(isEmpty())
      return end();

    iterator beggining = const_iterator(hash_vector.begin(), hash_vector[0].begin(), hash_vector.begin(), hash_vector.end());
    if(hash_vector[0].size() != 0)
      return beggining;
    return ++beggining;
  }

  const_iterator cend() const
  {
    return ConstIterator(hash_vector.end(), hash_vector[max_hash-1].end(), hash_vector.begin(), hash_vector.end());
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
  using list_i = typename HashMap::list::const_iterator;
  using vector_i = typename HashMap::vector_i;

private:
  vector_i current_list;
  list_i current;
  //potrzeba jakos okreslic poczatek i koniec dostepnego bufora, stad:
  vector_i v_begin;
  vector_i v_end;

  friend class HashMap<KeyType, ValueType>;


public:
  explicit ConstIterator(vector_i vector_it, list_i list_it, vector_i vb, vector_i ve)
  : current_list(vector_it), current(list_it), v_begin(vb), v_end(ve)
  {}

  ConstIterator(const ConstIterator& other)
  {
    current_list = other.current_list;
    current = other.current;
    v_begin = other.v_begin;
    v_end = other.v_end;
  }

  ConstIterator& operator++()
  {
    if(current_list == v_end)
      throw std::out_of_range("cant increment end iterator");

    current++;
    if((*current_list).size() == 0 || current == (*current_list).end())
    {
      current_list++;
      for(; current_list != v_end; current_list++)
      {
        if((*current_list).size() != 0)
        {
          current = (*current_list).begin();
          return *this;
        }
      }

      current_list = v_end;
      current = (*(current_list-1)).end();
    }
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto old = ConstIterator(current_list, current, v_begin, v_end);
    operator++();
    return old;
  }

  ConstIterator& operator--()
  {
    if(current_list == v_end)
      current_list--;

    if(current == (*current_list).begin())
    {
      if(current_list == v_begin)
        throw std::out_of_range("cant increment begin iterator");

      current_list--;
      for(;current_list != v_begin; current_list--)
      {
        if((*current_list).size() != 0)
        {
          current = --(*current_list).end();
          return *this;
        }
      }

      throw std::out_of_range("cant decrement begin");
    }
    else
      current--;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto old = ConstIterator(current_list, current, v_begin, v_end);
    operator--();
    return old;
  }

  reference operator*() const
  {
    if(current_list == v_end)
      throw std::out_of_range("cant acces end element");

    return *current;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(v_end == current_list || other.v_end == other.current_list)
        return (v_end == current_list && other.v_end == other.current_list);
    return ( *current == *(other.current) );
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
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = typename HashMap::value_type*;
  using list_i = typename HashMap::list::iterator;
  using vector_i = typename HashMap::vector_i;


  explicit Iterator(vector_i vector_it, list_i list_it, vector_i vb, vector_i ve):
  ConstIterator(vector_it, list_it, vb, ve)
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
