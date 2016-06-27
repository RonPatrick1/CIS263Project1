/*Huffman Code Try*/

#include <queue>
using namespace std;

template <typename Comparable>
class RonsHuffmanCode {

public:
	RonsHuffmanCode() : root{ nullptr }	{
		ArrayCoded = false;
	}
	
	RonsHuffmanCode(const RonsHuffmanCode & rhs) : root{ nullptr } {
		root = clone(rhs.root);
	}

	RonsHuffmanCode(RonsHuffmanCode && rhs) : root{ rhs.root } {
		rhs.root = nullptr;
	}

	RonsHuffmanCode & operator=(const RonsHuffmanCode & rhs) {
		RonsHuffmanCode copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	RonsHuffmanCode & operator=(RonsHuffmanCode && rhs) {
		std::swap(root, rhs.root);
		return *this;
	}

	~RonsHuffmanCode() {
		makeEmpty();
	}

	void insert(const Comparable & x, string whichFile) {
		insert(x, root, whichFile);
	}

	void insert(Comparable && x, string whichFile) {
		insert(std::move(x), root, whichFile);
	}

	void remove(const Comparable & x) {
		remove(x, root);
	}

	bool contains(const Comparable & x) const {
		return contains(x, root);
	}

	bool isEmpty() const {
		return root == nullptr;
	}

	void makeEmpty() {
		makeEmpty(root);
	}

	void makeCode() {
		if (ArrayCoded) {
			return;
		}
	}

private:

	bool ArrayCoded;

	struct BinaryNode {
		Comparable element;
		BinaryNode *left;
		BinaryNode *right;
		int Freq;
		queue<string> whichFilesHaveIt;

		BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt) {
			element = theElement;
			left = lt;
			right = rt;
		}

		BinaryNode(Comparable && theElement, BinaryNode *lt, BinaryNode *rt) {
			element = move(theElement);
			left = lt;
			right = rt;
		}
	};

	BinaryNode *root;

	void makeEmpty(BinaryNode * & t) { 
		if (t != nullptr) {
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
			ArrayCoded = false;
		}
		t = nullptr;
	}

	void insert(const Comparable & x, BinaryNode * & t) {
		ArrayCoded = false;
		if (t == nullptr)
			t = new BinaryNode{ x, nullptr, nullptr };
		else if (x < t->element)
			insert(x, t->left);
		else if (t->element < x)
			insert(x, t->right);
		else
			/*duplicate*/
			while (!t->whichFilesHaveIt.empty()) {
				x->whichFilesHaveIt.push(t->whichFilesHaveIt.front());
				t->whichFilesHaveIt.pop();
				x->Freq++;
			}
	}

	void insert(Comparable && x, BinaryNode * & t) {
		ArrayCoded = false;
		if (t == nullptr)
			t = new BinaryNode{ std::move(x), nullptr, nullptr };
		else if (x < t->element)
			insert(std::move(x), t->left);
		else if (t->element < x)
			insert(std::move(x), t->right);
		else
			/*duplicate*/
			while (!t->whichFilesHaveIt.empty()) {
				x->whichFilesHaveIt.push(t->whichFilesHaveIt.front());
				t->whichFilesHaveIt.pop();
				x->Freq++;
			}
	}

	BinaryNode * clone(BinaryNode *t) const	{
		if (t == nullptr)
			return nullptr;
		else
			return new BinaryNode{ t->element, clone(t->left), clone(t->right) };
	}
};