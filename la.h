#ifndef _LA_
#define _LA_

typedef struct{
    int** data;
    int m;
    int n;
} Matrix;

typedef struct{
    int* data;
    int n;
}Vector;

// m rows and n columns
Matrix* create_matrix(int m, int n);
Vector* create_vector(int n);

void free_matrix(Matrix* matrix);
void free_vector(Vector* vector);

void print_matrix(Matrix* matrix);
void print_vector(Vector* vector);

Vector* prod_matrix_vector(Matrix* mat, Vector* vec);
Matrix* prod_matrix(Matrix* A, Matrix* B);

void mul_vector_scalar(Vector* vec, int scalar);
Vector* apply_vector(Vector* vec, int(*f)(int));
Matrix* apply_matrix(Matrix* vec, int(*f)(int));

Matrix* populate_matrix_random(Matrix* mat);

#endif