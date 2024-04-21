#include "illini_book.hpp"
#include "utilities.hpp"
using namespace utilities;
// Your code here!
IlliniBook::IlliniBook(const std::string &people_fpath, const std::string &relations_fpath)
{
    std::ifstream  ifs;
    std::string    str1;
    int index=0,uin=0;
    ifs.open(people_fpath);
    if(!ifs.is_open()) throw invalid_argument("File is not exist");
    while(ifs >> str1 )
    {
        uin = stoi(str1);
        uintos_.insert(pair<int,int>(uin,index));
        stouin_.insert(pair<int,int>(index,uin));
        index++; 
    }
    vertexcnt_ = index;
    ifs.close();
    ifs.open(relations_fpath);
    std::pair<std::map<string,int>::iterator,bool> ret;
    if(!ifs.is_open()) throw invalid_argument("File is not exist");
    while(ifs >> str1)
    {
        std::vector<std::string> strs = utilities::Split(str1, ',');
        vector<int> uins={stoi(strs[0]),stoi(strs[1])};
        edges_.push_back(pair<vector<int>,std::string>(uins,strs[2]));
        ret = relation_.insert(pair<string,int>(strs[2],relationcnt_));
        if(ret.second) relationcnt_++;
    }
    ifs.close(); 
    CreateGraphs();     
}
IlliniBook::~IlliniBook()
{

}
bool IlliniBook::AreRelated(int uin_1, int uin_2) const
{
   vector<vector<int>> graph = graphs_[relationcnt_];
   if( GetSeq(uin_1) == kError || GetSeq(uin_2) == kError) return false;
   vector<int> dist = GetShortestDistance(graph,GetSeq(uin_1));
   if(dist[GetSeq(uin_2)]!=-1)
       return true;
   else 
      return false;
}
bool IlliniBook::AreRelated(int uin_1, int uin_2, const std::string &relationship) const
{
    if( GetSeq(uin_1) == kError || GetSeq(uin_2) == kError) return false;
    if(GetRelationIndex(relationship) == kError) return false;
    vector<vector<int>> graph = graphs_[GetRelationIndex(relationship)];
    vector<int> dist = GetShortestDistance(graph,GetSeq(uin_1));
    if(dist[GetSeq(uin_2)]!=-1)
       return true;
    else 
      return false;
}
int IlliniBook::GetRelated(int uin_1, int uin_2) const
{
   vector<vector<int>> graph = graphs_[relationcnt_];
   if( GetSeq(uin_1) == kError || GetSeq(uin_2) == kError) return kError;
   vector<int> dist = GetShortestDistance(graph,GetSeq(uin_1));
   if(dist[GetSeq(uin_2)]!=-1)
       return dist[GetSeq(uin_2)];
   else 
      return kError;
}
int IlliniBook::GetRelated(int uin_1, int uin_2, const std::string &relationship) const
{
    if( GetSeq(uin_1) == kError || GetSeq(uin_2) == kError) return kError;
    if(GetRelationIndex(relationship) == kError) return kError;
    vector<vector<int>> graph = graphs_[GetRelationIndex(relationship)];
    vector<int> dist = GetShortestDistance(graph,GetSeq(uin_1));
    if(dist[GetSeq(uin_2)]!=-1)
       return dist[GetSeq(uin_2)];
    else 
      return kError;
}
std::vector<int> IlliniBook::GetSteps(int uin, int n) const
{
   vector<int>  steps; 
   vector<vector<int>> graph = graphs_[relationcnt_];
   if( GetSeq(uin) == kError) return steps; //empty
   vector<int> dist = GetShortestDistance(graph,GetSeq(uin));
   int index=0;
   for(auto it:dist)
   {
      if(it == n)
        steps.push_back(GetUin(index));
      index++;  
   }
   return steps;
}
bool compare_int (const int & first, const int& second)
{
  return ( first < second);
}
size_t IlliniBook::CountGroups() const
{
    vector<vector<int>> graph = graphs_[relationcnt_];
    queue<int> q;
    set<int> visited;
    set<int> sub1;
    set<int>::iterator it;
    pair<std::set<int>::iterator,bool> ret;
    vector<set<int>> groups;
    int V=graph.size();
    // Push the source node to the queue
    for(int S=0; S < V ; S++)
    {
    sub1.clear();    
    ret = visited.insert(S);
    if(ret.second) {q.push(S);sub1.insert(S);}
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbour : graph[node]) {
            ret = sub1.insert(neighbour);
            if (ret.second) {
                q.push(neighbour);
                visited.insert(neighbour);
            }
        }
      }
      if(sub1.size()>0) groups.push_back(sub1);
    }
    return groups.size();
}
size_t IlliniBook::CountGroups(const std::string &relationship) const
{
   if(GetRelationIndex(relationship) == kError) return vertexcnt_;//all are single.
   vector<vector<int>> graph = graphs_[GetRelationIndex(relationship)];
   int index=0;
   set<list<int>> groups;
   for (auto adj : graph) {
        list<int> grpm;
        grpm.push_back(index);
        for(auto node:adj)
        {   
            list<int> grp2;//two persons group
            if(node > index) {grp2.push_back(node);grp2.push_back(index);}
            else {grp2.push_back(index);grp2.push_back(node);}
            groups.insert(grp2); // if exist, will not insert again.
            grpm.push_back(node);
        }
        grpm.sort(compare_int);//multiple persons
        if(grpm.size()>2) groups.insert(grpm);
        index++;
    }
    return(groups.size());
}
size_t IlliniBook::CountGroups(const std::vector<std::string> &relationships) const
{  int index =0 ;
   vector<vector<int>> graph(vertexcnt_);
   for(auto relationship:relationships)
   {  
      if(GetRelationIndex(relationship) == kError) continue;//all are single.
      vector<vector<int>> graphnew = graphs_[GetRelationIndex(relationship)];
      index = 0;
      for(auto &grh:graph)
      {    
            vector<int> grhn = graphnew[index];
            for(auto node:grhn)
            {//check if node is inside grp;
               bool inside = false;
               for(auto nd:grh)
                   if(node==nd)
                   { inside = true;
                     break;
                   }
                if(!inside) grh.push_back(node);
            }
            index++;
        }
    }
    return(CountGroups(graph));
}
size_t IlliniBook::CountGroups(vector<vector<int>> graph) const
{
    queue<int> q;
    set<int> visited;
    set<int> sub1;
    set<int>::iterator it;
    pair<std::set<int>::iterator,bool> ret;
    vector<set<int>> groups;
    int V=graph.size();
    // Push the source node to the queue
    for(int S=0; S < V ; S++)
    {
    sub1.clear();    
    ret = visited.insert(S);
    if(ret.second) {q.push(S);sub1.insert(S);}
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbour : graph[node]) {
            ret = sub1.insert(neighbour);
            if (ret.second) {
                q.push(neighbour);
                visited.insert(neighbour);
            }
        }
      }
      if(sub1.size()>0) groups.push_back(sub1);
    }
    return groups.size();
}
// Function to print the shortest distance between source
// vertex and destination vertex
vector<int> IlliniBook::GetShortestDistance(vector<vector<int>>& graph,int S ) const
{
    // par[] array stores the parent of nodes
    int vcnt = graph.size();
    queue<int> q;
    vector<int> par(vcnt, -1);
    vector<int> dist(vcnt, -1);
    dist[S] = 0;
    q.push(S);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbour : graph[node]) {
            if (dist[neighbour] == -1) {
                par[neighbour] = node;
                dist[neighbour] = dist[node] + 1;
                q.push(neighbour);
            }
        }
    }
    return dist;
}
//Create graphs vector
void IlliniBook::CreateGraphs()
{//
  graphs_.resize(relationcnt_ + 1);
  for(auto it:relation_ )
  { std::string relstr = it.first;
    vector<vector<int>>& graph = graphs_[it.second];
    graph.resize(vertexcnt_);
    for (auto edge : edges_) {
        if(relstr == edge.second)
        {
         graph[GetSeq(edge.first[0])].push_back(GetSeq(edge.first[1]));
         graph[GetSeq(edge.first[1])].push_back(GetSeq(edge.first[0]));
         }
    }
  }
  vector<vector<int>>& graph = graphs_[relationcnt_];
  graph.resize(vertexcnt_);
  for (auto edge : edges_) {
         graph[GetSeq(edge.first[0])].push_back(GetSeq(edge.first[1]));
         graph[GetSeq(edge.first[1])].push_back(GetSeq(edge.first[0]));
      }
}
