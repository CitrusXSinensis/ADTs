// Example program
#include <iostream>
#include <string>
#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>
#include <utility>
using namespace std;

int main()
{
  int n, m, s, u, v, w, tmpmin;
  string args;
  getline(cin, args);
  vector<string> tmpS;
  boost::split(tmpS, args, boost::is_any_of(" "));
  n = stoi(tmpS[0]);
  m = stoi(tmpS[1]);
  s = stoi(tmpS[2]);
  vector<vector<pair<int,int>>> G(n);
  vector<int> DP(n, INT_MAX);
  vector<int> parent(n, -1);
  DP[s] = 0;
  vector<int> prevDP(n);
  while (getline(cin, args)) {
    boost::split(tmpS, args, boost::is_any_of(" "));
    u = stoi(tmpS[0]);
    v = stoi(tmpS[1]);
    w = stoi(tmpS[2]);
    G[v].push_back(make_pair(u, w));
  }

  for (int i = 1; i < n; ++i) {
    prevDP = DP;
    for (int j = 0; j < n; ++j) {
      tmpmin = prevDP[j];
      for (int k= 0; k < G[j].size(); ++k) {
        if (prevDP[G[j][k].first] != INT_MAX) {
          if (tmpmin > prevDP[G[j][k].first] + G[j][k].second) {
            tmpmin = prevDP[G[j][k].first] + G[j][k].second;
            parent[j] = G[j][k].first;
          }
        }
      }
      DP[j] = tmpmin;
    }
    if (DP == prevDP) break;
  }

  prevDP = DP;
  for (int j = 0; j < n; ++j) {
    tmpmin = prevDP[j];
    for (int k= 0; k < G[j].size(); ++k) {
      if (prevDP[G[j][k].first] == INT_MAX) tmpmin = tmpmin;
      else if (tmpmin > prevDP[G[j][k].first] + G[j][k].second) tmpmin = prevDP[G[j][k].first] + G[j][k].second;
    }
    DP[j] = tmpmin;
  }

  if (DP != prevDP) {
    cout << "ERROR: NEGATIVE WEIGHT CYCLE\n";
    int start, tmp;
    vector<int> negCyc;
    for (int i = 0; i < n; ++i) {
      if (DP[i] != prevDP[i]) start = i;
    }
    negCyc.push_back(start);
    tmp = parent[start];
    while (find(negCyc.begin(), negCyc.end(), tmp) == negCyc.end()) {
      negCyc.push_back(tmp);
      tmp = parent[tmp];
    }
    cout << tmp << ' ';
    for (int i = negCyc.size() - 1; i > 0; --i) {
      if (tmp == negCyc[i]) break;
      cout << negCyc[i] << ' ';
    }
    cout << tmp; 
    return 0;
  }

  if (DP[0] == INT_MAX) cout << "inf";
  else cout << DP[0];
  for (int i = 1; i < DP.size(); ++i) {
    if (DP[i] == INT_MAX) {
      cout << " inf";
    }
    else cout << ' ' << DP[i];
  }
}
