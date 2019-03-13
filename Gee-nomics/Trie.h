#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <iostream>
// TODO: remove iostream and print
using namespace std;


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
	void cleanUp(Node* p);
	void insertHelper(const std::string& key, const ValueType& value, Node* currentNode);
	std::vector<ValueType> findHelper(const std::string& key, bool exactMatchOnly, Node* currentNode) const;
};

// TODO:DOUBT: Should I pass Node pointer by constant reference?
// TODO:DOUBT: When to use typename? Why is it needed in the loop below?

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
		cout << "val" << " "; // TODO: Print properly // cout << (*p) << " ";
	cout << endl << "Children: ";
	for (typename vector<ChildPtr>::iterator p = currentNode->m_children.begin();
		p != currentNode->m_children.end(); p++)
	{
		cout << (*p).m_label << endl;
		print((*p).m_child);
	}
}

template<typename ValueType>
inline void Trie<ValueType>::cleanUp(Node* p)
{
	if (p != nullptr)
	{
		// Dont need to delete all the values
		// They are integers and are deleted automatically

		// Clean up all the Node's children
		// then delete the node
		for (typename vector<ChildPtr>::iterator currentChildPtr = p->m_children.begin();
			currentChildPtr != p->m_children.end(); currentChildPtr++)
		{
			cleanUp((*currentChildPtr).m_child);
		}
		delete p;
	}
}

template<typename ValueType>
inline Trie<ValueType>::Trie()
{
	createEmpty();
}

template<typename ValueType>
inline Trie<ValueType>::~Trie()
{
	// TODO: REMOVE PRINT STEP
	//print(m_root);
	cleanUp(m_root);
}

template<typename ValueType>
inline void Trie<ValueType>::reset()
{
	cleanUp(m_root);
	createEmpty();
}

template<typename ValueType>
inline void Trie<ValueType>::insert(const std::string & key, const ValueType & value)
{
	insertHelper(key, value, m_root);
	// TODO: insertHelper probably has a bug
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
	// Base Case: Empty Key
	if (key.length() == 0)
		return std::vector<ValueType>();
	
	vector<ValueType> returnVector;
	for (typename vector<ChildPtr>::iterator p = m_root->m_children.begin();
		p != m_root->m_children.end(); p++)
	{
		if (p->m_label == key[0])
		{
			returnVector = findHelper(key.substr(1), exactMatchOnly, p->m_child);
			break;
		}
	}
	return returnVector;
}

template<typename ValueType>
inline std::vector<ValueType> Trie<ValueType>::findHelper(const std::string & key, bool exactMatchOnly, Node * currentNode) const
{
	// Base Case: Empty Key
	if (key.length() == 0)
		return currentNode->m_values;

	char currentChar = key[0];
	vector<ValueType> returnVector;
	vector<ValueType> tempResult;
	// Loop through the current node's children to find a matching label
	for (typename vector<ChildPtr>::iterator p = currentNode->m_children.begin();
		p != currentNode->m_children.end(); p++)
	{
		if (p->m_label == currentChar)
		{
			tempResult = findHelper(key.substr(1), exactMatchOnly, p->m_child);
			returnVector.insert(returnVector.end(), tempResult.begin(), tempResult.end());
		}
		else if (p->m_label != currentChar && !exactMatchOnly)
		{
			// if there was not a match, set exactMatchOnly to true and keep looking
			tempResult = findHelper(key.substr(1), true, p->m_child);
			returnVector.insert(returnVector.end(), tempResult.begin(), tempResult.end());
		}
	}
	return returnVector;
}

#endif // TRIE_INCLUDED