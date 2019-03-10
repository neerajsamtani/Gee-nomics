#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <iostream>
// TODO: remove iostream
using namespace std;

//TODO: REMOVE
template<typename T> struct TD;

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
	struct Node;
	struct ChildPtr
	{
		char m_label;
		Node* m_child;
	};
	struct Node
	{
		std::vector<ValueType> m_values;
		std::vector<ChildPtr> m_children;
	};
	Node* m_root;

	void createEmpty();
	void print(Node* currentNode);
	void cleanUp();
	void insertHelper(const std::string& key, const ValueType& value, Node* currentNode);
	// DOUBT: Should I pass Node pointer by constant reference?
};

// DOUBT: When to use typename? Why is it needed in the loop below?

template<typename ValueType>
inline void Trie<ValueType>::createEmpty()
{
	m_root = new Node;
}

template<typename ValueType>
inline void Trie<ValueType>::print(Node* currentNode)
{
	cout << "Values :";
	for (typename vector<ValueType>::iterator p = currentNode->m_values.begin();
		p != currentNode->m_values.end(); p++)
		cout << *p;
	cout << endl << "Children: ";
	for (typename vector<ChildPtr>::iterator p = currentNode->m_children.begin();
		p != currentNode->m_children.end(); p++)
	{
		cout << (*p).m_label << endl;
		print((*p).m_child);
	}
}

template<typename ValueType>
inline void Trie<ValueType>::cleanUp()
{
	// delete all nodes
}

template<typename ValueType>
inline Trie<ValueType>::Trie()
{
	createEmpty();
}

template<typename ValueType>
inline Trie<ValueType>::~Trie()
{
	print(m_root);
	cleanUp();
}

template<typename ValueType>
inline void Trie<ValueType>::reset()
{
	cleanUp();
	createEmpty();
}

template<typename ValueType>
inline void Trie<ValueType>::insert(const std::string & key, const ValueType & value)
{
	insertHelper(key, value, m_root);
}


template<typename ValueType>
inline void Trie<ValueType>::insertHelper(const std::string & key, const ValueType & value, Node* currentNode)
{
	// Base Case: string key is empty
	if (key.length() == 0)
		return;
	
	char currentChar = key[0];
	bool foundCurrentChar = false;

	// Loop through the current node's children to find a matching label
	// vector<ChildPtr>::iterator
	for (typename vector<ChildPtr>::iterator p = currentNode->m_children.begin();
		p != currentNode->m_children.end(); p++)
	{
		// TODO: Remove the line below
		//TD<decltype(p)> td;
		if (p->m_label == currentChar)
		{
			foundCurrentChar = true;
			// Special Case : string key has only one character
			if (key.length() == 1)
				p->m_child->m_values.push_back(value);
			// Otherwise if key has more than one value
			else
				insertHelper(key.substr(1), value, p->m_child);
			break;
		}
	}
	// If there is no child with a matching label, create one
	if (!foundCurrentChar)
	{
		ChildPtr newChildPtr;
		newChildPtr.m_label = currentChar;
		newChildPtr.m_child = new Node;
		
		// Special Case : string key has only one character
		if (key.length() == 1)
		{
			newChildPtr.m_child->m_values.push_back(value);
			currentNode->m_children.push_back(newChildPtr);
		}
		// Otherwise if key has more than one value
		else
		{
			currentNode->m_children.push_back(newChildPtr);
			insertHelper(key.substr(1), value, newChildPtr.m_child);
		}
	}
}

template<typename ValueType>
inline std::vector<ValueType> Trie<ValueType>::find(const std::string & key, bool exactMatchOnly) const
{
	return std::vector<ValueType>();
}

#endif // TRIE_INCLUDED