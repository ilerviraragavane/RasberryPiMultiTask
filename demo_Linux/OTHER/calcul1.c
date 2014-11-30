#include <stdio.h>
#include <sys/time.h>

int main(){
	
	struct timeval tv_begin;
	gettimeofday (&tv_begin, NULL);
	
	int k = 1;
	int i,j;
	for(i=0; i<10000; i++){
		for(j=0; j<20000; j++){
			k = k*1;		
		}
		
	}
	
	struct timeval tv_end;
	gettimeofday (&tv_end, NULL);
	
	printf("time = %lu\n", 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec );
	
	return 0;
}
