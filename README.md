## Info
Computes the cosine similarity between each pair of nodes in very large directed graphs.  
The cosine similarity between nodes $u$ and $v$ is denined as
- <img src="https://latex.codecogs.com/gif.latex?s=\text { sensor reading }  " /> 

The program is quite scallable as it avoids to compute the cosines similarity between pairs of nodes having no in-neighbors in common.  

## To compile:

gcc cosinemat.c -O9 -o cosinemat -lm -fopenmp  
gcc clean.c -O9 -o clean

## To execute

./cosinemat net.txt
- net.txt is the input directed graph "source target" on each line. node's IDs should be integer, preferably from 0 to n-1.

If the program does not scalle, because there are too many hubs (i.e. nodes with a very high in-degree), then just remove links to hubs. with the folowing command
./clean max_in-degree neti.txt neto.txt
- max_in-degree is the maximum allowed in-degree. For instance 10000.
- neti.txt is the input directed graph: "source target" on each line. node's IDs should be integer, preferably from 0 to n-1.
- neto.txt is the output directed graph: (with links to hubs removed).


## Initial contributors

Maximilien Danisch  
The branch and bound code is adapted from the java version of Manthos Letsios  
Technical consultants: Oana Balalau, Emmanuel Orsini and Mauro Sozio  
September 2016  
http://bit.ly/danisch  
maximilien.danisch@gmail.com
