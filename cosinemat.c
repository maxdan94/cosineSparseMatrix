/*
gcc cosinemat.c -O9 -o cosinemat -lm -fopenmp
./cosinemat net
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>


#define NLINKS 500000000 //Maximum number of links, will automatically increase if needed.

typedef struct {
	unsigned s;
	unsigned t;
} edge;

typedef struct {
	//edge list structure:
	unsigned n;//number of nodes
	unsigned e;//number of edges
	edge *edges;//list of edges

	//neighborhoods:
	unsigned *d_o; //out degrees
	unsigned *cd_o; //cumulative out-degrees: (start with 0) length=dim+1
	unsigned *adj_o; //list of out-neighbors
	unsigned *d_i; //in degrees
	unsigned *cd_i; //cumulative in-degrees: (start with 0) length=dim+1
	unsigned *adj_i; //list of in-neighbors
} digraph;


//compute the maximum of three unsigned
unsigned max3(unsigned a,unsigned b,unsigned c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the edgelist from file
digraph* readedgelist(char* edgelist){
	unsigned e1=NLINKS;
	digraph *g=malloc(sizeof(digraph));
	FILE *file;

	g->n=0;
	g->e=0;
	file=fopen(edgelist,"r");
	g->edges=malloc(e1*sizeof(edge));
	while (fscanf(file,"%u %u\n", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if (g->e++==e1) {
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);
	g->n++;
	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}

//Building the special graph structure
void mkdigraph(digraph *g){
	unsigned i,s,t,max,tmp;
	unsigned *d=calloc(g->n,sizeof(unsigned));

	g->cd_o=malloc((g->n+1)*sizeof(unsigned));
	g->adj_o=malloc((g->e)*sizeof(unsigned));
	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
	}
	g->cd_o[0]=0;
	max=0;
	for (i=1;i<g->n+1;i++) {
		g->cd_o[i]=g->cd_o[i-1]+d[i-1];
		max=(d[i-1]>max)?d[i-1]:max;
	}
	printf("Maximum out-degree: %u\n",max);
	bzero(d,(g->n)*sizeof(unsigned));
	for (i=0;i<g->e;i++) {
		s=g->edges[i].s;
		g->adj_o[g->cd_o[s] + d[s]++ ]=g->edges[i].t;
	}
	bzero(d,(g->n)*sizeof(unsigned));
	g->cd_i=malloc((g->n+1)*sizeof(unsigned));
	g->adj_i=malloc((g->e)*sizeof(unsigned));
	for (i=0;i<g->e;i++) {
		d[g->edges[i].t]++;
	}
	g->cd_i[0]=0;
	max=0;
	for (i=1;i<g->n+1;i++) {
		g->cd_i[i]=g->cd_i[i-1]+d[i-1];
		max=(d[i-1]>max)?d[i-1]:max;
	}
	printf("Maximum in-degree: %u\n",max);
	bzero(d,(g->n)*sizeof(unsigned));
	for (i=0;i<g->e;i++) {
		t=g->edges[i].t;
		g->adj_i[g->cd_i[t] + d[t]++ ]=g->edges[i].s;
	}

	free(d);
}

void freedigraph(digraph *g){
	free(g->edges);
	free(g->cd_o);
	free(g->adj_o);
	free(g->cd_i);
	free(g->adj_i);
	free(g);
}

//histogram of cosine values
unsigned long long* cosine(digraph *g){
	unsigned i,j,k,u,v,w,n;
	double val;
	unsigned long long *hist_p,*hist=calloc(10,sizeof(unsigned long long));
	bool *tab,*done;
	unsigned *list;
	#pragma omp parallel private(i,j,k,u,v,w,val,tab,hist_p,done,list,n)
	{
	hist_p=calloc(10,sizeof(unsigned long long));
	tab=calloc(g->n,sizeof(bool));
	done=calloc(g->n,sizeof(bool));
	list=malloc(g->n*sizeof(unsigned));

	#pragma omp for schedule(dynamic, 1) nowait
	for (u=0;u<g->n;u++){//embarrassingly parallel...
		n=0;
		for (i=g->cd_i[u];i<g->cd_i[u+1];i++){
			tab[g->adj_i[i]]=1;
		}
		for (i=g->cd_i[u];i<g->cd_i[u+1];i++){
			v=g->adj_i[i];
			for (j=g->cd_o[v];j<g->cd_o[v+1];j++){
				w=g->adj_o[j];
				if (w>u && done[w]==0){//make sure that (u,w) is processed only once
					done[w]=1;
					list[n++]=w;
					val=0.;
					for (k=g->cd_i[w];k<g->cd_i[w+1];k++){
						if (tab[g->adj_i[k]]==1){
							val++;
						}
					}
					val/=sqrt(((double)(g->cd_i[u+1]-g->cd_i[u]))*((double)(g->cd_i[w+1]-g->cd_i[w])));
					if (val>0.9){
						hist_p[9]++;
					}
					else {
						hist_p[(int)(floor(val*10))]++;
					}
					//printf("%u %u %le\n",u,w,val);//to print the pairs and similarity
				}
			}
		}
		for (i=g->cd_i[u];i<g->cd_i[u+1];i++){
			tab[g->adj_i[i]]=0;
		}
		for (i=0;i<n;i++){
			done[list[i]]=0;
		}
	}
	free(tab);
	#pragma omp critical
	{
		for (i=0;i<10;i++){
			hist[i]+=hist_p[i];
		}
		free(hist_p);
	}
	}
	return hist;
}


int main(int argc,char** argv){
	digraph* g;
	unsigned i;
	unsigned long long tot=0;
	unsigned long long *hist;

	time_t t0,t1,t2;
	t1=time(NULL);
	t0=t1;

	omp_set_num_threads(atoi(argv[1]));

	printf("Reading edgelist from file %s\n",argv[2]);
	g=readedgelist(argv[2]);

	t2=time(NULL);
	printf("- Time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	t1=t2;

	printf("Number of nodes: %u\n",g->n);
	printf("Number of edges: %u\n",g->e);

	mkdigraph(g);

	printf("Computing cosine similarities\n");

	hist=cosine(g);

	t2=time(NULL);
	printf("- Time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	t1=t2;

	freedigraph(g);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t0)/3600,((t2-t0)%3600)/60,((t2-t0)%60));

	printf("Number of cosine similarities in\n");
	for (i=0;i<9;i++){
		printf("]0.%u, 0.%u] = %llu\n",i,i+1,hist[i]);
		tot+=hist[i];
	}
	printf("]0.9, 1.0] = %llu\n",hist[9]);
	tot+=hist[9];
	printf("Number of non-zero cosine similarities = %llu\n",tot);

	return 0;
}


