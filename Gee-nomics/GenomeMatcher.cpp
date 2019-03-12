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
	int m_minSearchLength;
	vector<Genome> m_genomes;
	Trie<DNAMatch> m_genomeTrie;
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
		DNAMatch match;
		match.genomeName = genome.name();
		match.length = m_minSearchLength;
		match.position = i;
		m_genomeTrie.insert(fragment, match);
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return m_minSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	// empty the matches vector
	matches.clear();

	// Return false if
	// fragment's length is less than minimumLength, or
	// minimumLength is less than the m_minSearchLength
	if (fragment.length() < minimumLength ||
		minimumLength < m_minSearchLength)
		return false;

	// Attempt to find matches of length m_minSearchLength in the GenomeMatcher Trie
	vector<DNAMatch> potentialMatches = m_genomeTrie.find(fragment.substr(0,m_minSearchLength), exactMatchOnly);

	// TODO: Make more efficient

	// Look for a match of the rest of the fragement
	// Loop through all the potential matches
	for (auto p = potentialMatches.begin(); p != potentialMatches.end(); p++)
	{
		// Loop through all the available genomes
		for (int i = 0; i < m_genomes.size(); i++)
		{
			// If their names match
			if (p->genomeName == m_genomes[i].name())
			{
				string tempString;
				// Check if the user requested an exact match
				int mismatchesFound;
				if (exactMatchOnly)
					mismatchesFound = 1;
				else
					mismatchesFound = 0;
				// extract the rest of the string into tempString
				if (m_genomes[i].extract(p->position, fragment.length(), tempString))
				{
					// Compare each character
					int lengthOfMatch = 0;
					for ( ; lengthOfMatch < tempString.length() && mismatchesFound < 2; lengthOfMatch++)
					{
						if (fragment[lengthOfMatch] != tempString[lengthOfMatch])
							mismatchesFound++;
					}
					// If the loop broke because of too many mismatches
					// decrease the length of the match by one
					if (mismatchesFound >= 2)
						lengthOfMatch--;
					// If a long enough match was found, add it to the match vector
					if (lengthOfMatch >= minimumLength)
					{
						// Check if the matches vector already has a match from the same genome
						bool containsLongerMatchFromGenome = false;
						auto matchIterator = matches.begin();
						while ( matchIterator != matches.end())
						{
							if (matchIterator->genomeName == p->genomeName)
							{
								// delete the shorter match from the same genome
								if (matchIterator->length < lengthOfMatch)
								{
									auto q = matches.erase(matchIterator);
									matchIterator = q;
								}
								else
								{
									containsLongerMatchFromGenome = true;
									matchIterator++;
								}
							}
							else
								matchIterator++;
						}
						if (!containsLongerMatchFromGenome)
						{
							DNAMatch match;
							match.genomeName = p->genomeName;
							match.length = lengthOfMatch;
							match.position = p->position;
							matches.push_back(match);
						}
					}
				}
			}
		}
	}

	/*
	// TODO: REMOVE
	// Temporarily print out all the matches
	cout << endl << "Matches Found for " << fragment << " of minimum search length " << minimumLength << ":" << endl;
	for (auto p = matches.begin(); p != matches.end(); p++)
	{
		cout << p->genomeName << " of length " << p->length << " at position " << p->position << endl;
	}
	*/


	// Return false if there were no matches found
	if (matches.size() == 0)
		return false;
	else
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
