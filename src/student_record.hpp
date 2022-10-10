#ifndef SRC_STUDENT_RECORD_H_
#define SRC_STUDENT_RECORD_H_

#include <iostream>
#include <string>
#include <utility>

class StudentRecord {
 public:
  StudentRecord() {}
  StudentRecord(const std::string &last, const std::string &first, int year,
                const std::string &city, int coins)
      : last_name_(last),
        first_name_(first),
        year_of_birth_(year),
        city_(city),
        coins_(coins) {}
  StudentRecord(const char *last, const char *first, int year, const char *city,
                int coins)
      : last_name_(last),
        first_name_(first),
        year_of_birth_(year),
        city_(city),
        coins_(coins) {}
  StudentRecord(const StudentRecord &other) { *this = other; }
  StudentRecord(StudentRecord &&other) { *this = std::move(other); }
  ~StudentRecord() {}

  void operator=(const StudentRecord &other) {
    if (other.last_name_ != "-") last_name_ = other.last_name_;
    if (other.first_name_ != "-") first_name_ = other.first_name_;
    if (other.year_of_birth_ != -1) year_of_birth_ = other.year_of_birth_;
    if (other.city_ != "-") city_ = other.city_;
    if (other.coins_ != -1) coins_ = other.coins_;
  }

  void operator=(StudentRecord &&other) {
    if (other.last_name_ != "-") last_name_ = other.last_name_;
    if (other.first_name_ != "-") first_name_ = other.first_name_;
    if (other.year_of_birth_ != -1) year_of_birth_ = other.year_of_birth_;
    if (other.city_ != "-") city_ = other.city_;
    if (other.coins_ != -1) coins_ = other.coins_;
  }

  bool operator==(const StudentRecord &other) {
    bool res(true);
    if (last_name_ != "-" && other.last_name_ != "-" &&
        last_name_ != other.last_name_)
      res = false;
    else if (first_name_ != "-" && other.first_name_ != "-" &&
             first_name_ != other.first_name_)
      res = false;
    else if (year_of_birth_ != -1 && other.year_of_birth_ != -1 &&
             year_of_birth_ != other.year_of_birth_)
      res = false;
    else if (city_ != "-" && other.city_ != "-" && city_ != other.city_)
      res = false;
    else if (coins_ != -1 && other.coins_ != -1 && coins_ != other.coins_)
      res = false;
    return res;
  }

  friend std::ostream &operator<<(std::ostream &os, const StudentRecord &data) {
    os << "\"" << data.last_name_ << "\" \"" << data.first_name_ << "\" "
       << data.year_of_birth_ << " \"" << data.city_ << "\" " << data.coins_;
    return os;
  }

  friend std::istream &operator>>(std::istream &is, StudentRecord &data) {
    std::string year{}, coin{};
    is >> data.last_name_ >> data.first_name_ >> year >> data.city_ >> coin;

    if (year == "-") {
      data.year_of_birth_ = -1;
    } else {
      data.year_of_birth_ = std::stoi(year);
    }

    if (coin == "-") {
      data.coins_ = -1;
    } else {
      data.coins_ = std::stoi(coin);
    }

    for (auto each : {&data.last_name_, &data.first_name_, &data.city_}) {
      if (each->at(0) == '\"' && each->at(each->size() - 1) == '\"')
        *each = each->substr(1, each->size() - 2);
    }

    return is;
  }

  std::string ToString() {
    std::string result = last_name_ + " " + first_name_ + " " +
                         std::to_string(year_of_birth_) + " " + city_ + " " +
                         std::to_string(coins_);
    return result;
  }

  std::string last_name_{};
  std::string first_name_{};
  int year_of_birth_{};
  std::string city_{};
  int coins_{};
};

#endif  // SRC_STUDENT_RECORD_HPP_
