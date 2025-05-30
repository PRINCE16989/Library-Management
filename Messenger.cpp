#ifndef MESSENGER
#define MESSENGER

#include <iostream>
#include <string>
using namespace std;

class Messenger {
    public:
        Messenger() {}

        int displayMainMenu() {
            cout << "MAIN MENU\n" << endl;
            cout << "1. ADMINISTRATOR LOGIN\n" << endl;
            cout << "2. STUDENT LOGIN\n" << endl;
            cout << "Please Select Your Option(1-2) : ";
            int AorS;
            cin >> AorS;
            return AorS;
        }
        
        pair<string,string> displayLoginInterface() {
            string userId, password;
            cout << "\n\t\tLOGIN INTERFACE" << endl;
            cout << "\nEnter User Id(or type '0' to return to main menu): ";
            cin >> userId;
            if(userId == "0") return {"",""};
            cout << "Enter Your Password: ";
            cin >> password;
            return {userId, password};
        }

        void displayInvalidUserID() {
            cout << "\nInvalid UserID" << endl;
            cout << "Please register yourself" << endl;
        }

        void displayIncorrectPassword() {
            cout << "Incorret Password. Login again with correct credentials" << endl;
        }

        int displayAdministratorMenu() {
            cout << "\n\t\tADMINISTRATOR MENU" << endl;
            cout << "1. DISPLAY ALL STUDENT RECORD" << endl; 
            cout << "2. DISPLAY SPECIFIC STUDENT RECORD" << endl;
            cout << "3. DISPLAY ALL BOOKS" << endl;
            cout << "4. DISPLAY SPECIFIC BOOK" << endl;
            cout << "5. ADD BOOK" << endl;
            cout << "6. MODIFY BOOK" << endl;
            cout << "7. DELETE BOOK" << endl;
            cout << "8. DELETE STUDENT RECORD" << endl;
            cout << "9. CREATE STUDENT RECORD" << endl;
            cout << "10. LOGOUT" << endl;
            cout << "Enter Your Choice(1-10) : ";
            int choice;
            cin >> choice;
            return choice;
        }

        int displayStudentMenu(const string& UserID) {
            cout << "User: " << UserID << endl;
            cout << "1. Search Book" << endl;
            cout << "2. Borrow Book" << endl;
            cout << "3. Return Book" << endl;
            cout << "4. View Borrowed Books" << endl;
            cout << "5. Change Password" << endl;
            cout << "6. Logout" << endl;
            cout << "Enter Your Choice(1-6) : ";
            int choice; cin >> choice;
            return choice;
        }

        void displayBookNotFound() {
            cout << "Book Not Found" << endl;
        }
};

#endif // MESSENGER