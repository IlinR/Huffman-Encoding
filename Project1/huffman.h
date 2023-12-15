#pragma once
#ifndef _HUFFMAN_
#define _HUFFMAN_

#include <queue>
#include <unordered_map>
#include <iostream>
using namespace std;
struct Node
{
	char ch;
	int freq;
	Node* left, * right;
};

void TreeToMap(Node* root, string codeStr, unordered_map<char, string>  &huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = codeStr;
	}

	TreeToMap(root->left, codeStr + "0", huffmanCode);
	TreeToMap(root->right, codeStr + "1", huffmanCode);
}



Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
};

struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};
Node* makeHuffmanTree(int* header) {
	priority_queue<Node*, vector<Node*>, comp> pq;
	for (int i = 0; i < 256;i++) {
		if (header[i] != 0)
			pq.push(getNode((char)(i-128), header[i], nullptr, nullptr));
	}

	while (pq.size() != 1) {
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top();	pq.pop();
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}
	Node* root = pq.top();	pq.pop();

	return root;
}
#endif // !_HUFFMAN_