#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <conio.h>
#include <limits>
#include "../validasi.h"
#include "../database_handler.h"
#include "user.h"

using namespace std;

void clearScreenUser() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreenUser() {
    cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
    if (cin.peek() == '\n') cin.ignore();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string inputPassword() {
    string password = "";
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 0 || (unsigned char)ch == 224) { _getch(); continue; }
        if (ch == '\r') { cout << endl; break; }
        else if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password += ch;
            cout << '*';
        }
    }
    return password;
}

int inquirerMenuUser(string title, vector<string> options) {
    int selected = 0;
    int total    = (int)options.size();

    while (true) {
        clearScreenUser();

        cout << "\n\033[1;35m============================================\033[0m\n";
        cout << "\033[1;37m  " << title << "\033[0m\n";
        cout << "\033[1;35m============================================\033[0m\n\n";

        for (int i = 0; i < total; i++) {
            if (i == selected)
                cout << "  \033[1;32m> " << options[i] << "\033[0m\n";
            else
                cout << "    " << options[i] << "\n";
        }

        cout << "\n\033[1;33m  [Atas/Bawah] Navigasi  |  [Enter] Pilih\033[0m\n";

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + total) % total;
            if (key == 80) selected = (selected + 1) % total;
        } else if (key == '\r') {
            return selected;
        } else if (key == 'w' || key == 'W') {
            selected = (selected - 1 + total) % total;
        } else if (key == 's' || key == 'S') {
            selected = (selected + 1) % total;
        }
    }
}

string getValidStringUser() {
    string input;
    getline(cin, input);
    size_t start = input.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = input.find_last_not_of(" \t\r\n");
    return input.substr(start, end - start + 1);
}

long long getValidNumberUser() {
    string input;
    while (true) {
        getline(cin, input);
        bool valid = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) { valid = false; break; }
        }
        if (valid) return stoll(input);
        cout << "\033[1;31m[!] Masukkan angka yang valid: \033[0m";
    }
}