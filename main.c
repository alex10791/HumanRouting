
#include <stdio.h>
#define N 5
#define INF 0x7FFFFFFF

// alex function definitions
void getPath(int P[][N], int i, int j, int paths[], int k);		//, int paths[], int k
void node_compression(int dist[][N], int p[][N], int path[N], int paths[N][N][N], int compressed_nodes[][N], int nodes_of_interest[], int mapping_of_nodes[N], int nodes_of_interest_count);

// constantina function definitions
int finalizePath( int *minimum_path, int dist[N][N],int  ARRAY_SIZE, int id);
void swap(int *x, int *y);
int min(int x1, int x2);
void permute(int *a, int l, int r, int dist[N][N],int *minimum_distance, int *minimum_path, int ARRAY_SIZE, int id);


int main(void) {
	// alex variables
	int id = 2;
	int dist[N][N];  // For some N
	int p[N][N];
	int path[N];
	int paths[N][N][N];
	int compressed_nodes[N][N];
	int i, j, k;
	int nodes_of_interest[N];
	int mapping_of_nodes[N];
	int nodes_of_interest_count = 0;
	int final_path[N*N];

	// constantinas variables
	int min_dist= INF;
	int minimum_path[N];

	// NODES OF INTEREST
	//for (i = 0; i < N; ++i) {
	//	nodes_of_interest[i] = -1;
	//}

	// alex main
	nodes_of_interest_count = 4;
	nodes_of_interest[0] = 1;
	nodes_of_interest[1] = 0;
	nodes_of_interest[2] = 1;
	nodes_of_interest[3] = 1;
	nodes_of_interest[4] = 1;

	mapping_of_nodes[0] = 0;
	mapping_of_nodes[1] = 2;
	mapping_of_nodes[2] = 3;
	mapping_of_nodes[3] = 4;
	mapping_of_nodes[4] = -1;


	for (i = 0; i < N*N; ++i) {
		final_path[i] = -1;
	}

	// CREATE TOPOLOGY
	for (i = 0; i < N; ++i) {
	 	for (j = 0; j < N; ++j) {
	 		if (i == j) {
	 			dist[i][j] = 0;
	 			compressed_nodes[i][j] = 0;
	 		} else {
	 			dist[i][j] = INF;
	 			compressed_nodes[i][j] = INF;
	 		}
	 	}
	}


	dist[0][1] = 1;
	dist[1][2] = 1;
	dist[1][4] = 1;
	dist[2][1] = 1;
	dist[2][3] = 1;
	dist[3][2] = 1;
	dist[3][4] = 1;
	dist[4][1] = 1;
	dist[4][3] = 1;
	dist[1][0] = 1;


	

	// init parent matrix
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (dist[i][j] != 0 && dist[i][j] != INF) {
				p[i][j] = i;
			} else {
				p[i][j] = -1;
			}
		}
	}

	

	node_compression(dist, p, path, paths, compressed_nodes, nodes_of_interest, mapping_of_nodes, nodes_of_interest_count);




	printf("\n\n\n");



	for (i = 0; i < nodes_of_interest_count; ++i) {
		for (j = 0; j < nodes_of_interest_count; ++j) {
			//int idx = nodes_of_interest[i];
			//if (idx != -1 && paths[idx] == ) {
			printf("%d\t", compressed_nodes[i][j]);
			//}
		}
		printf("\n");
	}



	printf("DIO SOU TOUTA\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (compressed_nodes[i][j] == INF) {
				printf("INF\t");
			} else {
				printf("%d\t", compressed_nodes[i][j]);
			}
		}
		printf("\n");
	}

	printf("%d\n", nodes_of_interest_count);
	printf("OS DAME\n");


	// constantinas main
	int id_mapping = -1;
	for (i = 0; i < N; ++i) {
		if (mapping_of_nodes[i] == id) {
			id_mapping = i;
		}
	}

	if (id_mapping == -1) {
		printf("SHOULD NEVER HAPPEN\n");
		return 1;
	}

	min_dist = finalizePath(minimum_path, compressed_nodes, nodes_of_interest_count, id_mapping);
	
    printf("Minimum distance: %d\n", min_dist);
	printf("%d %d %d \n", minimum_path[0],minimum_path[1],minimum_path[2]);


	for (i = 0; i < N; i++) {
		printf("%d ", minimum_path[i]);
	}	
	printf("\n");









	int idx = 0;
	j = 0;
	while(idx < N-1 && minimum_path[idx] != -1 && minimum_path[idx+1] != -1) {
		i = 1;
		while (i < N && paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i] != -1) {
			final_path[j] = paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i];
			printf("paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i] : %d\n", paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i]);
			printf("mapping_of_nodes[minimum_path[idx]] : %d\n", mapping_of_nodes[minimum_path[idx]]);
			printf("mapping_of_nodes[minimum_path[idx+1]] : %d\n", mapping_of_nodes[minimum_path[idx+1]]);
			printf("minimum_path[idx] : %d\n", minimum_path[idx]);
			printf("minimum_path[idx+1] : %d\n", minimum_path[idx+1]);
			printf("idx : %d\n", idx);
			printf("idx+1 : %d\n", idx+1);
			printf("i : %d\n", i);
			printf("j : %d\n", j);



			i++;
			j++;
		}
		idx++;
	}

	for (i = 0; i < N*N; i++) {
		if (final_path[i] == -1) {
			break;
		}
		printf("%d ", final_path[i]);
	}

	printf("\n");

	return 0;
}


void getPath(int P[][N], int i, int j, int path[], int k) { 
	if (i == j) {
		//printf("%d ", i);
		path[k] = i;
		++k;
	}
	else if (P[i][j] == -1) {
		//printf("No Path");
		path[k] = -2;
		k = 0;
	}
	else {
		path[k] = j;
		++k;
		getPath(P, i, P[i][j], path, k); 
		//printf("%d ", j);
	}
}











void node_compression(int dist[][N], int p[][N], int path[N], int paths[N][N][N], int compressed_nodes[][N], int nodes_of_interest[], int mapping_of_nodes[N], int nodes_of_interest_count) {

	int i, j, k;

	// Input data into dist, where dist[i][j] is the distance from i to j.
	 
   	for ( k = 0; k < N; k++ )
      	for ( i = 0; i < N; i++ )
	        for ( j = 0; j < N; j++ ) {
	            //dist[i][j] = min( dist[i][j], dist[i][k] + dist[k][j] );
	            if (dist[i][k] == INF || dist[k][j] == INF) {
					continue;                 
				}
				if (dist[i][j] > dist[i][k] + dist[k][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
					p[i][j] = p[k][j];
				}
	 		}
	// Now the shortest distance between i and j will be in dist[i][j].


	
	



	printf("\nShortest Paths\n--------------------------------\n\n");
	printf("\t");
    for (int i = 0; i < N; ++i) { 
		printf("%d\t", i);
	}
	printf("\n");
	for (int i = 0; i < N; ++i) {
		printf("%d\t", i);
	 	for (int j = 0; j < N; ++j) {
	 		printf("%d\t", dist[i][j]);
	 	}
	 	printf("\n");
	}



	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (nodes_of_interest[i] == 1 && nodes_of_interest[j] == 1) {
				for (k = 0; k < N; ++k) {
					path[k] = -1;
				}
				printf("Path from %d to %d: ", i, j );
				getPath(p, i, j, path, 0);
				int len = 0;
				while (path[++len]!=-1);
				printf("len = %d\n", len);
				int interesting = 1;
				if (len == 1) {
					interesting = 0;
				}
				for (k = 1; k < (len-1); ++k) {
					if (nodes_of_interest[path[k]] == 1) {
						interesting = 0;
					}
				}
				if (interesting == 1) {
					int x, y;
					for (k = 0; k < N; ++k) {
						if (mapping_of_nodes[k] == i) {
							x = k;
						}
						if (mapping_of_nodes[k] == j) {
							y = k;
						}
					}
					compressed_nodes[x][y] = dist[i][j];
					for (k = 0; k < N; ++k) {
						printf("%d\t", path[k]);
						paths[i][j][k] = path[k];
					}
					printf("\n");
				}
				
			} //else {
				//for (k = 0; k < N; ++k) {
				//	path[k] = -5;
				//}
			//}
		}
	}
	





	for (i = 0; i < nodes_of_interest_count; ++i) {
		for (j = 0; j < nodes_of_interest_count; ++j) {
			//int idx = nodes_of_interest[i];
			//if (idx != -1 && paths[idx] == ) {
			printf("%d\t", compressed_nodes[i][j]);
			//}
		}
		printf("\n");
	}






}

	
	
	











// CONSTANTINA ALG

void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
int min(int x1, int x2){
	if (x1 < x2)
		return x1;
	return x2;
}
void permute(int *a, int l, int r, int dist[N][N],int *minimum_distance, int *minimum_path, int ARRAY_SIZE, int id)
{
   int i;
   int c1,c2;
   int distance=0;
   if (l == r){
   		if (id == a[0]) {
	        printf("%d %d %d \n", a[0],a[1],a[2]);
	        printf("%d %d\n", dist[a[0]][a[1]], dist[a[1]][a[2]] );
	        printf("======\n");
	        for (c1=0; c1<ARRAY_SIZE-1; ++c1){
	        		if (distance == INF || dist[a[c1]][a[c1+1]] == INF) {
	        			distance = INF;
	        		} else {
	        			distance = distance + dist[a[c1]][a[c1+1]] ;
	        		}
	    			
	    		}	
	    	printf("distance is: %d\n", distance);
	    	printf("======\n");
	    	if (*minimum_distance > distance){
	    		*minimum_distance = distance;
	    		for (c2=0; c2<ARRAY_SIZE; ++c2){
	    			minimum_path[c2] = a[c2];
				}
	    		printf("Current Min Path\n\n");
	    		printf("%d %d %d\n", a[0],a[1],a[2]);
	   			printf("%d %d %d\n", minimum_path[0],minimum_path[1],minimum_path[2]);
	   			printf("\n\n");
			}
		}
	} 
   else
   {
       for (i = l; i <= r; i++)
       {
          swap((a+l), (a+i));
          permute(a, l+1, r, dist,minimum_distance, minimum_path, ARRAY_SIZE, id);
          swap((a+l), (a+i)); 
       }
   }
}

int finalizePath( int *minimum_path, int dist[N][N],int  ARRAY_SIZE, int id){
	int str[N] ;
	int i=0;
	int minimum = INF;
	for (i=0; i<N; i++){
		minimum_path[i]=-1;
		str[i]=i;
	}
    permute(str, 0, ARRAY_SIZE-1, dist, &minimum, minimum_path, ARRAY_SIZE, id);
    
    printf("Minimum distance: %d\n", minimum);
	printf("%d %d %d \n", minimum_path[0],minimum_path[1],minimum_path[2]);
	return minimum;
	
}
 



