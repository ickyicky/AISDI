#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{
void perfomTest()
{
  //ugly, yet practical test
  using namespace std::chrono;

  aisdi::TreeMap<int, std::string> tree;
  aisdi::HashMap<int, std::string> hash;

  std::string el = "asd";
  auto time = 0;
  int maxtree,maxhash;
  //inserting
  size_t amount = 4096;
  while(time < 10000)
  {
    tree = aisdi::TreeMap<int, std::string>{};
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
      tree[count] = el;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    time = duration_cast<milliseconds>( t2 - t1 ).count();
    std::cout<<amount<<"operations took:"<<time<<" ";
    maxtree = amount;
    amount*=2;
  }
  std::cout <<std::endl;

  time = 0;
  amount = 4096;
  while(time < 10000)
  {
    hash = aisdi::HashMap<int, std::string>{};
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
      hash[count] = el;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    time = duration_cast<milliseconds>( t2 - t1 ).count();
    std::cout<<amount<<"operations took:"<<time<<" ";
    maxhash = amount;
    amount*=2;
  }
  std::cout <<std::endl;

  //accesing
  time = 0;
  amount = 4096;

  while(time < 1000)
  {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
      tree.valueOf(count%maxtree);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    time = duration_cast<milliseconds>( t2 - t1 ).count();
    std::cout<<amount<<"operations took:"<<time<<" ";
    amount*=2;
  }
  std::cout <<std::endl;

  time = 0;
  amount = 4096;
  while(time < 1000)
  {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(size_t count = 0; count <= amount; count++)
    {
      hash.valueOf(count%maxhash);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    time = duration_cast<milliseconds>( t2 - t1 ).count();
    std::cout<<amount<<"operations took:"<<time<<" ";
    amount*=2;
  }
  std::cout <<std::endl;
}

} // namespace

int main()
{
  perfomTest();
  return 0;
}
