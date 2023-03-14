
#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

string months[] = {"January",   "February", "March",    "April",
                   "May",       "June",     "July",     "August",
                   "September", "October",  "November", "December"};

class Date {

private:
  int year;
  int date;
  string month;

public:
  Date() { // system date
    const int century = 1900;
    time_t timer;
    tm *time;
    std::time(&timer);
    time = localtime(&timer);
    date = time->tm_mday;
    month = months[time->tm_mon];
    year = time->tm_year + century;
  }
  string newDate;
  string getSystemDate() {
    string d = to_string(date);
    string y = to_string(year);

    newDate = "Date Added: " + month + " " + d + ", " + y + "\n";
    return newDate;
  }
};

class ToDo {
private:
  string item;
  Date date;

public:
  int d = 1; // 1 if txtfile 2 if created in main
  string txtDate;
  ToDo(string s) {
    item = s;
    d = 2;
  }

  ToDo() { item = "dummy"; }

  ToDo(string s, string d) {
    item = s;
    date.newDate = d;
    d = 1;
  }

  ToDo(const ToDo &o) { item = o.item; }

  string getDate() {
    if (d == 2) {
      return date.getSystemDate();
    } else {
      return date.newDate;
    }
  }
  string getItem() { return item; }

  void setItem(string s) { item = s; }

  void operator=(const ToDo &x) { item = x.item; }
};


class NotArrayList {

private:
  int size;
  int count;

public:
  int empty = 1;
  friend ostream &operator<<(ostream &, const NotArrayList &);
  friend istream &operator>>(istream &in, NotArrayList &list);
  ToDo **array = nullptr;
  NotArrayList(int s) {
    size = s;
    array = new ToDo *[size];
    count = 0;
  }

  NotArrayList() {
    size = 0;
    array = new ToDo *[size];
    count = 0;
  }

  ~NotArrayList() {
    cout << "\n\nSetting array to null\n";
    cout << "Freeing " << size << " element(s)...\n";
    for (int i = 0; i < size; i++) {
      cout << "\nElement at " << &array[i] << " "
           << "set to null";
      delete array[i];
      array[i] = nullptr;
    }
    cout << "\n\nArray at " << &array << " set to null\n";
    array = nullptr;
  }

  void createInv(string s) {
    if (count < size) {
      array[count] = new ToDo(s);
      count++;
    } else if (count >= size) {
      ToDo **temp = nullptr;
      size += 1;
      count++;
      temp = new ToDo *[size];
      for (int i = 0; i < size; i++) {
        temp[i] = array[i];
      }
      temp[size - 1] = new ToDo(s);
      delete[] array;
      array = temp;
      temp = nullptr;
    } else {
      cout << "Error";
    }
  }

  void addInv(string s, string d) {
    if (count < size) {
      array[count] = new ToDo(s, d);
      count++;
    } else if (count >= size) {
      ToDo **temp = nullptr;
      size += 1;
      count++;
      temp = new ToDo *[size];
      for (int i = 0; i < size; i++) {
        temp[i] = array[i];
      }
      temp[size - 1] = new ToDo(s, d);
      delete[] array;
      array = temp;
      temp = nullptr;
    } else
      cout << "Error";
  }

  void addToDo() {
    if (count < size) {
      array[count] = new ToDo;
      count++;
    } else if (count >= size) {
      ToDo **temp = nullptr;
      size += 1;
      count++;
      temp = new ToDo *[size];
      for (int i = 0; i < size; i++) {
        temp[i] = array[i];
      }
      temp[size - 1] = new ToDo;
      delete[] array;
      array = temp;
      temp = nullptr;
    } else
      cout << "Error";
  }

  void deleteInv(int id) {

    ToDo **tmpArr = new ToDo *[size];

    for (int i = 0; i < id; i++) {
      tmpArr[i] = array[i];
    }
    size--;
    count--;
    for (int i = (id + 1); i < size + 1; i++) {
      tmpArr[i - 1] = array[i];
    }
    delete[] array;
    array = tmpArr;
    tmpArr = nullptr;
  }


  void testArray() {
    NotArrayList testList;
    testList.createInv("Study");
    testList.createInv("Study2");
    testList.createInv("Study3");
    testList.createInv("Study4");
    testList.createInv("Study5");

    if (testList.array[0]->getItem() == "Study" &&
        testList.array[1]->getItem() == "Study2" &&
        testList.array[2]->getItem() == "Study3" &&
        testList.array[3]->getItem() == "Study4" &&
        testList.array[4]->getItem() == "Study5") {
      cout << "Testing create to do items...\t\tPassed\n";
    } else
      cout << "Testing create to do items...\t\tPassed\n";

    for (int i = 0; i < 5; i++) {
      testList.deleteInv(i);
    }
  } // end test

  int getSize() { return size; }
};

int *running = new int; // use running variable in overloaded cin and main

string getWord(string s);
// Specification C1 - Overload «
ostream &operator<<(ostream &osObject, const NotArrayList &list) {
  for (int i = 0; i < list.size; i++) {
    osObject << "Item " << i + 1 << ": " << list.array[i]->getItem() << endl;
    cout << list.array[i]->getDate() << endl;
    cout << endl;
  }
  return osObject;
}

istream &operator>>(istream &in, NotArrayList &list) {
  cin.clear();
  string input;
  getline(cin, input);

 
  if (input[0] == '+' && input[1] == ' ') {
    string newWord = getWord(input);
    if (list.empty == 2) {
      for (int i = 0; i < list.size; i++) {
        if (list.array[i]->getItem() == "dummy") {
          // Specification A4 - Overload Constructor
          list.array[i] = new ToDo(newWord);
          list.deleteInv(list.size - 1);
        }
      }
      list.empty = 1;
    }
    if (newWord.empty()) {
      list.addToDo();
      list.empty = 2;
    } else {
      list.createInv(newWord);
    }
  }

  else if (input[0] == '?') {
    cout << list;
    cout << endl;
  }
 
  else if (input[0] == '-' && input[1] == ' ') {
    int id = int(input[2] - '0');
    if (list.size == 0 || id > list.size || id < 0) {
      cout << "\nOut of bounds\n";
    } else {
      cout << "\nRemoved Id: " << id << endl;
      list.deleteInv(id - 1);
    }

  } else if (input[0] == '/') {
    *running = 0;
  } else
    cout << "\nInvalid input\n";

  return in;
}

void UnitTest();
void programGreeting();

int main() {
  *running = 1;
  string line;
  NotArrayList list;
  string date;
  string todo;

  ifstream myFile("todo.txt");
  if (myFile.is_open()) {
    while (getline(myFile, line)) {
      for (int i = 0; i < (int)line.length(); i++) {

        if (line[i] == '+') {
          todo = line.substr(0, i);
        }
        if (line[i] == '+') {
          date = line.substr(i + 1, line.length());
          list.addInv(todo, date);
        }
      }
    }
  }

  UnitTest();
  programGreeting();

  while (*running == 1) {
    cin.clear();
    cout << "\nEnter a command\n";
    cin >> list;
  }

  
  ofstream outputFile("todo.txt");
  if (outputFile.is_open()) {
    for (int i = 0; i < list.getSize(); i++) {
      outputFile << list.array[i]->getItem() << "+" << list.array[i]->getDate()
                 << "\n";
    }
    outputFile.close();
  } else
    cout << "\nError";

  return 0;
}

string getWord(string str) {
  string newWord = str.substr(2, str.length());
  return newWord;
}

void UnitTest() {
  NotArrayList testList;
  testList.testArray();
}

void programGreeting() {
  cout << "\nProgram for users to enter items into a to do list\nCody "
          "Hopkins\n11-6-2022\n\nEnter '+' followed by a space to enter "
          "todos\nEnter '-' followed by a space and the id number to remove an "
          "item\nEnter '?' to show all items\nEnter '/' to quit.\n";
}
