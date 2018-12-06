#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{
void perfomTest(size_t amount)
{
  //ugly, yet practical test
  using namespace std::chrono;
  if(amount > 100000000) amount = 100000000;
  else if(amount < 10) amount = 10;

  aisdi::TreeMap<int, std::string> tree;
  aisdi::HashMap<int, std::string> hash;

  std::string el = "asd";

  //inserting
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  for(size_t count = 0; count <= amount; count++)
  {
    tree[count] = el;
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  auto tree_time = duration_cast<milliseconds>( t2 - t1 ).count();

  high_resolution_clock::time_point t3 = high_resolution_clock::now();
  for(size_t count = 0; count <= amount; count++)
  {
    hash[count] = el;
  }
  high_resolution_clock::time_point t4 = high_resolution_clock::now();

  auto hash_time = duration_cast<milliseconds>( t4 - t3 ).count();

  std::cout<<"Inserting "<<amount<<" elements took"<<std::endl<<
              tree_time<<" in treemap"<<std::endl<<
              hash_time<<" in hashmap"<<std::endl;

  //accesing
  t1 = high_resolution_clock::now();
  for(size_t count = 0; count <= amount; count++)
  {
    //tree.valueOf(count);
  }
  t2 = high_resolution_clock::now();

  tree_time = duration_cast<milliseconds>( t2 - t1 ).count();

  t3 = high_resolution_clock::now();
  for(size_t count = 0; count <= amount; count++)
  {
    hash.valueOf(count);
  }
  t4 = high_resolution_clock::now();

  hash_time = duration_cast<milliseconds>( t4 - t3 ).count();

  std::cout<<"Accesing "<<amount<<" elements took"<<std::endl<<
              tree_time<<" in treemap"<<std::endl<<
              hash_time<<" in hashmap"<<std::endl;

  //iterating
  t1 = high_resolution_clock::now();
  for(auto i = tree.begin(); i != tree.end() ; i++)
  {}
  t2 = high_resolution_clock::now();

  tree_time = duration_cast<milliseconds>( t2 - t1 ).count();

  t3 = high_resolution_clock::now();
  for(auto i = hash.begin(); i != hash.end() ; i++)
  {}
  t4 = high_resolution_clock::now();

  hash_time = duration_cast<milliseconds>( t4 - t3 ).count();

  std::cout<<"Iterating through "<<amount<<" elements took"<<std::endl<<
              tree_time<<" in treemap"<<std::endl<<
              hash_time<<" in hashmap"<<std::endl<<std::endl<<std::endl<<std::endl;
}

} // namespace

int main()
{
  for(size_t amount=10000; amount<=100000 ; amount*=10)
    perfomTest(amount);
  return 0;
}
