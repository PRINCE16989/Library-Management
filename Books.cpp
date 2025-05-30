#ifndef BOOKS
#define BOOKS

#include <iostream>
#include <limits>
#include <iomanip>

#include "book.h"

void book::updateCopies(int count) {
    if(count + copiesAvailable >= 0) {
        copiesAvailable += count;
        totalCopies += count;
    }
    else cout << "Invalid request! Count becomes negative" << endl; 
}

bool book::borrowBook() {
    if(copiesAvailable == 0) {
        cout << "Invalid request! Copy of book not available" << endl;
        return false;
    }
    else {
        --copiesAvailable;  
        return true;
    }
}

bool book::returnBook() {
    if(copiesAvailable == totalCopies) {
        cout << "Invalid request! You donot contain this book" << endl; 
        return false;
    }
    else {
        ++copiesAvailable;  
        return true;
    }
}

void book::ModifyBook() {
    printDetails();
    cout << "\n\t\tEnter new details for the book:\n" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline
    cout << "Enter new title: ";
    getline(cin, title);
    cout << "\nEnter new author: ";
    getline(cin, author);

}

void book::printDetails() {
    cout << left << setw(20) << "\nBOOK DETAILS :" << endl;
    cout << left << setw(20) << "\nBook Isbn " << ": " << isbn;
    cout << left << setw(20) << "\nBook Title " << ": " << title;
    cout << left << setw(20) <<  "\nBook Author " << ": " << author;
    cout << left << setw(20) << "\nCopies Available " << ": " << copiesAvailable << "\n" << endl;
}

istream& operator>>(istream& s, book& A) {
    if(&s == &std::cin) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline
        cout << "\nEnter The Name Of The Book: ";
        getline(cin, A.title);
        cout << "\nEnter The Author's Name: ";
        getline(cin, A.author);
        cout << "\nEnter valid ISBN: ";
        cin >> A.isbn;
        cout << "\nTotal Copies: ";
        while(true) {
            if(cin >> A.totalCopies && A.totalCopies > 0) break; // ensure totalCopies is a positive integer
            cout << "Invalid input! Please enter a positive integer for total copies: ";
            cin.clear(); // clear the error flag
            // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
        }
        A.copiesAvailable = A.totalCopies; // initially all copies are available
        return s;
    }

    getline(s,A.title);
    getline(s,A.author);
    s >> A.isbn >> A.copiesAvailable >> A.totalCopies;
    return s;
}

bool operator==(const book&a,const book& b) {
    return (a.isbn == b.isbn);
}

#endif