#ifndef COMMANDS_H
#define COMMANDS_H
#include <iostream>
#include <unordered_map>
#include <forward_list>
#include <functional>
namespace fesenko
{
  class Commands
  {
   public:
    using list_t = std::forward_list< size_t >;
    using hash_t = std::unordered_map< std::string, list_t >;
    using data_t = std::unordered_map< std::string, hash_t >;
    Commands();
    void make(const std::string &command, data_t &data, std::istream &in, std::ostream &out);
   private:
    using commands1 = std::function< void(data_t &, std::istream &) >;
    using commands2 = std::function< std::ostream &(const data_t &, std::istream &, std::ostream &) >;
    std::unordered_map< std::string, commands1 > type1;
    std::unordered_map< std::string, commands2 > type2;
  };
}
#endif
