#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

// Sort GenomeMatch objects
bool wayToSort(GenomeMatch a, GenomeMatch b)
{
	if (a.percentMatch != b.percentMatch)
		return (a.percentMatch > b.percentMatch);
	else
		return (a.genomeName < b.genomeName);
}

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	struct TrieNode
	{
		int genomeIndex;
		int length;
		int position;
	};
	int m_minSearchLength;
	vector<Genome> m_genomes;
	Trie<TrieNode> m_genomeTrie;
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
		TrieNode node;
		node.genomeIndex = m_genomes.size()-1;
		node.length = m_minSearchLength;
		node.position = i;
		m_genomeTrie.insert(fragment, node);
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
	vector<TrieNode> potentialMatches = m_genomeTrie.find(fragment.substr(0, m_minSearchLength), exactMatchOnly);

	// Look for a match of the rest of the fragement
	// Loop through all the potential matches
	for (auto p = potentialMatches.begin(); p != potentialMatches.end(); p++)
	{
		string tempString;
		// Check if the user requested an exact match
		int mismatchesFound;
		if (exactMatchOnly)
			mismatchesFound = 1;
		else
			mismatchesFound = 0;
		// extract the rest of the string into tempString
		if (m_genomes[p->genomeIndex].extract(p->position, fragment.length(), tempString))
		{
			// Compare each character
			int lengthOfMatch = 0;
			for (; lengthOfMatch < tempString.length() && mismatchesFound < 2; lengthOfMatch++)
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
				while (matchIterator != matches.end())
				{
					if (matchIterator->genomeName == m_genomes[p->genomeIndex].name())
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
				// If there wasn't a shorter match in matches
				// insert the current data
				if (!containsLongerMatchFromGenome)
				{
					DNAMatch match;
					match.genomeName = m_genomes[p->genomeIndex].name();
					match.length = lengthOfMatch;
					match.position = p->position;
					matches.push_back(match);
				}
			}
		}
	}

	// Return false if there were no matches found
	if (matches.size() == 0)
		return false;
	else
		return true;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	// Check if the function call was invalid
	if (fragmentMatchLength < m_minSearchLength)
		return false;

	// Clear the results vector
	results.clear();
	// Declare the variables needed
	string fragment;
	vector<DNAMatch> matches;
	unordered_map<string, int> numOfGenomeMatches;
	int i = 0;
	int numberOfSequences = 0;

	// Extract fragments from the Genome
	while (query.extract(i, fragmentMatchLength, fragment))
	{
		// Check if other Genomes match the fragment
		findGenomesWithThisDNA(fragment, fragmentMatchLength, exactMatchOnly, matches);
		// Add the results to an unordered_map
		for (auto p = matches.begin(); p != matches.end(); p++)
		{
			// Copied from www.techiedelight.com/increment-map-value-associated-with-key-cpp/
			// check if genomeName exists in the map or not
			std::unordered_map<string, int>::iterator it = numOfGenomeMatches.find(p->genomeName);

			// If the key was already present in the map
			// increment map's value for genomeName
			if (it != numOfGenomeMatches.end()) {
				it->second++;
			}
			// Otherwise, the key was not found
			// insert it into the unordered_map
			else {
				numOfGenomeMatches.insert(make_pair(p->genomeName, 1));
			}
		}
		i += fragmentMatchLength;
		numberOfSequences++;
	}
	double percent;
	// If the matches cross the treshhold
	// add them to the results vector
	for (auto p = numOfGenomeMatches.begin(); p != numOfGenomeMatches.end(); p++)
	{
		percent = ((p->second) * 100) / numberOfSequences;
		if (percent >= matchPercentThreshold)
		{
			int lengthOfQuery = (query.name()).length();
			GenomeMatch matchingGenome;
			matchingGenome.genomeName = p->first;
			matchingGenome.percentMatch = percent;
			results.push_back(matchingGenome);
		}
		sort(results.begin(), results.end(), wayToSort);
	}

	// Return false if there were no matches which crossed the threshhold.
	if (results.size() > 0)
		return true;
	else
		return false;
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
