#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

// TODO: Destructor to destroy Trie

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	struct matchPosition
	{
		int m_genomeNumber;
		int m_position;
	};
	int m_minSearchLength;
	vector<Genome> m_genomes;
	Trie<matchPosition> m_genomeTrie;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
	:m_minSearchLength(minSearchLength)
{}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	m_genomes.push_back(genome);
	string fragment;
	for (int i = 0; genome.extract(i, m_minSearchLength, fragment); i++)
	{
		matchPosition matchPos;
		matchPos.m_genomeNumber = m_genomes.size();
		matchPos.m_position = i;
		m_genomeTrie.insert(fragment, matchPos);
		cout << "Genome " << matchPos.m_genomeNumber << " Position " << matchPos.m_position << endl;
		// TODO: REMOVE COUT
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return m_minSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	// Return false if
	// fragment's length is less than minimumLength, or
	// minimumLength is less than the m_minSearchLength
	if (fragment.length() < minimumLength ||
		minimumLength < m_minSearchLength)
		return false;

	// Attempt to find matches of length m_minSearchLength in the GenomeMatcher Trie
	vector<matchPosition> allMatches = m_genomeTrie.find(fragment.substr(0,m_minSearchLength), exactMatchOnly);
	// Look for a match of the rest of the fragement, perhaps recursively calling findGenomesWithThisDNA on the rest of the string

	// Return false if there were no matches found
	if (allMatches.size() == 0)
		return false;

	// Temporarily print out all the matches
	cout << "Matches Found:" << endl;
	for (auto p = allMatches.begin(); p != allMatches.end(); p++)
	{
		cout << "Genome " << p->m_genomeNumber << " Position " << p->m_position << endl;
	}

	return true;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return false;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
