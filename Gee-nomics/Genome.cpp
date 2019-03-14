#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string m_nm;
	string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
	:m_nm(nm), m_sequence(sequence)
{}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	/*
	Improper formatting includes things like not starting with a name line,
	non-name lines containing any characters other than upper or lower case A C T G N, no
	base lines after a name line, empty lines, or a line starting with a greater-than character
	but containing no other characters.
	*/
	string currentLine;
	string name = "";
	string sequence = "";
	while (getline(genomeSource, currentLine))
	{
		// Check for empty line
		if (currentLine == "" || currentLine[0] == ' ')
			return false;
		// If it is a name line
		if (currentLine[0] == '>')
		{
			// If there was a previous genome
			// add it to the genome vector
			if (name != "" && sequence != "")
			{
				genomes.push_back(Genome(name, sequence));
				// reset name and sequence
				name = "";
				sequence = "";
			}
			// Error if there were no base lines after name line
			// by checking if name has a value but sequence is empty
			// Error if there was a line starting with > but no other characters
			// by checking if name is empty but sequence has a value
			else if (name != "" && sequence == "")
			{
				return false;
			}
			name = currentLine.substr(1);
			// Check if there was a line starting with >
			// character but containing no other characters.
			if (name.length() == 0)
				return false;
		}
		// It is a sequence
		else
		{
			// Check if it didn't start with a name line
			if (name == "")
				return false;
			// Check if all characters in the sequence are valid
			for (int i = 0; i < currentLine.length(); i++)
			{
				char currentChar = toupper(currentLine[i]);
				if (currentChar == 'A' ||
					currentChar == 'C' ||
					currentChar == 'T' ||
					currentChar == 'G' ||
					currentChar == 'N')
					sequence += currentChar;
				else
					return false;
			}
		}
	}
	// end of file was reached
	// try to add the last genome
	if (name != "" && sequence != "")
	{
		genomes.push_back(Genome(name, sequence));
	}
	// Error if there were no base lines after name line
	// by checking if name has a value but sequence is empty
	// Error if there was a line starting with > but no other characters
	// by checking if name is empty but sequence has a value
	else if ((name != "" && sequence == "") ||
				(name == "" && sequence != ""))
	{
		return false;
	}
	return true;
}

int GenomeImpl::length() const
{
	return m_sequence.length();
}

string GenomeImpl::name() const
{
	return m_nm;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if ((position + length) > this->length())
		return false;
	else
	{
		fragment = m_sequence.substr(position, length);
		return true;
	}
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
