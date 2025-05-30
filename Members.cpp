#ifndef MEMBERS_CPP
#define MEMBERS_CPP

#include <iostream>
#include <iomanip>
#include <limits>
#include "member.h"

member::member(string Iname, string ID, int limit = 3) {
    name = Iname;
    memberID = ID;
    borrowLimit = limit;                     //Borrow Limit is the number of additional books which can be borrowed
}

int member::getBooksBorrowed() { 
    int count = 0;
    for(auto& x : borrowedBooks) {
        count += x.second; // sum up the number of copies borrowed for each book
    }
    return count; 
}

bool member::borrowBook(string isbn) {
    if(borrowLimit == 0) {                      //BorrowLimit is the number of addional books which can be borrowed 
        cout << "Invalid request! Borrow limit exceeded. You cannot borrow more books.\n" << endl;
        return false;
    }
    else  {
        borrowedBooks[isbn]++;
            --borrowLimit;                      //keeping track of borrowed books by increasing and deccreasing borrowed limit
            return true;
    }
}

void member::printBorrowedBooks() {
    if(borrowedBooks.empty()) {
        cout << "\n" << name << " has not borrowed any books\n" << endl;
        return;
    }
    // cout << "\nBorrowed books by " << name << ":" << endl;
    // cout << left << setw(10) << "ISBN" << setw(5) << "Count" << endl;
    cout << left << setw(30) << "\nISBN of Book Borrowed" << setw(15) << "No. of Copies" << endl;
    //cout << string(15, '-') << endl;
    for(auto& x : borrowedBooks) {
        if(x.second > 0) {
            cout << left << setw(30) << x.first << setw(15) << x.second << endl;
        }
    }
    cout << endl;
}

bool member::returnBook(string isbn) {
    auto it = borrowedBooks.find(isbn);
    if(it == borrowedBooks.end() || it->second == 0) {          //books which are borrowed and returned still exist in map 
        cout << "Invalid request! Book not borrowed" << endl;   //but the number of copies borrowed will be zero 
        return false;                                           //should check the number of borrowed copies if book exists in map
    }
    else {
        --it->second;                           //Decreases the number of copies borrowed until it is zero
        ++borrowLimit;
        return true;  
    }
}

void member::printDetails() {
    cout << "\nSTUDENT DETAILS :" << endl;
    cout << left << setw(17) << "\nAdmission no. : " << memberID;
    cout << left << setw(17) << "\nStudent Name  : " << name << endl; 
    printBorrowedBooks();
}

istream& operator>>(istream& s, member& M) {
    if(&s == &std::cin) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline
        cout << "\nEnter The Name Of The Student: " << endl;
        getline(cin, M.name);
        cout << "\nEnter The Admission no. : " << endl;
        cin >> M.memberID;
        cout << "\nEnter Borrow Limit : " << endl;
        while(true) {
            if(cin >> M.borrowLimit && M.borrowLimit > 0) break; // ensure limit is a positive integer
            cout << "Invalid input! Please enter a positive integer for borrow limit: ";
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
        }
        return s;
    }
    //s.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline
    string ID, Iname; int limit;
    s >> ID >> Iname >> limit;
    M.memberID = ID;
    M.name = Iname;
    M.borrowLimit = limit;
    return s;
}

bool operator==(const member& a, const member& b) {
    return (a.memberID == b.memberID);
}

#endif
