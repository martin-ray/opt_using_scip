#ifndef WSCP_solver
#define WSCP_solver

#include <vector>
#include <map>
#include <set>

int setcoveringproblem(std::vector<std::set<int>>& st_v,std::map<std::set<int>,int>& set2weight,int num_of_elem,std::vector<int>& optimal_set);

#endif