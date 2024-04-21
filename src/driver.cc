#include "illini_book.hpp"
#include <iostream>
int main() {
  IlliniBook books("/home/dennis/Desktop/sample9/example/persons.csv","/home/dennis/Desktop/sample9/example/relations.csv");
  std::cout << "books.AreRelated(1, 2) " ;
  std::cout << books.AreRelated(1, 2) << std::endl;
  std::cout << "books.GetRelated(1, 6, 128)";
  std::cout <<books.GetRelated(1, 6, "128") << std::endl;
  std::cout << "books.GetSteps(1, 1)"; 
  vector<int> steps = books.GetSteps(1, 1);
  for(auto it:steps)
    std::cout << it << " ";
  std::cout << std::endl;
  std::cout << books.CountGroups() << std::endl;
  std::cout << books.CountGroups("173") << std::endl;
  std::cout << books.CountGroups("128") << std::endl;
  std::cout << books.CountGroups("124") << std::endl; 
  std::cout << books.CountGroups(std::vector<std::string>{"128","124"}) << std::endl; 

  return 0;
}
