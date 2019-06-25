#pragma once
#ifndef PFMATRIX_H
#define PFMATRIX_H
#include"ConsoleManager.hpp"
#include<string>
#include<sstream>
#include"enums.hpp"

class PFMatrix {
public:
    static bool inbounds(const std::size_t& column, const std::size_t& row) {return column>=0 && column<10 && row>=0 && row<22;}
    static bool visible(const std::size_t& column, const std::size_t& row) {return column>=0 && column<10 && row>=2 && row<22;}
    nes_uchar& operator()(const std::size_t& column, const std::size_t& row){
        if (inbounds(column,row)) return matrix[column][row];
		std::stringstream error_stream;
		error_stream<< "Invalid matrix index: col=" << column << ", row=" << row << glb::newline;
		ConsoleManager::update_error(error_stream.str());
        return matrix[0][0];
    }
    nes_uchar operator()(const std::size_t& column, const std::size_t& row) const{
        if (inbounds(column,row)) return matrix[column][row];
		std::stringstream error_stream;
		error_stream << "Invalid matrix index: col=" << column << ", row=" << row << glb::newline;
		ConsoleManager::update_error(error_stream.str());
        return 0;
    }
	nes_uchar matrix[10][22] = { 0 };
};

#endif // PFMATRIX_H
