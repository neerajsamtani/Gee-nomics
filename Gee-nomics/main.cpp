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
	assert(g.extract(0, 5, f1) && f1 == "GCTCG");	 //	result1	=	true,	f1	=	�GCTCG�;
	assert(g.extract(74, 6, f2) && f2 == "CTGGGA"); //	result2 =	true,	f2 =	�CTGGGA�;
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

}

void testTrie()
{
	// Constructor
	Trie<int> trie;
	trie.insert("hit", 1);
	trie.insert("hit", 2);
	trie.insert("hi", 9);
	// reset()
	trie.reset();
	// insert()
	trie.insert("hit", 1);
	trie.insert("hit", 2);
	trie.insert("hi", 9);
	trie.insert("hi", 17);
	trie.insert("hip", 10);
	trie.insert("hip", 20);
	trie.insert("hat", 7);
	trie.insert("hat", 8);
	trie.insert("hat", 9);
	trie.insert("a", 14);
	trie.insert("to", 22);
	trie.insert("to", 23);
	trie.insert("tap", 19);
	trie.insert("tap", 6);
	trie.insert("tap", 32);

	// find(exactMatchOnly = true)
	string text;
	text = "hit";
	vector<int> result = trie.find(text, true);
	assert(result[0] == 1 && result[1] == 2 );
	text = "hi";
	result = trie.find(text, true);
	assert(result[0] == 9 && result[1] == 17);
	text = "hip";
	result = trie.find(text, true);
	assert(result[0] == 10 && result[1] == 20);
	text = "hat";
	result = trie.find(text, true);
	assert(result[0] == 7 && result[1] == 8 && result[2] == 9);
	text = "a";
	result = trie.find(text, true);
	assert(result[0] == 14);
	text = "to";
	result = trie.find(text, true);
	assert(result[0] == 22 && result[1] == 23);
	text = "tap";
	result = trie.find(text, true);
	assert(result[0] == 19 && result[1] == 6 && result[2] == 32);

	//find(exactMatchOnly = false)
	text = "hit";
	result = trie.find(text, false);
	assert(result[0] == 1 && 
			result[1] == 2 && 
			result[2] == 10 &&
			result[3] == 20 &&
			result[4] == 7 &&
			result[5] == 8 &&
			result[6] == 9);
	text = "sit";
	result = trie.find(text, false);
	assert(result.size() == 0);
	text = "hft";
	result = trie.find(text, false);
	assert(result[0] == 1 &&
		result[1] == 2 &&
		result[2] == 7 &&
		result[3] == 8 &&
		result[4] == 9);
	text = "a";
	result = trie.find(text, false);
	assert(result[0] == 14 && result.size() == 1);
	text = "h";
	result = trie.find(text, false);
	assert(result.size() == 0);

}

void testGenomeMatcher()
{
	vector<DNAMatch> matches;
	bool result;

	// create Genomes
	Genome GenomeOne("Genome 1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
	Genome GenomeTwo("Genome 2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
	Genome GenomeThree("Genome 3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");

	// constructor
	GenomeMatcher GenomeMatcherOne(4);
	GenomeMatcherOne.addGenome(GenomeOne);
	GenomeMatcherOne.addGenome(GenomeTwo);
	GenomeMatcherOne.addGenome(GenomeThree);
	
	
	// findGenomesWithThisDNA()
	result = GenomeMatcherOne.findGenomesWithThisDNA("GAAG", 4, true, matches);
	assert(result && matches[0].genomeName == "Genome 1" && matches[0].length == 4 && matches[0].position == 60);
	assert(result && matches[1].genomeName == "Genome 2" && matches[1].length == 4 && matches[1].position == 54);
	assert(result && matches[2].genomeName == "Genome 3" && matches[2].length == 4 && matches[2].position == 29);
	
	result	= GenomeMatcherOne.findGenomesWithThisDNA("GAATAC",	4,	true,	matches);
	assert(result && matches[0].genomeName == "Genome 1" && matches[0].length == 5 && matches[0].position == 22);
	assert(result && matches[1].genomeName == "Genome 2" && matches[1].length == 5 && matches[1].position == 48);

	result = GenomeMatcherOne.findGenomesWithThisDNA("GAATAC", 6, true, matches);
	assert(!result && matches.size() == 0);

	result = GenomeMatcherOne.findGenomesWithThisDNA("GAATAC", 6, false, matches);
	assert(result && matches[0].genomeName == "Genome 1" && matches[0].length == 6 && matches[0].position == 22);
	assert(result && matches[1].genomeName == "Genome 2" && matches[1].length == 6 && matches[1].position == 48);
	
	result = GenomeMatcherOne.findGenomesWithThisDNA("GTATAT", 6, false, matches);
	assert(result && matches[0].genomeName == "Genome 1" && matches[0].length == 6 && matches[0].position == 22);
	assert(result && matches[1].genomeName == "Genome 2" && matches[1].length == 6 && matches[1].position == 48);
	
	result = GenomeMatcherOne.findGenomesWithThisDNA("GAATACG", 6, false, matches);
	assert(result && matches[0].genomeName == "Genome 1" && matches[0].length == 6 && matches[0].position == 22);
	assert(result && matches[1].genomeName == "Genome 2" && matches[1].length == 7 && matches[1].position == 48);
	
	result = GenomeMatcherOne.findGenomesWithThisDNA("GAAGGGTT", 5, false, matches);
	assert(result && matches[0].genomeName == "Genome 1" && matches[0].length == 8 && matches[0].position == 60);
	assert(result && matches[1].genomeName == "Genome 2" && matches[1].length == 5 && matches[1].position == 54);
	assert(result && matches[2].genomeName == "Genome 3" && matches[2].length == 7 && matches[2].position == 35);
	
	result = GenomeMatcherOne.findGenomesWithThisDNA("GAAGGGTT", 6, false, matches);
	assert(result && matches[0].genomeName == "Genome 1" && matches[0].length == 8 && matches[0].position == 60);
	assert(result && matches[1].genomeName == "Genome 3" && matches[1].length == 7 && matches[1].position == 35);

		
	result = GenomeMatcherOne.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCC", 12, false, matches);
	assert(result && matches[0].genomeName == "Genome 2" && matches[0].length == 19 && matches[0].position == 28);
		
	result = GenomeMatcherOne.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCG", 12, false, matches);
	assert(result && matches[0].genomeName == "Genome 2" && matches[0].length == 19 && matches[0].position == 28);

	result = GenomeMatcherOne.findGenomesWithThisDNA("GAAG", 3, true, matches);
	assert(!result && matches.size() == 0);
	
	result = GenomeMatcherOne.findGenomesWithThisDNA("GAAG", 5, true, matches);
	assert(!result && matches.size() == 0);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GenomeMatcher GenomeMatcherTwo(5);

	const string PROVIDED_DIR = "c:/genomes";

	const string providedFiles[] = {
		"Ferroplasma_acidarmanus.txt",
		"Halobacterium_jilantaiense.txt",
		"Halorubrum_chaoviator.txt",
		"Halorubrum_californiense.txt",
		"Halorientalis_regularis.txt",
		"Halorientalis_persicus.txt",
		"Ferroglobus_placidus.txt",
		"Desulfurococcus_mucosus.txt"
	};

	// load()
	for (int i = 0; i < 8; i++)
	{
		string filename = PROVIDED_DIR + "/" + providedFiles[i];
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
			//GenomeMatcherTwo.addGenome(vg);
			for (const auto& g : vg)
				GenomeMatcherTwo.addGenome(g);
		}
		else
			cout << "Error loading genome data" << endl;
	}
	
	result = GenomeMatcherTwo.findGenomesWithThisDNA("GTGTGCAAA", 5, true, matches);
	cout << "Matches found: " << matches.size() << endl;
	result = GenomeMatcherTwo.findGenomesWithThisDNA("GTGTGCAAA", 5, false, matches);
	cout << "Matches found: " << matches.size() << endl;
	result = GenomeMatcherTwo.findGenomesWithThisDNA("GTGTGCAAAA", 10, true, matches);
	cout << "Matches found: " << matches.size() << endl;
	result = GenomeMatcherTwo.findGenomesWithThisDNA("GTGTGCAAAA", 10, false, matches);
	cout << "Matches found: " << matches.size() << endl;
	result = GenomeMatcherTwo.findGenomesWithThisDNA("ACGAATCACGTGCGAGA", 11, true, matches);
	cout << "Matches found: " << matches.size() << endl;
	for (auto p = matches.begin(); p != matches.end(); p++) 
	{
		cout << "length " << p->length << " position " << p->position << " in " << p->genomeName << endl;
	}

	// findRelatedGenomes

	Genome testGenome("Test Genome", "ATCCAAGACAGCGAGGACGGCGTCGACGTCGCCACGATTCTGGATCGGGCAATCGCCGACGAAC");
	vector<GenomeMatch> results;
	result = GenomeMatcherTwo.findRelatedGenomes(testGenome, 10, true, 95, results);
}


int main()
{
	testGenome();
	testTrie();
	testGenomeMatcher();
	cout << "Passed all tests" << endl;
}

