/*
gcc clean.c -O9 -o clean
./clean max_in-degree net_input net_output
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NNODES 100000000 //Maximum number of node, will automatically increase if needed.

//compute the maximum of three unsigned
unsigned max3(unsigned a,unsigned b,unsigned c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

int main(int argc,char** argv){
	unsigned s,t,n=0,n_max=NNODES,max=atoi(argv[1]);
	unsigned *d=malloc(n_max*sizeof(unsigned));
	FILE *file1,*file2;

	printf("Maximum in-degree allowed = %u\n",max);

	printf("Reading edgelist from file %s\n",argv[2]);
	file1=fopen(argv[1],"r");
	while (fscanf(file1,"%u %u\n", &s, &t)==2) {
		n=max3(n,s+1,t+1);
		if (n>n_max) {
			d=realloc(d,(n+NNODES)*sizeof(unsigned));
			bzero(d+n_max,(n+NNODES-n_max)*sizeof(unsigned));
			n_max=n+NNODES;
		}
		d[t]++;
	}
	fclose(file1);

	printf("Writting edgelist in file %s\n",argv[3]);
	file1=fopen(argv[1],"r");
	file2=fopen(argv[2],"w");
	while (fscanf(file1,"%u %u\n", &s, &t)==2) {
		if (d[t]<max){
			fprintf(file2,"%u %u\n", s, t);
		}
	}
	fclose(file1);
	fclose(file2);

	return 0;
}

