// C program to print all permutations with duplicates allowed
#include <stdio.h>
#define INF 0x7ffffff
#define N 5


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
void permute(int *a, int l, int r, int dist[N][N],int *minimum_distance, int *minimum_path, int ARRAY_SIZE)
{
   int i;
   int c1,c2;
   int distance=0;
   if (l == r){
        printf("%d %d %d \n", a[0],a[1],a[2]);
        printf("%d %d\n", dist[a[0]][a[1]], dist[a[1]][a[2]] );
        printf("======\n");
        for (c1=0; c1<ARRAY_SIZE-1; ++c1){
    			distance = distance + dist[a[c1]][a[c1+1]] ;
    		}	
    	printf("distance is: %d\n", distance);
    	printf("======\n");
    	if (*minimum_distance > distance){
    		*minimum_distance = distance;
    		for (c2=0; c2<ARRAY_SIZE; ++c2){
    			minimum_path[c2] = a[c2];
			}
    		printf("Current Min Path\n\n");
    		printf("%d %d %d %d\n", a[0],a[1],a[2]);
   			printf("%d %d %d %d\n", minimum_path[0],minimum_path[1],minimum_path[2]);
   			printf("\n\n");
		}
	} 
   else
   {
       for (i = l; i <= r; i++)
       {
          swap((a+l), (a+i));
          permute(a, l+1, r, dist,minimum_distance, minimum_path, ARRAY_SIZE);
          swap((a+l), (a+i)); 
       }
   }
}

int finalizePath( int *minimum_path, int dist[N][N],int  ARRAY_SIZE ){
	int str[N] ;
	int i=0;
	int minimum = INF;
	for (i=0; i<N; i++){
		minimum_path[i]=-1;
		str[i]=i;
	}
    permute(str, 0, ARRAY_SIZE-1, dist, &minimum, minimum_path, ARRAY_SIZE);
    
    printf("Minimum distance: %d\n", minimum);
	printf("%d %d %d \n", minimum_path[0],minimum_path[1],minimum_path[2]);
	return minimum;
	
}
 
int main()
{
	int ARRAY_SIZE = 3;
    int i,j;
    int dist[N][N];
    for (i=0; i< N; ++i){
    	for (j=0;j<N; ++j){
    		dist[i][j]=0;
		}
	}
	dist[0][0]=0;
	dist[0][1]=2;
	dist[0][2]=INF;
	dist[1][0]=2;
	dist[1][1]=0;
	dist[1][2]=1;
	dist[2][0]=INF;
	dist[2][1]=1;
	dist[2][2]=0;
	int min_dist= INF;
	int minimum_path[N];
	
	min_dist = finalizePath(minimum_path, dist, ARRAY_SIZE);
	
    printf("Minimum distance: %d\n", min_dist);
	printf("%d %d %d \n", minimum_path[0],minimum_path[1],minimum_path[2]);
    return 0;
   
}
