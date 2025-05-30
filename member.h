#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <unordered_map>
using namespace std;

class member
{
    private:
        string memberID;
        unordered_map<string,int> borrowedBooks;
        int borrowLimit;
        // int booksBorrowed = 0;
    public:
        string name;
        member() {}
        member(string ID,string Iname,int limit);

        //getters
        string getMemberID() { return memberID; }
        int getBorrowLimit() { return borrowLimit; }
        int getBooksBorrowed();

        //utilities
        bool borrowBook(string isbn);
        bool returnBook(string isbn);
        void printDetails();
        void printBorrowedBooks();

        friend istream& operator>>(istream& s, member& M);
        friend bool operator==(const member& a, const member& b);
        friend class Library; // Allow Library class to access private members
};

#endif