#include <iostream>
#include <unordered_map>
#include <forward_list>
#include "commands.h"

int main()
{
  using list_t = std::forward_list< size_t >;
  using hash_t = std::unordered_map< std::string, list_t >;
  using data_t = std::unordered_map< std::string, hash_t >;
  data_t data;
  fesenko::Commands commands;
  std::string cmd = "";
  while (std::cin >> cmd) {
    try {
      commands.make(cmd, data, std::cin, std::cout);
    } catch (const std::exception &e) {
      std::cout << e.what() << "\n";
      return 2;
    }
  }
}
