## Info:

This C program computes the cosine similarity between each pair of nodes in a very large directed graph.

The cosine similarity between nodes $u$ and $v$ is defined as  
$$\frac{|\Delta^i(u) \cap \Delta^i(v)|}{\sqrt{|Delta^i(u)|\cdot |Delta^i(v)|}},$$  
where $|\Delta^i(u|$ is the set of in-neighbors of node u.

In practice, the program is quite scalable as it avoids to compute the cosine similarity between pairs of nodes having no in-neighbors in common. 

The method is particularly adapted to compute the "similarity" between users in a (directed) social network such as Twitter. It can then be used as a subroutine in a collaborative recommendation framework.

## To compile:

- gcc cosinemat.c -O9 -o cosinemat -lm -fopenmp 
- gcc clean.c -O9 -o clean

## To execute:

./cosinemat p net.txt
- p is the number of threads to use (nearly optimal degree of parallelism)
- net.txt is the input directed graph "source target" on each line. Node's IDs should be integers, preferably from 0 to n-1.

If the program does not scale, because there are too many hubs (i.e. nodes with a very high in-degree), then just remove links to hubs with the following command.

./clean max_in-degree neti.txt neto.txt
- max_in-degree is the maximum allowed in-degree. For instance 10,000.
- neti.txt is the input directed graph: "source target" on each line. node's IDs should be integer, preferably from 0 to n-1.
- neto.txt is the output directed graph: (with links to hubs removed).

## Reference:

The program is an alternative to the MapReduce method presented in "When hashes met wedges: A distributed algorithm for finding high similarity vectors. Sharma, Seshadhri and Goel. WWW2017".  
https://papers-gamma.link/static/memory/pdfs/23-p431.pdf


## Initial contributors:

Maximilien Danisch  
February 2018  
http://bit.ly/danisch  
maximilien.danisch@gmail.com
