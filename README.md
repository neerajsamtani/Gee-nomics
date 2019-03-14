# Gee-nomics

In this project, I wrote three classes that can be used to process genetic data.
These three classes can be used together to enable the following use cases:
  * Maintain a library of genomes from multiple organisms; the user can add new
genomes to this library.
  * Allow the user to search the library for a specified DNA sequence and identify all
genomes in the library that contain that DNA sequence or any SNiP of it.
  * Allow the user to present the genome/DNA of a new organism and quickly
identify all genomes in the library that have a high percentage of matching DNA
with the presented genome.

### providedTests.cpp

Allows the user to test and use the software.

### Trie.h

Trie.h is a template-based multi-map class, based on the Trie data structure. It can 
be used to map any C++ string of characters to one or more values (of the templated type).

When the user searches for a key string (e.g. “ACTGGTCA”), the object not only provides 
all values associated with the exact, searched-for key string, but it also is able to 
return all values associated with any string in the multi-map which differs by at most 
one character from the searched for key string. So, when searching a Trie, the user can 
request that the class return either (a) only exact matches, or (b) both exact matches 
AND matches that differ by exactly one character (except that the first character of the 
search string must not differ).

Trie data structures have a variety of applications, from managing genomes to 
helping implement autocorrect!

### Genome.cpp

Genome that can be used to load organisms' genomes from a data file and, once loaded, 
allow a user of the class to (a) obtain an organism’s name, and (b) extract any 
subsequence of an organism’s DNA genome.

### GenomeMatcher.cpp

GenomeMatcher maintains a library of Genome objects and allows the user of the class to:
  * Add a new organism’s Genome to the library
  * Search all the genomes held in the library for a given DNA sequence, e.g. “Find
the names of all genomes in the library whose DNA sequence contains
‘ACCATGGATTACA’ or some SNiP of that sequence such as
ACCATGAATTACA’, and tell me at what offset(s) these located sequences were
found in each genome.”
  * Search the library to identify all genomes in the library whose DNA contains at
least T% overlap with a given Genome, where the threshold T is specified by the
user (e.g., T = 15%). This might be used to identify organisms whose genomes
are closely related to a queried organism’s genome

### provided.h

Contains required structs used by other classes and forwards function calls to their
implementations

### /data

The data folder has example genome files in `.txt` files that can be used by the classes.

---

This was developed for the CS32 course at UCLA
