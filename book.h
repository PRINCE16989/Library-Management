#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
using namespace std;

class book
{
    private:
        string isbn;
        int copiesAvailable;
        int totalCopies;
        vector<string> reservedUsers;
    public:
        string title;
        string author;

        //constructors
        book() {}
        book(string Ititle,string author,string isbn,int IcopiesAvailable,int ItotalCopies);
        book(book& reference,string new_isbn);        

        //getters
        string getISBN() { return isbn; } 
        int getCopiesAvailable() { return copiesAvailable; }
        int getTotalCopies() { return totalCopies; }
        string gettitle() { return title; }

        //utilities
        void ModifyBook();
        void updateCopies(int count);
        bool borrowBook();
        bool returnBook();
        void printDetails();    

        friend istream& operator>>(istream& s, book& A);
        friend bool operator==(const book&a,const book& b);
};

#endif