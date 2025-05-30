#ifndef LIBRARY 
#define LIBRARY

#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <algorithm>
#include "Library.h"
#include "PasswordChecker.cpp"

Library::Library() {
    AllBooks.reserve(50);         //reserving space so that using push_back doesn't change the address of the elements
    AllMembers.reserve(150);

    ifstream file("books.txt");
    if (!file.is_open()) {
        cerr << "Failed to open password.txt\n";
    }
    book B;
    while(file >> B) {
        addBook(B);
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        string blankLine;
        getline(file, blankLine); // to ignore the blank line after each book entry
    }
    file.close();
    
    ifstream memberFile("members.txt");
    if (!memberFile.is_open()) {
        cerr << "Failed to open members.txt\n";
    }
    string line;
    while (getline(memberFile, line)) {
        // Read member info
        stringstream ss(line);
        string id, name; int limit;
        ss >> id >> name >> limit;
        member M(name, id, limit);

        // Read possible book line
        if (!getline(memberFile, line)) { registerMember(M); break; } // EOF after member — ok
        if (line.empty()) { registerMember(M); continue; } // no books

        stringstream booksStream(line);
        string isbn;
        int count;

        while (booksStream >> isbn >> count) {
            M.borrowedBooks[isbn] = count;
        }
        registerMember(M); // register member after reading all borrowed books
    }
}

Library::~Library() {
    writeBooksToFile();
    writeMembersToFile();
}

bool Library::addBook(book& A) {
    string tempISBN = A.getISBN();
    auto itr = Allbooks.find(tempISBN);
    
    if(itr != Allbooks.end()) {
        cout << "Invalid request! Book with same isbn already exists" << endl;
        return false;
    } 
    
    AllBooks.push_back(A);
    Allbooks[tempISBN] = &AllBooks.back();
    titleToIsbn[A.gettitle()] = tempISBN;
    return true;
}

void Library::addBook() {
    cout << "\nNEW BOOK ENTRY..." << endl;
    book B;
    cin >> B;
    if(addBook(B)) {
        cout << "Book added successfully!" << endl;
    }
    else {
        cout << "Failed to add book." << endl;
    }

    cout << "Do you want to add another book? (y/n): ";
    char choice;
    cin >> choice;
    if(choice == 'y' || choice == 'Y') {
        addBook();
        // cout << endl;
    }
    else return; // exit the function if user does not want to add another book
}

bool Library::registerMember(member& M) {
    string tempID = M.getMemberID();
    auto itr = Allmembers.find(tempID);
    
    if(itr != Allmembers.end()) {
        cout << "Invalid request! Member with same id already exists" << endl;
        return false;
    }
   
    AllMembers.push_back(M);
    Allmembers[tempID] = &AllMembers.back();
    
    return true;
}

void Library::registerMember() {
    cout << "\nNEW STUDENT RECORD..." << endl;

    member M;
    cin >> M;
    if(registerMember(M)) {
        PasswordChecker::user_credentials[M.memberID] = "default_password"; // Assign a default password for the new member
        cout << "Member registered successfully!" << endl;
    }
    else {
        cout << "Failed to register member." << endl;
    }

    cout << "Do you want to add another member? (y/n): ";
    char choice;
    cin >> choice;
    if(choice == 'y' || choice == 'Y') {
        registerMember();
    }
    else return; // exit the function if user does not want to add another book
    cout << endl;
}

bool Library::borrowBook(string memberID) {
    string isbn; bool borrowed = false; // flag to allow user to try borrowing another book
    cout << "\nEnter ISBN of the book to borrow (or 0 to exit): ";
    cin >> isbn;
    if(isbn == "0") return false;
    auto itr = Allbooks.find(isbn);
    auto mem = Allmembers.find(memberID);
    if(itr == Allbooks.end()) {
        cout << "\nInvalid request! Book not found\n" << endl;
    }
    else if(!Allbooks[isbn]->getCopiesAvailable())  {
        cout << "\nInvalid request! Copy of book not available\n" << endl;
    }
    else if(mem == Allmembers.end()) {
        cout << "\nInvalid request! No such member exists\n" << endl;
    }
    else if(Allmembers[memberID]->borrowBook(isbn)) { 
        Allbooks[isbn]->borrowBook();
        cout << "\nBook borrowed successfully!" << endl;
        borrowed = true; // set flag to true if book is successfully borrowed
    }
    else return false; // if borrowing fails cause you exceeded your borrow limit

    if(borrowed) cout << "Do you want to borrow another book? (y/n): ";
    else cout << "Do you want to try borrowing another book? (y/n): ";
        char choice;
        cin >> choice;
        if(choice == 'y' || choice == 'Y') {
            return borrowBook(memberID);
        }
    cout << endl;
    return borrowed;
}

bool Library::returnBook(string memberID) {
    string isbn;
    bool returned = false;
    cout << "\nEnter ISBN of the book to return: ";
    cin >> isbn;
    auto itr = Allbooks.find(isbn);
    auto mem = Allmembers.find(memberID);
    if(!mem->second->getBooksBorrowed()) {
        cout << "\nYou have not borrowed any books\n" << endl;
        return false;
    }
    if(itr == Allbooks.end()) {
        cout << "\nInvalid request! Book not found\n" << endl; 
    }
    else if(mem == Allmembers.end()) {
        cout << "\nInvalid request! Member is not registered\n" << endl;
    }
    else if(itr->second->getCopiesAvailable() == itr->second->getTotalCopies()) {
        cout << "\nInvalid request! You have not borrowed this book\n" << endl;
    }
    else if(Allmembers[memberID]->returnBook(isbn)) {
        Allbooks[isbn]->returnBook();   
        cout << "\nBook returned successfully!" << endl;
    }

    if(returned) cout << "Do you want to return another book? (y/n): ";
    else cout << "Do you want to try returning another book? (y/n): ";
        char choice;
        cin >> choice;
        if(choice == 'y' || choice == 'Y') {
            return returnBook(memberID);
        }
        cout << endl;
    return returned;
}

void Library::printLibrary() {
    printAllBooks();
    printAllMembers();
}

void Library::printAllMembers() {
    if(AllMembers.empty()) {
        cout << "\nNO MEMBERS REGISTERED IN LIBRARY!\n" << endl;
        return;
    }
    cout << "\n\t\t\tSTUDENT RECORDS" << endl;
    cout << string(60, '#') << endl;
    cout << left << setw(15) << "MemberID" << setw(30) << "Name" << setw(15) << "Books Issued" << endl;
    cout << string(60, '#') << endl;

    for(auto& x : AllMembers) {
        cout << left << setw(15) << x.getMemberID() << setw(30) << x.name << setw(15) << x.getBooksBorrowed() << endl;
    }
    cout << endl;
}

void Library::printSpecificMember() {
    cout << "\nEnter member ID to search: ";
    string ID;
    cin >> ID;
    member* toPrint = searchMember(ID);
    if(toPrint == NULL) {
        cout << "Invalid request! No such member is registered" << endl; 
        return;
    }
    toPrint->printDetails();
}

void Library::printAllBooks() {
    if(AllBooks.empty()) {
        cout << "\nNO BOOKS AVAILABLE IN LIBRARY!\n" << endl;
        return;
    }
    cout << "\n\t\t\tBOOK RECORDS" << endl;
    cout << string(84, '#') << endl;
    cout << left << setw(15) << "ISBN" << setw(30) << "Title" << setw(30) << "Author" << setw(9) << "Available" << endl;
    cout << string(84, '#') << endl;
    for(auto& x : AllBooks) {
        cout << left << setw(15) << x.getISBN() << setw(30) << x.title << setw(30) << x.author << setw(9) << x.getCopiesAvailable() << endl; 
    }
    cout << endl;
}

void Library::printSpecificBook() {
    book* toPrint = searchBookByIsbn();
    if(toPrint == NULL) {
        cout << "Invalid request! Book with given ISBN does not exist" << endl; 
        return;
    }
    toPrint->printDetails();
}

book* Library::searchBook(string title) {
    // cout << "inside searchBook" << endl;
    if(titleToIsbn.empty()) {
        cout << "No books available in the library" << endl;
        return NULL;
    }
    auto it = titleToIsbn.find(title);
    string isbn;
    if(it != titleToIsbn.end()) isbn = titleToIsbn[title];
    else return NULL;
    auto itr = Allbooks.find(isbn);
    if(itr == Allbooks.end()) return NULL;
    return Allbooks[isbn];  
}

void Library::searchBook() {
    string s;
    cout << "\nEnter book title to search: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, s);
    book* B = searchBook(s);
    if(B) B->printDetails();
    else cout << "The Book " << s << " not found!\n" << endl;
    cout << "Do you want to search another book? (y/n): ";
    char choice;
    cin >> choice;
    if(choice == 'y' || choice == 'Y') {
        searchBook();
    } else {
        cout << endl;
        return; // exit the function if user does not want to search another book
    }
}

book* Library::searchBookByIsbn() {
    string isbn;
    cin >> isbn;
    auto itr = Allbooks.find(isbn);
    if(itr == Allbooks.end()) return NULL;
    return Allbooks[isbn];
}

void Library::modifyBook() {
    cout << "\nEnter book ISBN to modify: ";
    book* toModify = searchBookByIsbn();
    if(toModify == NULL) {
        cout << "Invalid request! Book with given ISBN does not exist" << endl; 
        return;
    }
    titleToIsbn.erase(toModify->title);

    toModify->ModifyBook();
    titleToIsbn[toModify->title] = toModify->getISBN();
    cout << "Book modified successfully!" << endl;
}

void Library::deleteBook() {
    string title;
    cout << "Enter book title to delete: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, title);
    book* toDelete = searchBook(title);
    if(toDelete == NULL) {
        cout << "Invalid request! Book with given title does not exist" << endl; 
        return;
    }
    if (toDelete->getCopiesAvailable() != toDelete->getTotalCopies()) {
        cout << "Cannot delete this book. Not all copies are returned." << endl;
        return;
    }
    titleToIsbn.erase(toDelete->title);

    string isbn = toDelete->getISBN();
    Allbooks.erase(isbn);
    
    auto it = find(AllBooks.begin(), AllBooks.end(), *toDelete);
    if(it != AllBooks.end()) {
        AllBooks.erase(it);
        cout << "Book deleted successfully!" << endl;
    } else {
        cout << "Failed to delete book." << endl;
    }
}

void Library::deleteMember() {
    string ID;
    cout << "Enter member ID to delete: ";
    cin >> ID;
    auto it = searchMember(ID);
    if(it == NULL) {
        cout << "Invalid request! No such member is registered" << endl; 
        return;
    }

    if(it->getBooksBorrowed() > 0) {
        cout << "Cannot delete this member. They have borrowed books." << endl;
        return;
    }
    PasswordChecker::user_credentials.erase(ID); // remove member's credentials

    Allmembers.erase(ID);
    auto memIt = find(AllMembers.begin(), AllMembers.end(), *it);
    if(memIt != AllMembers.end()) {
        AllMembers.erase(memIt);
        cout << "Member deleted successfully!" << endl;
    }
}

member* Library::searchMember(string ID) {
    auto itr = Allmembers.find(ID);
    if(itr == Allmembers.end()) return NULL;
    return Allmembers[ID];
}

void Library::writeBooksToFile() {
    ofstream file("books.txt");
    if (!file.is_open()) {
        cerr << "Failed to open books.txt for writing\n";
        return;
    }
    for (auto& b : AllBooks) {
        file << b.title << "\n" << b.author << "\n" << b.getISBN() << " " 
                << b.getCopiesAvailable() << " " << b.getTotalCopies() << "\n\n";
    }
    file.close();
}

void Library::writeMembersToFile() {
    ofstream file("members.txt");
    if (!file.is_open()) {
        cerr << "Failed to open members.txt for writing\n";
        return;
    }
    for (auto& m : AllMembers) {
        file << m.getMemberID() << " " << m.name << " " << m.getBorrowLimit() << "\n";
        for(auto& book : m.borrowedBooks) {
            if(book.second > 0) { // only write borrowed books with count > 0
                file << book.first << " " << book.second << " ";
            }
        }
        file << endl;
    }
    file.close();
}

#endif