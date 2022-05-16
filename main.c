#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "la.h"

#define NIBBLE_SIZE 4
#define BYTE_SIZE 8

Matrix* create_G_matrix(int size_data, int size_par){
    Matrix* g_mat = create_matrix(size_data+size_par, size_data);
    for (int i = 0; i < size_data; i++){
        for (int j = 0; j < size_data; j++){
            g_mat->data[j][j] = 1;
        }    
    }

    for (int i = size_data; i < size_data+size_par; i++){
        for (int j = 0; j < size_data; j++){
            g_mat->data[i][j] = ((((i-size_data)<j) | ((i-size_data)>j)))%2? 1 : 0;
        }
    }
    return g_mat;
}

Matrix* create_H_matrix(Matrix* G, int size_data, int size_par){
    Matrix* h_mat = create_matrix(size_par, size_data+size_par);
    for (int i = 0; i < size_par; i++){
        for (int j = 0; j < size_par; j++){
            h_mat->data[i][j] = G->data[i+size_data][j];
        }
    }

    for (int i = 0; i < size_par; i++){
        for (int j = size_data; j < size_data+size_par; j++){
            h_mat->data[j-size_data][j] = 1;
        }
    }
    return h_mat;
}

static inline int mod2(int n){
    return n%2;
}

Vector* encode(Matrix* G, Vector* vec){
    return apply_vector(prod_matrix_vector(G, vec), mod2);;
}

Vector* check_parity(Matrix* H, Vector* encoded_byte){
    return apply_vector(prod_matrix_vector(H, encoded_byte), mod2);
}

void correct_byte(Vector* parity_vec, Matrix* H, uint8_t* byte){
    
    for (int i = 0; i < H->n; i++){
        int match = 1;
        for (int j = 0; j < H->m; j++){    
            if (H->data[j][i] != parity_vec->data[j]){
                match = 0;
            }
        }
        if (match){
            *byte ^= (1 << (BYTE_SIZE-1 - i));
            return;
        }
    }

}

uint8_t vec_to_byte(Vector* vec){
    uint8_t res = 0;
    for (int i = 0; i < BYTE_SIZE; i++) res |= vec->data[i] << (BYTE_SIZE-1-i);
    // printf("%x\n", res);
    return res;
}

Vector* byte_to_vec(uint8_t byte){
    Vector* res = create_vector(BYTE_SIZE);
    for (int i = 0; i < BYTE_SIZE; i++) res->data[i] = (byte << i) & 0x80 ? 1 : 0;
    return res;
}


uint8_t convert(char* arg){
    uint8_t total = 0;
    for (int i = 0; arg[i] != '\0'; i++){
        if (arg[i] < '0' || arg[i] > '1' || i > NIBBLE_SIZE-1){
            fprintf(stderr, "ERROR: supply a nibble as a binary number of 4 bits\n");
            exit(1);
        }
        total = total * 2 + (arg[i] - '0');
    }
    return total;
}

int main(int argc, char** argv){

    if (argc != 2){
        fprintf(stderr, "ERROR: supply a nibble as a binary number of 4 bits\n");
        exit(1);
    }

    uint8_t input_nibble = convert(argv[1]);

    int d_size = NIBBLE_SIZE;
    int p_size = NIBBLE_SIZE;

    Vector* vec_nibble = create_vector(d_size);
    for (int i = 0; i < vec_nibble->n; i++) vec_nibble->data[i] = (input_nibble << i) & BYTE_SIZE ? 1 : 0;
    printf("input nibble as vector:\n---------------\n");
    print_vector(vec_nibble);

    Matrix* G = create_G_matrix(d_size, p_size);
    Matrix* H = create_H_matrix(G, d_size, p_size);
    printf("Matrix G:\n---------------\n");
    print_matrix(G);
    printf("Matrix H:\n---------------\n");
    print_matrix(H);

    Vector* encoded_vec = encode(G, vec_nibble);
    uint8_t encoded_byte = vec_to_byte(encoded_vec);
    printf("Encoded byte: 0x%x\n", encoded_byte);
    
    encoded_byte ^= 4;
    printf("Altered byte by making last bit 1: 0x%x\n\n", encoded_byte);

    Vector* parity_check = check_parity(H, byte_to_vec(encoded_byte));
    printf("Parity vector\nWhen vector == 0, there was no change\nWhen vector != 0, the index of the columns of H-matrix determine where the error is:\n---------------\n");
    print_vector(parity_check);

    correct_byte(parity_check, H, &encoded_byte);
    printf("Corrected byte: 0x%x\n", encoded_byte);

    free_matrix(G);
    free_matrix(H);
    free_vector(encoded_vec);
    free_vector(vec_nibble);
    free_vector(parity_check);
    
    return 0;
}
