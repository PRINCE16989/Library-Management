#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <unordered_map>
#include <string>
#include "book.h"
#include "member.h"
class Library
{
    private:
        vector<book> AllBooks;
        vector<member> AllMembers;
        unordered_map<string,book*> Allbooks;
        unordered_map<string,member*> Allmembers;
        unordered_map<string,string> titleToIsbn;

        bool addBook(book& A); 
        book* searchBook(string title);
        book* searchBookByIsbn();

        bool registerMember(member& M);

        void printLibrary();
        void writeBooksToFile();
        void writeMembersToFile();
    public:
        Library();
        ~Library();

        void printAllMembers();
        void printAllBooks();
        void printSpecificBook();
        void printSpecificMember();

        void addBook();
        void searchBook();
        void modifyBook();
        void deleteBook();
        bool borrowBook(string memberID);
        bool returnBook(string memberID);

        member* searchMember(string ID);
        void deleteMember();
        void registerMember();
};

#endif