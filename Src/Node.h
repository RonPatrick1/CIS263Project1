#include <string>
using namespace std;

class Node {
public:
	Node(double pFreq, string pWord) {
		word = pWord;
		freq = pFreq;
		lchild = nullptr;
		rchild = nullptr;
		parent = nullptr;
		myIndex = 0;
		ZeroOne=false;
	}
	Node() {
		word = "";
		freq = 0.0;
		lchild = nullptr;
		rchild = nullptr;
		parent = nullptr;
		myIndex = 0;
		ZeroOne=false;
	}
	/*Node(const Node & rhs) {
		freq = ((Node)rhs).freq;
		word = ((Node)rhs).word;
		lchild = nullptr;
		rchild = nullptr;
		parent = nullptr;
	}*/
	bool operator<(Node other) const {
		return freq > other.freq;
	}

	string word;
	double freq;
	Node *parent;
	Node *lchild;
	Node *rchild;
	bool ZeroOne;
	int myIndex;
};


