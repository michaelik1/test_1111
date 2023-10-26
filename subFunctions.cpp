#include "subFunctions.h"
#include <cctype>
#include <string>

std::string fesenko::get_cmd_word(std::string &line)
{
  if (line.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  std::string word = "";
  while (!line.empty()) {
    char c = line[0];
    line.erase(0, 1);
    if (!std::isspace(c)) {
      word += c;
    } else if (!word.empty()) {
      break;
    }
  }
  if (word.empty()) {
    throw std::invalid_argument("Wrong input");
  }
  while (!line.empty() && isspace(line[0])) {
    line.erase(0, 1);
  }
  return word;
}

std::forward_list< std::string > fesenko::parse_text_line(const std::string line)
{
  std::forward_list< std::string > list;
  if (line.empty()) {
    return list;
  }
  std::string word = "";
  for (size_t i = 0; i < line.size(); i++) {
    char c = line[i];
    if (std::isalnum(c)) {
      word += std::tolower(c);;
    } else if (!word.empty()) {
      list.push_front(word);
      word = "";
    }
  }
  if (!word.empty()) {
    list.push_front(word);
    word = "";
  }
  list.reverse();
  return list;
}

void fesenko::print_word(const hash_t &hash, const std::string word, std::ostream &out)
{
  const list_t &list = hash.at(word);
  out << word;
  for (auto &it: list) {
    out << " " << it;
  }
  out << "\n";
}

void fesenko::insert_in_asc_order(list_t &list, size_t number)
{
  if (list.empty()) {
    list.push_front(number);
    return;
  }
  list_t copy = list;
  auto cur = list.before_begin();
  while (!copy.empty() && copy.front() < number) {
    copy.pop_front();
    cur++;
  }
  if (!copy.empty() && copy.front() == number) {
    return;
  }
  list.insert_after(cur, number);
}

void fesenko::make_complementation(data_t &data, std::string new_dict_name, std::string dict_name1, std::string dict_name2)
{
  hash_t new_dict;
  hash_t &dict1 = data.at(dict_name1);
  hash_t &dict2 = data.at(dict_name2);
  if (dict_name1 == dict_name2) {
    data[new_dict_name] = new_dict;
    return;
  }
  for (auto &it: dict1) {
    if (dict2.find(it.first) != dict2.end()) {
      list_t new_list;
      list_t &list1 = it.second;
      list_t &list2 = dict2.at(it.first);
      make_complementation(new_list, list1, list2);
      if (!new_list.empty()) {
        new_dict.insert(std::make_pair(it.first, new_list));
      }
    } else {
      new_dict.insert(it);
    }
  }
  data[new_dict_name] = new_dict;
}

void fesenko::make_complementation(list_t &new_list, const list_t &list1, const list_t &list2)
{
  list_t list1_cp = list1;
  list_t list2_cp = list2;
  while (!list1_cp.empty() && !list2_cp.empty()) {
    if (list1_cp.front() == list2_cp.front()) {
      list1_cp.pop_front();
      list2_cp.pop_front();
    } else {
      if (list1_cp.front() < list2_cp.front()) {
        insert_in_asc_order(new_list, list1_cp.front());
        list1_cp.pop_front();
      } else {
        list2_cp.pop_front();
      }
    }
  }
  while (!list1_cp.empty()) {
    insert_in_asc_order(new_list, list1_cp.front());
    list1_cp.pop_front();
  }
}

void fesenko::make_intersection(data_t &data, std::string new_dict_name, std::string dict_name1, std::string dict_name2)
{
  hash_t new_dict;
  hash_t &dict1 = data.at(dict_name1);
  hash_t &dict2 = data.at(dict_name2);
  if (dict_name1 == dict_name2) {
    data[new_dict_name] = dict1;
    return;
  }
  for (auto &it: dict1) {
    if (dict2.find(it.first) != dict2.end()) {
      list_t new_list;
      list_t &list1 = it.second;
      list_t &list2 = dict2.at(it.first);
      make_intersection(new_list, list1, list2);
      if (!new_list.empty()) {
        new_dict.insert(std::make_pair(it.first, new_list));
      }
    }
  }
  data[new_dict_name] = new_dict;
}

void fesenko::make_intersection(list_t &new_list, const list_t &list1, const list_t &list2)
{
  list_t list1_cp = list1;
  list_t list2_cp = list2;
  while (!list1_cp.empty() && !list2_cp.empty()) {
    if (list1_cp.front() == list2_cp.front()) {
      insert_in_asc_order(new_list, list1_cp.front());
      list1_cp.pop_front();
      list2_cp.pop_front();
    } else if (list1_cp.front() < list2_cp.front()) {
      list1_cp.pop_front();
    } else {
      list2_cp.pop_front();
    }
  }
}

void fesenko::make_union(data_t &data, std::string new_dict_name, std::string dict_name1, std::string dict_name2)
{
  hash_t new_dict = data.at(dict_name1);
  hash_t &dict2 = data.at(dict_name2);
  if (dict_name1 == dict_name2) {
    return;
  }
  for (auto &it: dict2) {
    if (new_dict.find(it.first) == new_dict.end()) {
      new_dict.insert(it);
    } else {
      list_t &new_list = new_dict.at(it.first);
      list_t &list2 = it.second;
      make_union(new_list, list2);
    }
  }
  data[new_dict_name] = new_dict;
}

void fesenko::make_union(list_t &new_list, const list_t &list2)
{
  list_t list2_cp = list2;
  while (!list2_cp.empty()) {
    insert_in_asc_order(new_list, list2_cp.front());
    list2_cp.pop_front();
  }
}
