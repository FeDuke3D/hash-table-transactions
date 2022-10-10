#include <algorithm>
#include <iostream>

template <class Key, class Val>
HashTable<Key, Val>::HashTable()
    : limit_(kStartLimit),
      size_(GetClosePrime(limit_)),
      table_(new std::vector<std::vector<Entry>>) {
  table_->resize(size_);
}

template <class Key, class Val>
HashTable<Key, Val>::HashTable(const HashTable<Key, Val> &other)
    : limit_(), size_(), table_(new std::vector<std::vector<Entry>>) {
  *this = other;
}

template <class Key, class Val>
HashTable<Key, Val>::HashTable(HashTable<Key, Val> &&other)
    : limit_(), size_(), table_(new std::vector<std::vector<Entry>>) {
  *this = std::move(*other);
}

template <class Key, class Val>
HashTable<Key, Val>::~HashTable() {
  delete table_;
}

template <class Key, class Val>
void HashTable<Key, Val>::operator=(const HashTable<Key, Val> &other) {
  limit_ = other.limit_;
  size_ = other.size_;
  count_ = other.count_;
  resize_lock_ = other.resize_lock_;
  *table_ = *other.table_;
}

template <class Key, class Val>
void HashTable<Key, Val>::operator=(HashTable<Key, Val> &&other) {
  std::swap(limit_, other.limit_);
  std::swap(size_, other.size_);
  std::swap(count_, other.count_);
  std::swap(resize_lock_, other.resize_lock_);
  std::swap(table_, other.table_);
}

template <class Key, class Val>
bool HashTable<Key, Val>::SET(const Key &key, const Val &value,
                              const size_t &life_time) {
  bool res(false);

  if (!EXIST(key)) {
    auto moment(time(0));
    int index(CalcIndex(key));
    Entry to_add{};
    to_add.key_ = key;
    to_add.val_ = value;
    to_add.exp_ = (life_time == 0) ? 0 : moment + life_time;
    table_->at(index).push_back(to_add);
    count_++;
    res = true;
  }

  CheckResize();

  return res;
}

template <class Key, class Val>
Val HashTable<Key, Val>::GET(const Key &key) {
  Val res{};

  auto ptr = GetEntry(key);
  if (ptr != nullptr)
    res = ptr->val_;
  else
    throw std::out_of_range("(null)");

  return res;
}

template <class Key, class Val>
bool HashTable<Key, Val>::EXIST(const Key &key) {
  return (GetEntry(key) != nullptr);
}

template <class Key, class Val>
bool HashTable<Key, Val>::DEL(const Key &key) {
  bool res(false);
  auto moment(time(0));
  int index(CalcIndex(key));

  for (auto iter(table_->at(index).begin()); iter != table_->at(index).end();
       iter++) {
    if (iter->key_ == key) {
      if (iter->exp_ > moment || iter->exp_ == 0) {
        res = true;
      }
      table_->at(index).erase(iter);
      count_--;
      break;
    }
  }

  CheckResize();

  return res;
}

template <class Key, class Val>
bool HashTable<Key, Val>::UPDATE(const Key &key, const Val &value) {
  bool res(false);

  auto ptr = GetEntry(key);
  if (ptr != nullptr) {
    ptr->val_ = value;
    res = true;
  }

  return res;
}

template <class Key, class Val>
std::vector<Key> HashTable<Key, Val>::KEYS() {
  std::vector<Key> res{}, to_delete{};
  auto moment(time(0));

  for (int i{}; i < size_; i++) {
    if (table_->at(i).size() > 0) {
      for (auto &one : table_->at(i)) {
        if (one.exp_ > moment || one.exp_ == 0) {
          res.push_back(one.key_);
        } else {
          to_delete.push_back(one.key_);
        }
      }
    }
  }

  resize_lock_ = true;
  for (auto &each : to_delete) DEL(each);
  resize_lock_ = false;
  CheckResize();

  return res;
}

template <class Key, class Val>
bool HashTable<Key, Val>::RENAME(const Key &last_name_key,
                                 const Key &cur_name_key) {
  bool res(false);

  Entry *old_entry = GetEntry(last_name_key);
  if (old_entry != nullptr && !EXIST(cur_name_key)) {
    auto future_time = old_entry->exp_;
    if (future_time != 0) future_time -= time(0);
    SET(cur_name_key, old_entry->val_, future_time);
    DEL(last_name_key);
    res = true;
  }

  return res;
}

template <class Key, class Val>
int HashTable<Key, Val>::TTL(const Key &key) {
  int res{};

  auto ptr = GetEntry(key);
  if (ptr != nullptr) {
    if (ptr->exp_ != 0)
      res = ptr->exp_ - time(0);
    else
      throw std::out_of_range("(inf)");
  } else {
    throw std::out_of_range("(null)");
  }

  return res;
}

template <class Key, class Val>
std::vector<Key> HashTable<Key, Val>::FIND(const Val &search_value) {
  std::vector<Key> res{}, to_delete{};
  auto moment(time(0));

  for (int i{}; i < size_; i++) {
    if (table_->at(i).size() > 0) {
      for (auto &one : table_->at(i)) {
        if (one.exp_ > moment || one.exp_ == 0) {
          if (one.val_ == search_value) res.push_back(one.key_);
        } else {
          to_delete.push_back(one.key_);
        }
      }
    }
  }

  resize_lock_ = true;
  for (auto &each : to_delete) DEL(each);
  resize_lock_ = false;
  CheckResize();

  return res;
}

template <class Key, class Val>
std::vector<Val> HashTable<Key, Val>::SHOWALL() {
  std::vector<Val> res{};
  std::vector<Key> to_delete{};
  auto moment(time(0));

  for (int i{}; i < size_; i++) {
    if (table_->at(i).size() > 0) {
      for (auto &one : table_->at(i)) {
        if (one.exp_ > moment || one.exp_ == 0) {
          res.push_back(one.val_);
        } else {
          to_delete.push_back(one.key_);
        }
      }
    }
  }

  resize_lock_ = true;
  for (auto &each : to_delete) DEL(each);
  resize_lock_ = false;
  CheckResize();

  return res;
}

template <class Key, class Val>
int HashTable<Key, Val>::UPLOAD(const std::string &file_name) {
  delete table_;
  table_ = new std::vector<std::vector<Entry>>;
  limit_ = kStartLimit;
  size_ = GetClosePrime(limit_);
  table_->resize(size_);

  int res{};
  std::ifstream stream(file_name);

  if (stream.is_open()) {
    Key key{};
    Val record{};
    setlocale(LC_ALL, "en_US.UTF-8");
    while (!stream.eof()) {
      stream >> key;
      if (!stream.eof()) {
        stream >> record;
        SET(key, record);
        res++;
      }
    }
  } else {
    throw std::invalid_argument("No such file.");
  }

  return res;
}

template <class Key, class Val>
int HashTable<Key, Val>::EXPORT(const std::string &file_name) {
  int res{};
  std::ofstream stream(file_name);

  if (stream.is_open()) {
    setlocale(LC_ALL, "en_US.UTF-8");
    for (int i{}; i < size_; i++) {
      for (auto &each : table_->at(i)) {
        stream << each.key_ << " " << each.val_ << "\n";
        res++;
      }
    }
    stream.close();
  } else {
    throw std::invalid_argument("Can't open the file.");
  }
  return res;
}

template <class Key, class Val>
int HashTable<Key, Val>::GetClosePrime(int limit) {
  std::set<int> numbers{};
  int prime{1};
  for (int i{2}; i < limit; i++) numbers.insert(i);

  while (numbers.size() > 0) {
    auto iter = numbers.begin();
    if (iter != numbers.end()) prime = *iter;
    while (iter != numbers.end()) {
      auto next = iter;
      next++;
      if (*iter % prime == 0) numbers.erase(*iter);
      iter = next;
    }
  }
  return prime;
}

template <class Key, class Val>
int HashTable<Key, Val>::CalcIndex(const Key &key) {
  return key_hash_(key) % size_;
}

template <class Key, class Val>
typename HashTable<Key, Val>::Entry *HashTable<Key, Val>::GetEntry(
    const Key &key) {
  int index(CalcIndex(key));
  auto moment(time(0));
  Entry *res{};

  auto condition = [key](const Entry &one) { return one.key_ == key; };
  auto iter = std::find_if(table_->at(index).begin(), table_->at(index).end(),
                           condition);
  if (iter != table_->at(index).end()) {
    if (iter->exp_ > moment || iter->exp_ == 0) {
      res = &(*iter);
    } else {
      DEL(key);
    }
  }
  return res;
}

template <class Key, class Val>
void HashTable<Key, Val>::CheckResize() {
  if (!resize_lock_) {
    double ratio = (double)count_ / (double)limit_;
    if (ratio > kUpscaleBorder && limit_ < kUpperLimit) {
      Resize(limit_ * 10);
    } else if (ratio < kDownscaleBorder && limit_ > kLowerLimit) {
      Resize(limit_ / 10);
    }
  }
}

template <class Key, class Val>
void HashTable<Key, Val>::Resize(int new_limit) {
  resize_lock_ = true;
  auto moment(time(0));
  HashTable<Key, Val> res{};
  res.limit_ = new_limit;
  res.size_ = GetClosePrime(new_limit);
  res.table_->resize(res.size_);
  res.resize_lock_ = true;

  for (int i{}; i < size_; i++) {
    if (table_->at(i).size() > 0) {
      for (auto &each : table_->at(i)) {
        if (each.exp_ > moment || each.exp_ == 0) {
          res.SET(each.key_, each.val_,
                  (each.exp_ == 0 ? 0 : each.exp_ - moment));
        }
      }
    }
  }

  *this = std::move(res);
  resize_lock_ = false;
}
