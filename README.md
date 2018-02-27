## Info:

This C program computes the cosine similarity between each pair of nodes in a very large directed graph.

The cosine similarity between nodes $u$ and $v$ is defined as  
$$\frac{|\Delta^i(u) \cap \Delta^i(v)|}{\sqrt{|\Delta^i(u)|\cdot |\Delta^i(v)|}},$$  
where $|\Delta^i(u)|$ is the set of in-neighbors of node u (in Twitter, that would be the set of followers of $u$).

In practice, the program is quite scalable as it avoids to compute the cosine similarity between pairs of nodes having no in-neighbors in common. 

The method is particularly adapted to compute the "similarity" between users in a (directed) social network such as Twitter. It can then be used as a subroutine in a collaborative recommendation framework.

## To compile:
type "Make", or type
- gcc cosine.c -O3 -o cosine -lm -fopenmp 
- gcc rmhub.c -O3 -o rmhub

## To execute:

./cosine p net.txt
- p is the number of threads to use (nearly optimal degree of parallelism)
- net.txt is the input directed graph "source target" on each line. Node's IDs should be integers, preferably from 0 to n-1.  
It will print values in the terminal to plot a histogram with 0.1 bucketsize.

The program will be faster if the input graph has small out-degrees. Indeed, the running time is in $O(\sum_{u\in V} d^o(u)^2)$.

If the program does not scale, because there are too many nodes with a very high out-degree (or in-degree), then just remove out-links from these out-hubs (and/or in-links from these in-hubs) with the following command.

./rmhub max_in-degree max_out-degree neti.txt neto.txt

- max_in-degree is the maximum allowed in-degree. For instance 10,000. Use -1 to set it to infinity.
- max_out-degree is the maximum allowed out-degree. For instance 100,000. Use -1 to set it to infinity.
- neti.txt is the input directed graph: "source target" on each line. node's IDs should be integer, preferably from 0 to n-1.
- neto.txt is the output directed graph: (with links to hubs removed).

## Modification:

The code can be modified to obtain any other wished output, such as each pair of nodes with the similarity larger than a threshold (the output consisting of all pairs of nodes with nonzero similarity might be too large and not so useful).

The code can be modified to compute any similarity bettween nodes $u$ and $v$ of the form  
$$f(|\Delta^i(u)|,|\Delta^i(v)|, |\Delta^i(u)\cup \Delta^i(v)|, |\Delta^i(u)\cap \Delta^i(v)|).$$  
Such as:
- Jaccard: $\frac{|\Delta^i(u)\cap \Delta^i(v)|}{|\Delta^i(u)\cup \Delta^i(v)|}$
- F1: $2\cdot \frac{|\Delta^i(u)\cap \Delta^i(v)|}{|\Delta^i(u)|+|\Delta^i(v)|}$

Computing Adammic-Adar is also possible: https://it.wikipedia.org/wiki/Coefficiente_Adamic/Adar


## Performance:
On a commodity machine using a single thread and without removing hubs' edges:
- http://snap.stanford.edu/data/cit-Patents.html (16M edges): 15 seconds
- http://snap.stanford.edu/data/soc-LiveJournal1.html (69M edges): 2 minutes

On a commodity cluster (Intel(R) Xeon(R) CPU E5-4617 0 @ 2.90GHz) using a single thread and without removing hubs' edges:
- http://konect.uni-koblenz.de/networks/friendster (2.5G edges (<50G of RAM needed)): 50 hours and 420,903,466,824 non-zero similarity values
- http://konect.uni-koblenz.de/networks/twitter_mpi (1.9G edges (<40G of RAM needed)): 70 hours and 1,655,243,811,234 non-zero similarity values  
The maximum out-degree in twitter_mpi is 779,958. If "rmhub" is used to reduce this maximum out-degree to 10,000, we have (1.8G edges): 40 hours and 424,030,646,878 non-zero similarity values

Note that the degree of parallelism is very good: if 10 threads are used then the running time is divided by 10.

## Reference:

The program is an alternative to the MapReduce method presented in "When hashes met wedges: A distributed algorithm for finding high similarity vectors. Sharma, Seshadhri and Goel. WWW2017".  
https://papers-gamma.link/paper/23

The program shows that a "smart" brute-force approach is relatively scalable for this problem. The only problem being the RAM: it does not scale if the input graph does not fit in RAM (i.e., if a 2 integers for each edge in the graph cannot be stored in RAM).
 
Graph compression à la Boldi-Vigna could be a solution: http://law.di.unimi.it/datasets.php

## Initial contributors:

Maximilien Danisch  
February 2018  
http://bit.ly/danisch  
maximilien.danisch@gmail.com
