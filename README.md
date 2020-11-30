# DBMS-Implementation
## Empirically Benchmarking Tree-based and Hash-based Database Implementations
### To compile the program: 
```zsh
$ make
```

### To run the benchmark:
```zsh
$ ./benchmark.e <number of records to insert into data structures -- defaults to 150>
```
### Goal / Approach
For the CPS377 Final Project we developed a C implementation of a Linear Hash Table and a B+-Tree to compare their benefits and downsides. We expected to demonstrate that B+-trees are better at range queries and hash tables are better optimized for key equality searches.

### Page Simulation Design

Directly collecting data on page hits was infeasible for a number of reasons, primarily time: such data would require manipulation of the Linux kernel to collect, which would have added unrealistic amounts of work to the project. Moroever, we wanted to be able to modify the page size so that we could test in small scale and benchmark across a variety of page sizes. We thus needed a way to simulate page fetching within the program itself. The solution to this is the `pageManager` struct, which is a simple set of counters for reads and writes. Pages were abstracted to a `pageptr` struct, which contains a union of the various type of pages in play (hash buckets, tree nodes, tree leaf nodes, record pages) and a signifier to denote which type that particular pointer refers to. The data structures were then implemented based on this abstracted `pageptr`, and the algorithms call the `pageManager` every time they perform an action that would require fetching a new page (e.g. going to a bucket or traversing to a tree node).

The "page" size can then be set at compile time with a pre-processing directive, with the sizes of the individual types of pages calculated based on the sizes of their component structs; so a tree node, composed of a 64-bit int and an array of key/pointer unions contains as many of those unions as fit in the page size, rounded down to an odd number.  This arithmetic was the source of a number of very difficult errors to chase down, as it relies on certain assumptions about the sizes of various structs that may be invalidated based on how the C compiler packs members into a struct; this caused a number of memory errors, and so led to the implementation of a `checkSizes` function that compares the expected values against the actual at the initalization of the page manager and terminates the program if the values disagree.
 
### Results

Our prediction from our project proposal (which is supported by theory) that B+-trees are better at range queries ([Figure 1](Documents/searchReadsPlot.png)) and hash tables are better optimized for key equality searches ([Figure 2](Documents/rangeSearchReadsPlot.png)), was supported by our empirically benchmarked results.

#### Range Search

[Figure 1](Documents/searchReadsPlot.png) demonstrates the performance of a range query of the entire data structure for both the the linear hash and tree implementation. The tree performs overwhelming better than the hash table for range searches, the performance gap only widens the larger the range of the search. The tree performs roughly 20 times better than the hash table on average. Range search is the worst case for hash-based databases.

#### Search

[Figure 2](Documents/rangeSearchReadsPlot.png) demonstrates the performance of a key equality searches of the entire data structure for both the the linear hash and tree implementation. The hash table performs overwhelming better than the tree for searches. This, of course, makes sense because in key equality searches the tree cannot benefit from the links between the leaf nodes like it does in range search. The hash table performs roughly 6 times better than the tree on average. Key equality search is the worst case for tree-based databases.
