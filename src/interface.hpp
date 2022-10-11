#ifndef SRC_INTERFACE_HPP_
#define SRC_INTERFACE_HPP_

#include "hash_table.hpp"
#include "student_record.hpp"

class Interface {
 public:
  Interface();
  Interface(const Interface& other);
  Interface(Interface&& other);
  ~Interface();

  Interface& operator=(const Interface& other);
  Interface& operator=(Interface&& other);

  void Show();

 private:
  typedef HashTable<std::string, StudentRecord> table_type;

  void ScanInput();

  void PrintHeader();
  void Unrecognized();
  void PrintHelp();
  void ClearScreen();

  void SlotSet();
  void SlotGet();
  void SlotExist();
  void SlotDel();
  void SlotUpdate();
  void SlotKeys();
  void SlotRename();
  void SlotTtl();
  void SlotFind();
  void SlotShowAll();
  void SlotUpload();
  void SlotExport();

  void CompleteColumn(std::string* line, unsigned size);

  bool m_run{true};
  table_type* m_table{};
};

#endif  // SRC_INTERFACE_HPP_
