#ifndef ILLINI_BOOK_HPP
#define ILLINI_BOOK_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <set>
#include <queue>
#include <list>

using namespace std;
const int kError=-1;
class IlliniBook {
public:
  IlliniBook(const std::string &people_fpath, const std::string &relations_fpath);
  IlliniBook(const IlliniBook &rhs) = delete;
  IlliniBook &operator=(const IlliniBook &rhs) = delete;
  ~IlliniBook();
  bool AreRelated(int uin_1, int uin_2) const;
  bool AreRelated(int uin_1, int uin_2, const std::string &relationship) const;
  int GetRelated(int uin_1, int uin_2) const;
  int GetRelated(int uin_1, int uin_2, const std::string &relationship) const;
  std::vector<int> GetSteps(int uin, int n) const;
  size_t CountGroups() const;
  size_t CountGroups(const std::string &relationship) const;
  size_t CountGroups(const std::vector<std::string> &relationships) const;
  //Help function
  vector<int> GetShortestDistance(vector<vector<int> >& graph,int S) const;
  void CreateGraphs();
  size_t CountGroups(vector<vector<int>> graph) const;
  int GetSeq(int uin) const 
   { if(uintos_.find(uin) != uintos_.end())
          return uintos_.find(uin)->second;
     else 
          return kError;     
    };
  int GetUin(int seq) const
   { if(stouin_.find(seq) != stouin_.end())
          return stouin_.find(seq)->second;
     else 
          return kError;     
    };
  int GetRelationIndex(string relation) const
  { if(relation_.find(relation) != relation_.end())
       return relation_.find(relation)->second;
    else 
       return kError; };
private:
  std::map<int, int>  uintos_; //UIN to SEQ
  std::map<int, int>  stouin_; //SEQ to UIN
  std::vector<std::vector<std::vector<int>>> graphs_;//
  size_t  vertexcnt_=0;
  std::vector<std::pair<std::vector<int>,std::string>> edges_;//{{UIN1,UIN2},"relation"} 
  std::map<std::string, int>  relation_;//{"relation", seq}
  int relationcnt_=0;
};

#endif
