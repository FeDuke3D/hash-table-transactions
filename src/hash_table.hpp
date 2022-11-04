#ifndef SRC_HASH_TABLE_HPP_
#define SRC_HASH_TABLE_HPP_

#include <fstream>
#include <set>

#include "abstract_struct_data_base.hpp"

template <class Key, class Val>
class HashTable : public AbstractStructDatabase<Key, Val> {
 public:
  HashTable();
  HashTable(const HashTable &other);
  HashTable(HashTable &&other);
  ~HashTable();

  HashTable &operator=(const HashTable &other);
  HashTable &operator=(HashTable &&other);

  bool SET(const Key &key, const Val &value,
           const size_t &life_time = 0) override;
  Val GET(const Key &key) override;
  bool EXIST(const Key &key) override;
  bool DEL(const Key &key) override;
  bool UPDATE(const Key &key, const Val &value) override;
  std::vector<Key> KEYS() override;
  bool RENAME(const Key &last_name_key, const Key &cur_name_key) override;
  int TTL(const Key &key) override;
  std::vector<Key> FIND(const Val &search_value) override;
  std::vector<Val> SHOWALL() override;
  int UPLOAD(const std::string &file_name) override;
  int EXPORT(const std::string &file_name) override;

 private:
  const int kStartLimit = 10;
  const double kUpscaleBorder = 0.5;
  const double kDownscaleBorder = 0.03;
  const int kUpperLimit = 100000;
  const int kLowerLimit = 10;
  struct Entry {
    Key key_;
    Val val_;
    int exp_;
  };

 private:
  int GetClosePrime(int limit);
  int CalcIndex(const Key &key);
  Entry *GetEntry(const Key &key);
  void CheckResize();
  void Resize(int new_limit);

  int limit_;
  int size_;
  int count_{};
  bool resize_lock_{};
  std::vector<std::vector<Entry>> *table_;
  std::hash<Key> key_hash_{};
};

#include "hash_table.inl"

#endif  // SRC_HASH_TABLE_HPP_
