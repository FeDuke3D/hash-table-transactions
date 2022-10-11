#include "interface.hpp"

Interface::Interface() : m_table(new table_type) {}

Interface::Interface(const Interface& other) : m_table(new table_type) {
  *this = other;
}

Interface::Interface(Interface&& other) : m_table(new table_type) {
  *this = std::move(other);
}

Interface::~Interface() { delete m_table; }

Interface& Interface::operator=(const Interface& other) {
  if (this != &other) {
    m_run = other.m_run;
    *m_table = *other.m_table;
  }
  return *this;
}

Interface& Interface::operator=(Interface&& other) {
  if (this != &other) {
    std::swap(m_run, other.m_run);
    std::swap(m_table, other.m_table);
  }
  return *this;
}

void Interface::Show() {
  ClearScreen();
  while (m_run) {
    try {
      ScanInput();
    } catch (const std::exception& e) {
      std::cout << "> " << e.what() << std::endl;
      std::cin.clear();
    }
  }
  std::cout << "bye!" << std::endl;
}

void Interface::ScanInput() {
  std::string word{};
  std::cin >> word;
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  if (word == "set") {
    SlotSet();
  } else if (word == "get") {
    SlotGet();
  } else if (word == "exist") {
    SlotExist();
  } else if (word == "del") {
    SlotDel();
  } else if (word == "update") {
    SlotUpdate();
  } else if (word == "keys") {
    SlotKeys();
  } else if (word == "rename") {
    SlotRename();
  } else if (word == "ttl") {
    SlotTtl();
  } else if (word == "find") {
    SlotFind();
  } else if (word == "showall") {
    SlotShowAll();
  } else if (word == "upload") {
    SlotUpload();
  } else if (word == "export") {
    SlotExport();
  } else if (word == "help") {
    PrintHelp();
  } else if (word == "clear") {
    ClearScreen();
  } else if (word == "q") {
    m_run = false;
  } else {
    Unrecognized();
  }
}

void Interface::PrintHeader() {
  std::cout << " --- Hash Table Data Base ---" << std::endl;
}

void Interface::Unrecognized() {
  std::cout.flush();
  std::cin.clear();
  std::cout << "Input unrecognized, type 'help' for help" << std::endl;
}

void Interface::PrintHelp() {
  std::cout
      << "Available commands and syntax:\n"
         "SET\t: SET key last_name first_name year city coins\n"
         "\tor SET key last_name first_name year city coins EX seconds\t- add "
         "new entry\n"
         "GET\t: GET key\t- get asociated record\n"
         "EXIST\t: EXIST key\t- chek if entry exists\n"
         "DEL\t: DEL key\t- delete entry\n"
         "UPDATE\t: UPDATE key last_name first_name year city coins\n"
         "\t'-' to skip column\t- change record\n"
         "KEYS\t: KEYS\t- print all keys in the base\n"
         "RENAME\t: RENAME old_key new_key\t- change a key of an entry\n"
         "TTL\t: TTL key\t- show remaining time to live for a temporal entry\n"
         "FIND\t: FIND last_name first_name year city coins\t- find all "
         "records according to the pattern\n"
         "SHOWALL\t: SHOWALL\t- print all records\n"
         "UPLOAD\t: UPLOAD file_name\t- load a database from the file\n"
         "EXPORT\t: EXPORT file_name\t- export the database to the file\n"
         "help\t- to see this help\n"
         "clear\t- to clear the screen and see the header\n"
         "q\t- to quit"
      << std::endl;
}

void Interface::ClearScreen() {
  std::cout.flush();
  std::cin.clear();
  system("clear");
  PrintHeader();
}

void Interface::SlotSet() {
  std::string key{}, word{};
  StudentRecord record{};
  std::cin >> key >> record;
  int seconds{};
  if (std::cin.peek() != '\n') {
    std::cin >> word;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (word == "ex") {
      std::cin >> word;
      seconds = std::stoi(word);
    } else {
      throw std::invalid_argument("only EX keyword can be here");
    }
  }
  if (m_table->SET(key, record, seconds)) {
    std::cout << "> OK" << std::endl;
  } else {
    std::cout << "> FAIL" << std::endl;
  }
}

void Interface::SlotGet() {
  std::string word{};
  std::cin >> word;
  StudentRecord answer{m_table->GET(word)};
  if (answer == StudentRecord()) {
    std::cout << "> FAIL" << std::endl;
  } else {
    std::cout << "> " << answer << std::endl;
  }
}

void Interface::SlotExist() {
  std::string word{};
  std::cin >> word;
  if (m_table->EXIST(word)) {
    std::cout << "> TRUE" << std::endl;
  } else {
    std::cout << "> FALSE" << std::endl;
  }
}

void Interface::SlotDel() {
  std::string word{};
  std::cin >> word;
  if (m_table->DEL(word)) {
    std::cout << "> OK" << std::endl;
  } else {
    std::cout << "> FAIL" << std::endl;
  }
}

void Interface::SlotUpdate() {
  std::string word{};
  StudentRecord record{};
  std::cin >> word >> record;
  if (m_table->UPDATE(word, record)) {
    std::cout << "> OK" << std::endl;
  } else {
    std::cout << "> FAIL" << std::endl;
  }
}

void Interface::SlotKeys() {
  auto answer{m_table->KEYS()};
  std::cout << "> " << answer.size() << std::endl;
  unsigned i{1};
  for (auto& each : answer) {
    std::cout << i << ") " << each << std::endl;
    i++;
  }
}

void Interface::SlotRename() {
  std::string old_key{}, new_key{};
  std::cin >> old_key >> new_key;
  if (m_table->RENAME(old_key, new_key)) {
    std::cout << "> OK" << std::endl;
  } else {
    std::cout << "> FAIL" << std::endl;
  }
}

void Interface::SlotTtl() {
  std::string word{};
  std::cin >> word;
  int answer{m_table->TTL(word)};
  std::cout << "> " << answer << std::endl;
}

void Interface::SlotFind() {
  StudentRecord record{};
  std::cin >> record;
  auto answer{m_table->FIND(record)};
  std::cout << "> " << answer.size() << std::endl;
  int i{1};
  for (auto& each : answer) {
    std::cout << i << ") " << each << std::endl;
    i++;
  }
}

void Interface::SlotShowAll() {
  auto all_records = m_table->SHOWALL();
  int i{1};
  std::cout << "№    |Last name     |First name    |Year |City          |Coins\n"
               "-----+--------------+--------------+-----+--------------+------\n";
  for (auto& each : all_records) {
    std::string line{};
    line = std::to_string(i);
    CompleteColumn(&line, 5);
    line.append(each.last_name_);
    CompleteColumn(&line, 20);
    line.append(each.first_name_);
    CompleteColumn(&line, 35);
    line.append(std::to_string(each.year_of_birth_));
    CompleteColumn(&line, 41);
    line.append(each.city_);
    CompleteColumn(&line, 56);
    line.append(std::to_string(each.coins_));
    std::cout << line << std::endl;
    i++;
  }
}

void Interface::SlotUpload() {
  std::string word{};
  std::cin >> word;
  if (word.size() < 4 || word.substr(word.size() - 4) != ".dat")
    word.append(".dat");
  int answer{m_table->UPLOAD(word)};
  std::cout << "> " << answer << std::endl;
}

void Interface::SlotExport() {
  std::string word{};
  std::cin >> word;
  if (word.size() < 4 || word.substr(word.size() - 4) != ".dat")
    word.append(".dat");
  int answer{m_table->EXPORT(word)};
  std::cout << "> " << answer << std::endl;
}

void Interface::CompleteColumn(std::string* line, unsigned size) {
  for (size_t i{line->size()}; i < size; i++)
    line->append(" ");
  line->resize(size);
  line->append("|");
}
