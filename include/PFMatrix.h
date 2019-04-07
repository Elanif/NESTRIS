#pragma once
#ifndef PFMATRIX_H
#define PFMATRIX_H
#include<cstdio>

class PFMatrix {
public:
    PFMatrix() {
        for (size_t i=0; i<10; ++i)
            for (size_t j=0; j<22; ++j)
                matrix[i][j]=0;
    }
    static bool inbounds(const size_t& column, const size_t& row) {return column>=0 && column<10 && row>=0 && row<=22;}
    nes_uchar& operator()(const size_t& column, const size_t& row){
        if (inbounds(column,row)) return matrix[column][row];
        printf("ERROR: ACCESSING INVALID MATRIX INDEX: col= %d, row= %d\n", column, row);
        return matrix[0][0];
    }
    nes_uchar operator()(const size_t& column, const size_t& row) const{
        if (inbounds(column,row)) return matrix[column][row];
        printf("ERROR: ACCESSING INVALID MATRIX INDEX: col= %d, row= %d\n", column, row);
        return 0;
    }
    nes_uchar matrix[10][22];
};

#endif // PFMATRIX_H
