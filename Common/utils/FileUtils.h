/*
 * Author : fieryzig
 * Create Time : Thu 05 Jan 2017 08:27:43 PM PST
 * File Name : FileUtils.h
 */

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <fstream>

using namespace std;

namespace fieryzig{

string GetStringFromFile(const char* filename) {
    string ret = "";
    string line;
    ifstream file(filename,ios::in);
    if (!file.is_open()) {
        return ret;
    }
    while (!file.eof()) {
        getline(file,line);
        ret.append(line + '\n');
    }
    return ret;
}

} // end of namespace

#endif
