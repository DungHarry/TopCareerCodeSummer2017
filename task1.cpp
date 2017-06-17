/*
    Date created: June 18th, 2017
    Author: Dung Harry
    Compiler: GCC for C++ version 6.3.0
    Contest: Top Career Code Challenge Summer 2017

    Description: this is the solution for the first task in C++ programming language

    The task:
            A file contains a sequence of integers, stored one per line.
            Implement a class that facilitates iterator over these integers.
            A valid integer is a sequence of one or more digits(without leading zeros), optionally preceded by a plus
        or minus sign, representing a number within the range [-1,000,000,000...1,000,000,000].
            We allow spaces to appear in a line before and/or after a valid integer.
            Lines are separated with the line-feed character(ASCII code 10).
            There might be lines that do not represent valid integers, e.g. 2u1, 23.9, #12, 00, ++1, 2000000000.
            Such lines are considered to be comments and should be discarded.
            Define a class Solution with an input iterator(as defined by C++ 03 and 11) that interates over integers from an input stream compliant with the above format

    Compile and run by the following commands:
        g++ -std=c++0x -o task1 task1.cpp
        ./task1
*/

#include <iostream>
#include <iosfwd>
#include <string>
#include <cstdint>
#include <fstream>

using namespace std;

class iter {
    public:
        iter();
        iter(ifstream *pStream, int32_t iPosition);

        iter& operator++();
        bool operator!=(const iter &iter);

        int32_t operator*();
    private:
        bool isValidNumber(const char *pcNumber, int32_t iSize);

        ifstream *pStream;
        int32_t iPosition;
};

class Solution {
    public:
        Solution(ifstream *pStream);
        virtual ~Solution();

        iter begin();
        iter end();
    private:
        bool isValidNumber(const char *pcNumber, int32_t iSize);

        ifstream *pStream;
};

Solution::Solution(ifstream *pStream) :
    pStream (pStream)
{

}

Solution::~Solution() {

}

iter Solution::begin() {
    int32_t iCurrentPosition = this->pStream->tellg();
    char *pcBuffer = new char[1 << 15];
    int32_t i = 0;
    iter temp;

    *pcBuffer = '\0';

    while(this->isValidNumber(pcBuffer, i) == false) {
        iCurrentPosition = static_cast<int32_t>(this->pStream->tellg());

        for(i = 0; this->pStream->get(*(pcBuffer + i)) && *(pcBuffer + i) != '\n' && this->pStream->eof() == false && i < ((int32_t) (1 << 15)) - 1; i ++);
        *(pcBuffer + i) = '\0';
    }

    cout<<this->isValidNumber(pcBuffer, i)<<endl;

    delete[] pcBuffer;

    return (temp = iter(this->pStream, iCurrentPosition));
}

iter Solution::end() {
    int32_t iCurrentPosition;
    bool isSet = false;
    char *pcBuffer = new char[1 << 15];
    int32_t i = 0;
    iter *pResult;

    *pcBuffer = '\0';

    while(this->pStream->eof() == false) {
        for(i = 0; this->pStream->get(*(pcBuffer + i)) && *(pcBuffer + i) != '\n' && this->pStream->eof() == false && i < ((int32_t) (1 << 15)) - 1; i ++);
        *(pcBuffer + i) = '\0';

        if(this->isValidNumber(pcBuffer, i)) {
            if(!isSet)
                isSet = true;

            iCurrentPosition = static_cast<int32_t>(pStream->tellg()) - i - 1;
        }
    }

    delete[] pcBuffer;

    pResult = isSet ? new iter(this->pStream, iCurrentPosition) : new iter();

    return *pResult;
}

bool Solution::isValidNumber(const char *pcNumber, int32_t iSize) {
    int32_t i;

    if(pcNumber == NULL || iSize <= 0)
        return false;

    for(i = 0; *(pcNumber + i) == ' ' && i < iSize; i ++);

    if(i >= iSize)
        return false;

    if(*(pcNumber + i) == '+' || *(pcNumber + i) == '-')
        i++;

    if(iSize - i > 10)
        return false;

    for(; *(pcNumber + i) > '0' && *(pcNumber + i) < '9' && i < iSize; i ++);

    if(i < iSize)
        return false;

    return true;
}

iter::iter() :
    pStream (NULL),
    iPosition (-1)
{

}

iter::iter(ifstream *pStream, int32_t iPosition) :
    pStream (pStream),
    iPosition (iPosition)
{

}

iter& iter::operator++() {
    char *pcBuffer;
    int32_t iTimes = 0;
    int32_t i;

    this->pStream->clear();
    this->pStream->seekg(this->iPosition, this->pStream->beg);

    pcBuffer = new char[1 << 15];

    *pcBuffer = '\0';
    i = 0;

    while((this->isValidNumber(pcBuffer, i) == false || iTimes < 2) && this->pStream->eof() == false) {
        for(i = 0; this->pStream->get(*(pcBuffer + i)) && *(pcBuffer + i) != '\n' && this->pStream->eof() == false && i < ((int32_t) (1 << 15)) - 1; i ++);
        *(pcBuffer + i) = '\0';

        if(this->isValidNumber(pcBuffer, i) == true)
            iTimes ++;
    }

    if(iTimes >= 2)
        this->iPosition = static_cast<int32_t>(this->pStream->tellg()) - i - 1;

    delete[] pcBuffer;

    return *this;
}

bool iter::operator!=(const iter &iter) {
    return iPosition != iter.iPosition ? true : false;
}

int32_t iter::operator*() {
    char *pcBuffer;
    int32_t i;

    if(this->pStream == NULL || this->iPosition < 0)
        return -1;

    pcBuffer = new char[1 << 15];

    this->pStream->clear();
    this->pStream->seekg(this->iPosition, this->pStream->beg);

    for(i = 0; this->pStream->get(*(pcBuffer + i)) && *(pcBuffer + i) != '\n' && this->pStream->eof() == false && i < ((int32_t) (1 << 15)) - 1; i ++);
    *(pcBuffer + i) = '\0';

    i = atoi(pcBuffer);

    delete[] pcBuffer;

    return i;
}

bool iter::isValidNumber(const char *pcNumber, int32_t iSize) {
    if(pcNumber == NULL || iSize <= 0)
        return false;

    int32_t i;

    if(pcNumber == NULL || iSize <= 0)
        return false;

    for(i = 0; *(pcNumber + i) == ' ' && i < iSize; i ++);

    if(i >= iSize)
        return false;

    if(*(pcNumber + i) == '+' || *(pcNumber + i) == '-')
        i++;

    if(iSize - i > 10)
        return false;

    for(; *(pcNumber + i) > '0' && *(pcNumber + i) < '9' && i < iSize; i ++);

    if(i < iSize)
        return false;

    return true;
}

int32_t main(int32_t argc, char **argv) {
    ifstream *pFile = new ifstream();
    char c;
    int32_t iNumber;
    iter endIter, i;

    pFile->open("numbers.txt");

    if(pFile->is_open() == false)
        return -1;

    Solution *pSolution = new Solution(pFile);

    cout<<"Author: Dung Harry"<<endl<<endl;

    cout<<"The content of file: "<<endl<<endl;

    while(pFile->get(c))
        cout<<c;

    cout<<endl<<endl;

    pFile->clear();
    pFile->seekg(0, pFile->beg);

    for(i = pSolution->begin(), endIter = pSolution->end(); i != endIter; ++ i) {
        iNumber = *i;

        cout<<"Number: "<<iNumber<<endl;
    }

    iNumber = *i;

    cout<<"Number: "<<iNumber<<endl;

    pFile->close();

    delete pFile;
    delete pSolution;

    return 0;
}
