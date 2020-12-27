
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "Matrix.h"
#include "List.h"

typedef struct EntryObj{
	int column;
	double value;
}EntryObj;

typedef struct EntryObj* Entry;

typedef struct MatrixObj {
	List* row;
	int size;
	int NNZ;
} MatrixObj;

typedef struct MatrixObj* Matrix;


Entry newEntry(int column, double value) {
	Entry E  = NULL;
	E = malloc(sizeof(EntryObj));
	E->column = column;
	E->value = value;
	return (E);
}

void freeEntry(Entry* pE) {
	if(pE!= NULL && *pE!=NULL) {
		free (*pE);
		*pE = NULL;
	}
}


// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n) {
	Matrix M = malloc(sizeof(MatrixObj));
	M->row = malloc(sizeof(List)*n);
	for(int i =0; i < n; i++) {
		M->row[i] = newList();
	}

	M->size  = n;
	M->NNZ = 0;
	return (M);



}

// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix *pM){
	if(pM!=NULL && *pM!= NULL) {
		makeZero(*pM);
		for(int i =0; i<size(*pM); i++) {
			freeList(&((*pM) -> row[i]));
		}
		free((*pM)->row);
		(*pM)->row = NULL;
		(*pM)->size =0;
	}

	free(*pM);
	*pM = NULL;
}

int size(Matrix M){
	if(M == NULL) {
		printf("Matrix Error: calling size() on a NULL list reference \n");
		exit(1);
	}
	return M->size;
}

int NNZ(Matrix M) {
	if(M == NULL) {
		printf("Matrix Error: calling NNZ() on a NULL list reference \n");
		exit(1);
	}
	return M->NNZ;
}

int equals(Matrix A, Matrix B) {
	if(A == NULL || B == NULL) {
		printf("List Error: calling equals() on NULL List reference\n");
		exit(1);
	}
	else if(A->size != B->size) {
		exit(1);
	}
	

}


// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix.
void makeZero(Matrix M){
	if(M == NULL) {
		printf("Matrix Error: calling NNZ() on a NULL list reference \n");
		exit(1);
	} else {
		List L;
		Entry e  = NULL;
		for(int i = 0; i < size(M); i++) {
			L = M->row[i];
			if(length(L) != 0) {
				moveFront(L);
				while(index(L) != -1){
					e = get(L);
					moveNext(L);
					freeEntry(&e);
				}
				clear(M->row[i]);
			}
		}
	}
	M->NNZ = 0;

}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
	if(M == NULL) {
		printf("Matrix Error: calling changeEntry() on a NULL list reference \n");
		exit(1);
	}
	else if(i > size(M)){
		printf("Matrix Error: calling changeEntry() on a NULL list reference \n");
		exit(1);
	} 
	else if (j > size(M)) {
		printf("Matrix Error: calling changeEntry() on a NULL list reference \n");
		exit(1);
	}
	else {
		Entry ent = newEntry(j,x);
		int con =0;
		List L = M->row[i-1];

		if(length(L) == 0 && x!= 0){
			append(L, ent);
			M->NNZ++;
			return;
		}
		else if (length(L) > 0){
			moveFront(L);
		}
		while(index(L) >= 0) {
			Entry new = (Entry)get(L);
			if(new->column == j){
				con =1;
				break;
			}
			moveNext(L);
		}

		if(con == 0 && x != 0){
			moveFront(L);
			Entry cur = (Entry)get(L);
			while(1){
				if( cur->column < j && get(L) == back(L)){
					append(L,ent);
					M->NNZ++;
					break;
				}
				else if( cur->column < j && get(L) == back(L)){
					moveNext(L);
					ent = (Entry)get(L);
					break;
				}
				else{
					insertBefore(L, ent);
					M->NNZ++;
					break;
				}
			}
			return;

		}

		if(con != 0 && x==0) {
			Entry cur = (Entry)get(L);
			delete(L);
			M->NNZ--;
			return;
		}

		if(con != 0 && x != 0) {
			Entry cur = (Entry)get(L);
			delete(L);
			M->NNZ--;
			return;
		}
	}

}


// Matrix Arithmetic operations
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
	if(A == NULL) {
		printf("Matrix Error: calling copy() on a NULL list reference \n");
		exit(1);
	}
	Matrix M = newMatrix(size(A));
	List l1, l2;
	int x;
	double y;
	for(int i =0; i < size(M); i++) {
		l1 = A->row[i];
		l2 = M->row[i];
		if(length(l1) != 0){
			moveFront(l1);
			while(index(l1) != -1){
				Entry ent = (Entry)get(l1);
				x = ent->column;
				y = ent->value;
				append(l2, newEntry(x,y));
				M->NNZ++;
				moveNext(l1);
			}
		}
	}
	return M;
}





Matrix transpose(Matrix A) {
	if(A == NULL) {
		printf("Matrix Error: calling transpose() on a NULL list reference \n");
		exit(1);
	}
	else {
		Matrix tran = newMatrix(size(A));
		for(int i=0; i < size(tran); i++) {
			moveFront(tran->row[i]);
			while(index(tran->row[i]) > -1) {
				changeEntry(tran, ((Entry)get(tran->row[i]))->column   , i, ((Entry)get(tran->row[i]))->value);
				moveNext(tran->row[i]);
			}
		}
		tran->NNZ = A->NNZ;
		return tran;
	}
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
	if(A == NULL) {
		printf("Matrix Error: calling scalarMult() on a NULL list reference \n");
		exit(1);
	}
	else {

		Matrix M = copy(A);
		for(int i =0; i < size(M); i++) {
			moveFront(M->row[i]);
			while(index(M->row[i]) != -1) {
				Entry ent = (Entry)get(M->row[i]);
				changeEntry(M, ent->column, ent->value, ent->value *x);
				moveNext(M->row[i]);
			}
		}
		return M;
	}
}



//helper function for add and sum
List math(List A, List B, bool pos) {
	List L = malloc(sizeof(List));
	moveFront(A);
	moveFront(B);
	double condition;
	if(pos) {
		condition = 1.0;
	}
	else{
		condition = -1.0;
	}
	while(index(A) > -1 || index(B) > -1) {
		if(index(A) > -1 && index(B) > -1) {
			Entry e = (Entry)get(A);
			Entry f = (Entry)get(B);
			if(e->column > f->column) {
				append(L, newEntry(f->column, (condition * f->value)) );
				moveNext(B);
			}
			else if(e->column < f->column) {
				append(L, newEntry(e->column, (condition * e->value)) );
				moveNext(A);
			}
			else if(e->column == f->column){
				if(((pos) && (e->value + f->value)!= 0) || (!pos && (e->value - f->value)!=0)    ) {
					if(pos) {
						condition = ((e->value) + (f->value));
					}
					else{
						condition = ((e->value) - (f->value));
					}
				}
				append(L, newEntry(e->column, condition));
				moveNext(A);
				moveNext(B);
			}
			else if(index(A) > (-1) ) {
				Entry x = (Entry)get(A);
				append(L, newEntry(x->column, x->value));
				moveNext(A);
			}
			else{
				Entry y = (Entry)get(B);
				append(L, newEntry(y->column, y->value * (condition)));
				moveNext(B);
			}
	}



}

return L;
}





// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
	if(A == NULL || B == NULL) {
		printf("Matrix Error: calling sum() on a NULL matrix reference \n");
		exit(1);
	}
	if(size(A) != size(B)) {
		printf("Matrix Error: calling sum() on unequal Matrices \n");
		exit(1);
	}
	Matrix M = newMatrix(size(A));
	for(int i=0; i< size(M); i++) {
		M->row[i] = math(A->row[i], B->row[i], true);
	}

	return M;

}

// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
	if(A == NULL || B == NULL) {
		printf("Matrix Error: calling diff() on a NULL matrix reference \n");
		exit(1);
	}
	if(size(A) != size(B)) {
		printf("Matrix Error: calling diff() on unequal Matrices \n");
		exit(1);
	}
	Matrix M = newMatrix(size(A));
	for(int i=0; i< size(M); i++) {
		M->row[i] = math(A->row[i], B->row[i], false);
	}

	return M;


}




double vectorDot(List A, List B){
	double prod = 0.0;
	moveFront(A);
	moveFront(B);
	while(index(A)>-1 && index(B) > -1 ) {
		Entry a = (Entry)get(A);
		Entry b = (Entry)get(B);
		if(a->column > b->column) {
			moveNext(B);
		}
		else if(a->column < b->column) {
			moveNext(A);
		}
		else{
			prod += ((a->value)*(b->value));
			moveNext(A);
			moveNext(B);
		}

	}

	return prod;
}





// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
	if(A == NULL || B == NULL) {
		printf("Matrix Error: calling product() on a NULL list reference \n");
		exit(1);
	}
	if(size(A) != size(B)) {
		printf("Matrix Error: calling product() on unequal Matrices \n");
		exit(1);
	}
	else{
		Matrix M = newMatrix(size(A));
		Matrix N = transpose(M);
		for(int i=0; i < size(A); i++) {
			if(length(A->row[i]) != 0) {
				for(int j=0; j<size(A); j++) {
					changeEntry(M, i+1, j+1, vectorDot(A->row[i], N->row[j]) );
				}
			}
		}
		return M;

	}
}


// printMatrix()
// Prints a string representation of Matrix M to filestream out. 
void printMatrix(FILE* out, Matrix M){
		for(int i=0; i<size(M); i++) {
			moveFront(M->row[i]);
			fprintf(out, "%d", i+1);
			while(index(M->row[i]) > -1){
				Entry ent = (Entry)get(M->row[i]);
				if(index(M->row[i]) == length(M->row[i])-1) {
					fprintf(out, "%d, %0.1f\n", ent->column, ent->value);
					break;
				}
				fprintf(out, "(%d, %0.1f)",ent->column, ent->value);
				moveNext(M->row[i]);

			}
		}


}

