#ifndef SRC_ABSTRACT_STRUCT_DATA_BASE_HPP_
#define SRC_ABSTRACT_STRUCT_DATA_BASE_HPP_

#include <string>
#include <vector>

template <class Key, class Value>
class AbstractStructDatabase {
 public:
  virtual ~AbstractStructDatabase() = default;

  virtual bool SET(const Key &key, const Value &value,
                   const size_t &life_time = 0) = 0;
  virtual Value GET(const Key &key) = 0;
  virtual bool EXIST(const Key &key) = 0;
  virtual bool DEL(const Key &key) = 0;
  virtual bool UPDATE(const Key &key, const Value &value) = 0;
  virtual std::vector<Key> KEYS() = 0;
  virtual bool RENAME(const Key &last_name_key, const Key &cur_name_key) = 0;
  virtual int TTL(const Key &key) = 0;
  virtual std::vector<Key> FIND(const Value &search_value) = 0;
  virtual std::vector<Value> SHOWALL() = 0;
  virtual int UPLOAD(const std::string &file_name) = 0;
  virtual int EXPORT(const std::string &file_name) = 0;
};

#endif  // SRC_ABSTRACT_STRUCT_DATA_BASE_HPP_
