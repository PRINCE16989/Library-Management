#include "Messenger.cpp"
#include "Library.h"
#include "PasswordChecker.cpp"
// Static member definitions — needed exactly once
unordered_map<std::string, std::string> PasswordChecker::user_credentials;
unordered_map<std::string, std::string> PasswordChecker::admin_credentials;

int main()
{
    Messenger M;
    PasswordChecker P;
    // P.print_details();
    Library L;
    // L.printLibrary();

    while(true) {
        int AorS = M.displayMainMenu();
        if(AorS == 1) {
            while(true) {
                auto [userID, password] = M.displayLoginInterface();
                if(userID == "") break;
                int validation = P.validateLogin("admin", userID, password);
                if(validation == -1) M.displayInvalidUserID();
                else if(validation == 0) M.displayIncorrectPassword();
                else {
                    // use switch case for better readability
                    bool AdminLoggedIN = true;
                    while(AdminLoggedIN) {
                        int choice = M.displayAdministratorMenu();
                        // administrator functions
                        switch (choice)
                        {
                            case 1: 
                                L.printAllMembers();
                                break;
                            case 2: 
                                L.printSpecificMember();
                                break;
                            case 3: 
                                L.printAllBooks();
                                break;
                            case 4: {
                                L.searchBook();
                                break;
                            }
                            case 5: {
                                L.addBook();
                                break;
                            }
                            case 6: {
                                L.modifyBook();
                                break;
                                // code for modifying book
                            }
                            case 7: {
                                L.deleteBook();
                                break;
                                // code for deleting book
                            }
                            case 8: {
                                L.deleteMember();
                                break;
                                // code for deleting student record
                            }
                            case 9: {
                                L.registerMember();
                                break;
                                // code for creating student record
                            }
                            case 10: { AdminLoggedIN = false; break; }// logout
                            default : {
                                cout << "Invalid choice! Please try again." << endl;
                                continue; // prompt for choice again
                            }
                        }
                    }
                    break; // exit the loop if user chooses to logout
                }
            }
        }
        else if(AorS == 2) {
            while(true) {
                auto [userID, password] = M.displayLoginInterface();
                if(userID == "") break;

                int validation = P.validateLogin("student", userID, password);
                
                if(validation == -1) M.displayInvalidUserID();
                else if(validation == 0) M.displayIncorrectPassword();
                else {
                    bool StudentLoggedIN = true;
                    while(StudentLoggedIN) {
                        int choice = M.displayStudentMenu(userID);
                        if(choice == 1) {
                            // code for searching book
                            L.searchBook();
                        }
                        else if(choice == 2) {
                            L.borrowBook(userID);
                        } 
                        else if(choice == 3) {
                            L.returnBook(userID);
                        }
                        else if(choice == 4) {
                            member* mem = L.searchMember(userID);
                            if(mem) mem->printBorrowedBooks();
                        }
                        else if(choice == 5) {
                            P.changePassword(userID);
                        }
                        else StudentLoggedIN = false; // logout
                    }
                    break; // exit the loop if user chooses to logout
                }
            }
        }
        else break;
        cout << endl;
    }
}
