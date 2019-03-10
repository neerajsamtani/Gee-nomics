#include "provided.h"
#include "Trie.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
using namespace std;

void testGenome()
{
	// Constructor
	Genome GenomeOne("Genome 1", "ACTG");
	Genome GenomeTwo("Genome 2", "TCGACT");
	Genome GenomeThree("Genome 3", "TCTCG");
	Genome GenomeEmpty("", "");

	// name() and length()
	assert(GenomeOne.name() == "Genome 1" && GenomeOne.length() == 4);
	assert(GenomeTwo.name() == "Genome 2" && GenomeTwo.length() == 6);
	assert(GenomeThree.name() == "Genome 3" && GenomeThree.length() == 5);
	assert(GenomeEmpty.name() == "" && GenomeEmpty.length() == 0);

	// extract()
	// Provided Tests
	Genome g("oryx",
		"GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
	string	f1, f2, f3;
	assert(g.extract(0, 5, f1) && f1 == "GCTCG");	 //	result1	=	true,	f1	=	“GCTCG”;
	assert(g.extract(74, 6, f2) && f2 == "CTGGGA"); //	result2 =	true,	f2 =	“CTGGGA”;
	assert(!g.extract(74, 7, f3)); //	result3 =	false,	f3 is	unchanged
	// Written unit tests
	string fragment;
	// extract from start
	assert(GenomeOne.extract(0, 0, fragment) && fragment.length() == 0);
	assert(GenomeOne.extract(0, 1, fragment) && fragment.length() == 1);
	assert(GenomeOne.extract(0, 2, fragment) && fragment.length() == 2);
	assert(GenomeOne.extract(0, 3, fragment) && fragment.length() == 3);
	assert(GenomeOne.extract(0, 4, fragment) && fragment.length() == 4);
	assert(!GenomeOne.extract(0, 5, fragment) && fragment.length() == 4);
	// extract from middle
	assert(GenomeOne.extract(1, 0, fragment) && fragment.length() == 0);
	assert(GenomeOne.extract(1, 1, fragment) && fragment.length() == 1);
	assert(GenomeOne.extract(1, 2, fragment) && fragment.length() == 2);
	assert(GenomeOne.extract(1, 3, fragment) && fragment.length() == 3);
	assert(!GenomeOne.extract(1, 4, fragment) && fragment.length() == 3);
	// extract from middle
	assert(GenomeOne.extract(2, 0, fragment) && fragment.length() == 0);
	assert(GenomeOne.extract(2, 1, fragment) && fragment.length() == 1);
	assert(GenomeOne.extract(2, 2, fragment) && fragment.length() == 2);
	assert(!GenomeOne.extract(2, 3, fragment) && fragment.length() == 2);
	// extract from end
	assert(GenomeOne.extract(3, 0, fragment) && fragment.length() == 0);
	assert(GenomeOne.extract(3, 1, fragment) && fragment.length() == 1);
	assert(!GenomeOne.extract(3, 2, fragment) && fragment.length() == 1);
	// extract after end
	assert(!GenomeOne.extract(4, 1, fragment) && fragment.length() == 1);
	// extract from empty Genome
	assert(!GenomeEmpty.extract(0, 1, fragment) && fragment.length() == 1);

	// load()
	string filename = "c:/genomes/Test_Genomes.txt";
	ifstream strm(filename);
	if (!strm)
	{
		cout << "Cannot	open " << filename << endl;
		return;
	}
	vector<Genome> vg;
	bool success = Genome::load(strm, vg);
	if (success)
	{
		cout << "Loaded	" << vg.size() << " genomes successfully:" << endl;
		//for (int k = 0; k != vg.size(); k++)
		//	cout << vg[k].name() << endl;
	}
	else
		cout << "Error loading genome data" << endl;
}

void testTrie()
{
	Trie<int> trie;
	trie.insert("Hi", 2);
	trie.insert("Hello", 5);
	trie.insert("Hell", 4);
	trie.insert("Hell", 3);
}

int main()
{
	testGenome();
	testTrie();
	cout << "Passed all tests" << endl;
}