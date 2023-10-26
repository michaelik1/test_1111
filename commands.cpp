#include "commands.h"
#include <messages.h>
#include <stdexcept>
#include <string>
#include "subCommands.h"

fesenko::Commands::Commands():
  type1(),
  type2()
{
  type1.insert(std::make_pair("read_file", read_file_cmd));
  type1.insert(std::make_pair("delete_dict", delete_dict_cmd));
  type1.insert(std::make_pair("delete_word", delete_word_cmd));
  type1.insert(std::make_pair("complement", complement_cmd));
  type1.insert(std::make_pair("intersect", intersect_cmd));
  type1.insert(std::make_pair("union", union_cmd));
  type1.insert(std::make_pair("rename", rename_cmd));
  type1.insert(std::make_pair("insert", insert_cmd));

  type2.insert(std::make_pair("find", &find_cmd));
  type2.insert(std::make_pair("print_dict", &print_dict_cmd));
  type2.insert(std::make_pair("print_word", &print_word_cmd));
}

void fesenko::Commands::make(const std::string &command, data_t &data, std::istream &in, std::ostream &out)
{
  try {
    if (type1.find(command) != type1.cend()) {
      type1.at(command)(data, in);
    } else if (type2.find(command) != type2.cend()) {
      type2.at(command)(data, in, out);
    } else {
      throw std::logic_error("No command");
    }
  } catch (...) {
    outInvalidCommandMessage(out);
    out << "\n";
  }
}
