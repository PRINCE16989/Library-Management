#ifndef PASSWORDCHECKER
#define PASSWORDCHECKER

#include <unordered_map>
#include <fstream>
#include <string>
using namespace std;
class PasswordChecker 
{
    private:
        static unordered_map<string,string> user_credentials;
        static unordered_map<string,string> admin_credentials;
    public:
        PasswordChecker() {
            ifstream file("passwords.txt");
            if (!file.is_open()) {
                cerr << "Failed to open password.txt\n";
            }
            string type, uid, pass;
            while(file >> type >> uid >> pass) {
                if(type == "student") user_credentials[uid] = pass;
                else if(type == "admin") admin_credentials[uid] = pass;
            }
            file.close();
        }

        ~PasswordChecker() {
            writeCredentialsToFile();
        }

        int validateUserLogin(const string& MemId, const string& Password) {
            auto it = user_credentials.find(MemId);
            if(it == user_credentials.end()) return -1;
            return user_credentials[MemId] == Password;
        }

        int validateAdminLogin(const string& AdminId, const string& Password) {
            auto it = admin_credentials.find(AdminId);
            if(it == admin_credentials.end()) return -1;
            return admin_credentials[AdminId] == Password;
        }

        int validateLogin(const string& type, const string& userId, const string& password) {
            if(type == "student" && user_credentials.find(userId) != user_credentials.end()) {
                return validateUserLogin(userId, password);
            } else if(type == "admin" && admin_credentials.find(userId) != admin_credentials.end()) {
                return validateAdminLogin(userId, password);
            }
            return -1; // User ID not found
        }
        
        bool changePassword(string userID) {
            bool changed = false;
            cout << "Enter old password (or type exit to cancel): ";
            string oldPassword;
            cin >> oldPassword;
            if(oldPassword == "exit") {
                cout << "Password change cancelled.\n" << endl;
                return false;
            }
            cout << "Enter new password: ";
            string newPassword;
            cin >> newPassword;
            cout << "Confirm new password: ";
            string confirmPassword;
            cin >> confirmPassword;
            
            auto it = user_credentials.find(userID);
            if(it->second != oldPassword) {
                cout << "Old password is incorrect.\n" << endl;
            }
            else if(newPassword != confirmPassword) {
                cout << "New passwords do not match.\n" << endl;
            }
            else if(newPassword == oldPassword) {
                cout << "New password cannot be the same as the old password.\n" << endl;
            }
            else changed = true;
            if(changed) {
                user_credentials[userID] = newPassword;
                cout << "Password changed successfully.\n" << endl;
            }
            return changed;
        }

        void writeCredentialsToFile() {
            ofstream file("passwords.txt");
            if (!file.is_open()) {
                cerr << "Failed to open passwords.txt for writing\n";
                return;
            }
            for (const auto& [uid, pass] : user_credentials) {
                file << "student " << uid << " " << pass << "\n";
            }
            for (const auto& [uid, pass] : admin_credentials) {
                file << "admin " << uid << " " << pass << "\n";
            }
            file.close();
        }

        friend class Library; // Allow Library class to access private members
};


#endif