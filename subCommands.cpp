#include "subCommands.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include "subFunctions.h"

void fesenko::read_file_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  std::string filename = get_cmd_word(line);
  if (!line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  std::ifstream fin(filename);
  if (!fin.is_open()) {
    throw std::invalid_argument("Can`t open the file");
  }
  data[dict_name].clear();
  hash_t &dict = data.at(dict_name);
  size_t counter = 0;
  std::forward_list< std::string > word_list;
  while (std::getline(fin, line)) {
    counter++;
    word_list = parse_text_line(line);
    std::string word = "";
    while (!word_list.empty()) {
      word = word_list.front();
      word_list.pop_front();
      insert_in_asc_order(dict[word], counter);
    }
  }
}

void fesenko::delete_dict_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  if (!line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  data.erase(dict_name);
}

void fesenko::delete_word_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  std::string word = get_cmd_word(line);
  if (!line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  data.at(dict_name).erase(word);
}

void fesenko::complement_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string new_dict_name = get_cmd_word(line);
  std::string dict_name1 = get_cmd_word(line);
  std::string dict_name2 = get_cmd_word(line);
  make_complementation(data, new_dict_name, dict_name1, dict_name2);
  while (!line.empty()) {
    std::string dict_name = get_cmd_word(line);
    make_complementation(data, new_dict_name, new_dict_name, dict_name);
  }
}

void fesenko::intersect_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string new_dict_name = get_cmd_word(line);
  std::string dict_name1 = get_cmd_word(line);
  std::string dict_name2 = get_cmd_word(line);
  make_intersection(data, new_dict_name, dict_name1, dict_name2);
  while (!line.empty()) {
    std::string dict_name = get_cmd_word(line);
    make_intersection(data, new_dict_name, new_dict_name, dict_name);
  }
}

void fesenko::union_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string new_dict_name = get_cmd_word(line);
  std::string dict_name1 = get_cmd_word(line);
  std::string dict_name2 = get_cmd_word(line);
  make_union(data, new_dict_name, dict_name1, dict_name2);
  while (!line.empty()) {
    std::string dict_name = get_cmd_word(line);
    make_union(data, new_dict_name, new_dict_name, dict_name);
  }
}

void fesenko::rename_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  std::string new_dict_name = get_cmd_word(line);
  if (!line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  if (dict_name.compare(new_dict_name) == 0) {
    return;
  }
  hash_t temp = data.at(dict_name);
  data.erase(dict_name);
  data.insert(std::make_pair(new_dict_name, temp));
}

void fesenko::insert_cmd(data_t &data, std::istream &in)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  std::string word = get_cmd_word(line);
  if (line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  hash_t &dict = data.at(dict_name);
  while (!line.empty()) {
    size_t number = stoull(get_cmd_word(line));
    insert_in_asc_order(dict[word], number);
  }
}

std::ostream &fesenko::print_word_cmd(const data_t &data, std::istream &in, std::ostream &out)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  std::string word = get_cmd_word(line);
  if (!line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  const hash_t &hash = data.at(dict_name);
  print_word(hash, word, out);
  return out;
}

std::ostream &fesenko::print_dict_cmd(const data_t &data, std::istream &in, std::ostream &out)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  if (!line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  const hash_t &hash = data.at(dict_name);
  for (auto &it: hash) {
    print_word(hash, it.first, out);
  }
  return out;
}

std::ostream &fesenko::find_cmd(const data_t &data, std::istream &in, std::ostream &out)
{
  std::string line = "";
  std::getline(in, line);
  std::string dict_name = get_cmd_word(line);
  std::string word = get_cmd_word(line);
  if (!line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  const hash_t &hash = data.at(dict_name);
  if (hash.find(word) == hash.end()) {
    out << "Not found\n";
  } else {
    print_word(hash, word, out);
  }
  return out;
}
