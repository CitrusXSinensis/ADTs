#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Tree {
	pair<int, int> node;
	vector<Tree> child;
};

void trav(Tree &T) {
	if (T.child.size() == 0) {
		cout << T.node.first << ' ' << T.node.second << ' ';
	}
	else {
		trav(T.child[0]);
		trav(T.child[1]);
		cout << T.node.first << ' ' << T.node.second << ' ';
	}
}

void buildTree(vector<pair<int, int>> &points, Tree &T, int flag) {
	if (points.size() == 1) {
		T.node = points[0];
	}
	else {
		vector<int> Compare;
		vector<pair<int, int>> points1;
		vector<pair<int, int>> points2;
		if (flag == 0) { // compare x
			for (int i = 0; i < points.size(); i++) {
				Compare.push_back(points[i].first);
			}
			sort(Compare.begin(), Compare.end());
			int pivot = Compare[points.size() / 2];
			pair<int, int> node;
			for (int i = 0; i < points.size(); i++) {
				if (points[i].first == pivot) node = points[i];
				if (points[i].first < pivot) points1.push_back(points[i]);
				else points2.push_back(points[i]);
			}
			Tree t1, t2;
			vector<Tree> V;
			V.push_back(t1);
			V.push_back(t2);
			T = Tree{ node, V };
			int f = 0;;
			if (flag == 0) f = 1;
			buildTree(points1, T.child[0], f);
			buildTree(points2, T.child[1], f);

		}
		else { // compare y
			for (int i = 0; i < points.size(); i++) {
				Compare.push_back(points[i].second);
			}
			sort(Compare.begin(), Compare.end());
			int pivot = Compare[points.size() / 2];
			pair<int, int> node;
			for (int i = 0; i < points.size(); i++) {
				if (points[i].second == pivot) node = points[i];
				if (points[i].second < pivot) points1.push_back(points[i]);
				else points2.push_back(points[i]);
			}
			Tree t1, t2;
			vector<Tree> V;
			V.push_back(t1);
			V.push_back(t2);
			T = Tree{ node, V };
			int f = 0;;
			if (flag == 0) f = 1;
			buildTree(points1, T.child[0], f);
			buildTree(points2, T.child[1], f);
		}
	}
}

int main() {
	int size;
	vector<pair<int, int>> points;
	cin >> size;
	for (int i = 0; i < size; i++) {
		int tmp1;
		int tmp2;
		cin >> tmp1 >> tmp2;
		pair<int, int> tmpP(tmp1, tmp2);
		points.push_back(tmpP);
	}
	Tree T;
	buildTree(points, T, 0);
	trav(T);
}
