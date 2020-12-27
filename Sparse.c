
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"List.h"
#include"Matrix.h"


int main(int argc, char * argv[]) {

	FILE *in, *out;
	int nMatrix =0;
	int aMatrix =0;
	int bMatrix =0;
	int a, b;
	double c;
	int x, y;
	double z;

	// check command line for correct number of arguments
   if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   // open files for reading and writing 
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }
   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }


   fscanf(in, "%d %d %d", &nMatrix, &aMatrix, &bMatrix);

   Matrix A = newMatrix(nMatrix);
   Matrix B = newMatrix(nMatrix);

   for(int i=0; i<aMatrix; i++) {
   	fscanf(in, "%d %d %lf", &a, &b, &c);
   	changeEntry(A, a, b, c);
   } 

   for(int i=0; i<bMatrix; i++) {
   	fscanf(in, "%d %d %lf", &x, &y, &z);
   	changeEntry(B, x, y, z);
   } 


   fprintf(out, "A has %d non-zero entries:\n", aMatrix);
   printMatrix(out, A);
   fprintf(out, "\n");
   fprintf(out, "B has %d non-zero entries:\n", bMatrix);
   printMatrix(out, B);
   fprintf(out, "\n");




   Matrix C = scalarMult(1.5, A);
   Matrix D = sum(A,B);
   Matrix E = sum(A,A);
   Matrix F = diff(B,A);
   Matrix G = diff(A,A);
   Matrix H = transpose(A);
   Matrix I = product(A, B);
   Matrix J = product(B, B);

   fprintf(out, "1.5*(A = \n");
   printMatrix(out, C);
   fprintf(out, "\n");

   fprintf(out, "A+B =");
   printMatrix(out, D);
   fprintf(out, "\n");

   fprintf(out, "A+A =");
   printMatrix(out, E);
   fprintf(out, "\n");   

   fprintf(out, "B-A =");
   printMatrix(out, F);
   fprintf(out, "\n");

   fprintf(out, "A-A =");
   printMatrix(out, G);
   fprintf(out, "\n");


   fprintf(out, "Transpose(A) =");
   printMatrix(out, H);
   fprintf(out, "\n");


   fprintf(out, "A*B =");
   printMatrix(out, I);
   fprintf(out, "\n");

   fprintf(out, "B*B =");
   printMatrix(out, J);
   fprintf(out, "\n");


}





