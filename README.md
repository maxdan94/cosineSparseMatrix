## Info:

This C program computes the cosine similarity between each pair of nodes in a very large directed graph.

The cosine similarity between nodes $u$ and $v$ is defined as  
$$\frac{|\Delta^i(u) \cap \Delta^i(v)|}{\sqrt{|\Delta^i(u)|\cdot |\Delta^i(v)|}},$$  
where $|\Delta^i(u)|$ is the set of in-neighbors of node u.

In practice, the program is quite scalable as it avoids to compute the cosine similarity between pairs of nodes having no in-neighbors in common. 

The method is particularly adapted to compute the "similarity" between users in a (directed) social network such as Twitter. It can then be used as a subroutine in a collaborative recommendation framework.

## To compile:

- gcc cosinemat.c -O3 -o cosinemat -lm -fopenmp 
- gcc clean.c -O3 -o clean

## To execute:

./cosinemat p net.txt
- p is the number of threads to use (nearly optimal degree of parallelism)
- net.txt is the input directed graph "source target" on each line. Node's IDs should be integers, preferably from 0 to n-1.

It will print values in the terminal to plot a histogram with 0.1 bucketsize.   
The code can be modified for any other wished output, such as each pair of nodes with the similarity larger than a threshold (the output consisting of all pairs of nodes with nonzero similarity might be too large and not so useful).

If the program does not scale, because there are too many nodes with a very high out-degree (or in-degree), then just remove out-links from these out-hubs (and/or in-links from these in-hubs) with the following command.

./clean max_in-degree max_out-degree neti.txt neto.txt
- max_in-degree is the maximum allowed in-degree. For instance 10,000. Use -1 to set it to infinity.
- max_out-degree is the maximum allowed out-degree. For instance 100,000. Use -1 to set it to infinity.
- neti.txt is the input directed graph: "source target" on each line. node's IDs should be integer, preferably from 0 to n-1.
- neto.txt is the output directed graph: (with links to hubs removed).

## Performance:
On a commodity machine using a single thread and without removing hubs' edges:
- http://snap.stanford.edu/data/cit-Patents.html (16M edges): 30 seconds
- http://snap.stanford.edu/data/soc-LiveJournal1.html (69M edges): 15 minutes

- http://konect.uni-koblenz.de/networks/twitter_mpi (1.4G edges):
- http://konect.uni-koblenz.de/networks/friendster (2.5G edges):

## Reference:

The program is an alternative to the MapReduce method presented in "When hashes met wedges: A distributed algorithm for finding high similarity vectors. Sharma, Seshadhri and Goel. WWW2017".  
https://papers-gamma.link/static/memory/pdfs/23-p431.pdf


## Initial contributors:

Maximilien Danisch  
February 2018  
http://bit.ly/danisch  
maximilien.danisch@gmail.com
