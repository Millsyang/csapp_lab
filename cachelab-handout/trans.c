/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include <math.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{	
    int i, j;
    int row;
    int x0,x1,x2,x3;
    int y0,y1,y2,y3;
    //int n,m;
    //n = ceil(N / 8);
    //m = ceil(M / 8);

    for (i = 0; i < N; i += 8) {
        for (j = 0; j < M; j += 8) {
	    for(row = i;row < i + 4;row++){
	    	x0 = A[row][j];
		x1 = A[row][j + 1];
		x2 = A[row][j + 2];
		x3 = A[row][j + 3];
		y0 = A[row + 4][j];
		y1 = A[row + 4][j + 1];
		y2 = A[row + 4][j + 2];
		y3 = A[row + 4][j + 3];
		B[j][row] = x0;
		B[j + 1][row] = x1;
		B[j + 2][row] = x2;
		B[j + 3][row] = x3;
		B[j][row + 4] = y0;
		B[j + 1][row + 4] = y1;
		B[j + 2][row + 4] = y2;
		B[j + 3][row + 4] = y3;
	    }

	    for(row = i;row < i + 4;row++){
	    	x0 = A[row][j + 4];
		x1 = A[row][j + 5];
		x2 = A[row][j + 6];
		x3 = A[row][j + 7];
		y0 = A[row + 4][j + 4];
		y1 = A[row + 4][j + 5];
		y2 = A[row + 4][j + 6];
		y3 = A[row + 4][j + 7];
		B[j + 4][row] = x0;
		B[j + 5][row] = x1;
		B[j + 6][row] = x2;
		B[j + 7][row] = x3;
		B[j + 4][row + 4] = y0;
		B[j + 5][row + 4] = y1;
		B[j + 6][row + 4] = y2;
		B[j + 7][row + 4] = y3;
	    }
		/*
	    for(row = i;row < i + 4;row++){
	    	for(col = j;col < j + 4;col++){
		    B[col][row] = A[row][col];
		}
	    }
	    */
	}
    }    
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, x, y;
    int x1,x2,x3,x4,x5,x6,x7,x8;

    for(i=0;i<N;i+=8){
			for(j=0;j<M;j+=8){
				for(x=i;x<i+8;x++){
					if(i == j){
						x1 = A[x][j];		
						x2 = A[x][j+1];	
						x3 = A[x][j+2];	
						x4 = A[x][j+3];	
						x5 = A[x][j+4];	
						x6 = A[x][j+5];	
						x7 = A[x][j+6];	
						x8 = A[x][j+7];	
						
						B[j][x] = x1;		
						B[j+1][x] = x2;	
						B[j+2][x] = x3;	
						B[j+3][x] = x4;	
						B[j+4][x] = x5;	
						B[j+5][x] = x6;	
						B[j+6][x] = x7;	
						B[j+7][x] = x8;
						continue;						
					}	
					for(y=j;y<j+8;y++){
						B[y][x]=A[x][y];
					}
				}
			}
		}

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

