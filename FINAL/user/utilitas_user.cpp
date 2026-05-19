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

// bersihin layar terminal biar selalu keliatan fresh pas pindah menu
void clearScreenUser() {
<<<<<<< HEAD
    std::cout << "\033[2J\033[H" << std::flush;
=======
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
>>>>>>> ebf0d88b9941d1c50ebe3fc3f075b1196c8a1441
}

// jeda bentar buat nungguin respon dari user
void pauseScreenUser() {
    cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
    if (cin.peek() == '\n') cin.ignore();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

<<<<<<< HEAD
// ngambil input text dan pastiin user gak iseng ngirim baris kosong
std::string getValidStringUser() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        
        // hapus spasi berlebih di awal dan akhir biar data rapi
        while (!input.empty() && input.front() == ' ') input.erase(input.begin());
        while (!input.empty() && input.back()  == ' ') input.pop_back();

        if (input.empty()) {
            std::cout << "\033[1;31mInput tidak boleh kosong. Masukkan kembali: \033[0m";
            continue;
        }
        
        // cek harus ada minimal huruf atau angka di dalamnya
        bool valid = true;
        for (char c : input) {
            if (!std::isalnum((unsigned char)c) && c != ' ') {
                valid = false;
                break;
=======
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
>>>>>>> ebf0d88b9941d1c50ebe3fc3f075b1196c8a1441
            }
        } else {
            password += ch;
            cout << '*';
        }
<<<<<<< HEAD
        if (!valid) {
            std::cout << "\033[1;31mInput mengandung karakter ga valid. Masukkan kembali: \033[0m";
            continue;
        }
        return input;
    }
=======
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
>>>>>>> ebf0d88b9941d1c50ebe3fc3f075b1196c8a1441
}

// fungsi buat nangkap input khusus angka, biar program ga error atau crash kalau salah ketik huruf
long long getValidNumberUser() {
<<<<<<< HEAD
    std::string input;
    long long num;
    while (true) {
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "\033[1;31mInput tidak boleh kosong. Masukkan angka: \033[0m";
            continue;
        }
        
        // pastiin input beneran angka semua biar aman pas dikonversi
        bool valid = true;
        for (char c : input) {
            if (!std::isdigit((unsigned char)c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "\033[1;31mHanya angka bulat yang diizinkan. Masukkan kembali: \033[0m";
            continue;
        }
        
        num = std::stoll(input);
        return num;
    }
}

// bikin menu jadi interaktif biar user bisa milih pakai keyboard panah atas bawah
int inquirerMenuUser(std::string title, std::vector<std::string> options) {
    int selected = 0;

    // ngilangin kursor berkedip dari layar sementara
    std::cout << "\033[?25l" << std::flush;

    while (true) {
        // ngereset tampilan layar biar keliatan mulus tiap ganti baris opsi
        std::cout << "\033[H\033[J";
        std::cout << "\033[1;35m================================\033[0m\n";
        std::cout << "\033[1;37m   " << title << "\033[0m\n";
        std::cout << "\033[1;35m================================\033[0m\n";
        std::cout << "\033[1;33mGunakan Panah Atas/Bawah dan Enter\033[0m\n\n";

        for (int i = 0; i < (int)options.size(); ++i) {
            if (i == selected) {
                std::cout << "\033[1;36m > " << options[i] << " \033[0m\n";
            } else {
                std::cout << "   " << options[i] << " \n";
            }
        }
        std::cout << std::flush;

        // langsung nangkap tombol yang ditekan sama user
        int ch = _getch();
        if (ch == 224) { 
            ch = _getch();
            if (ch == 72) { 
                selected = (selected > 0) ? selected - 1 : options.size() - 1;
            } else if (ch == 80) { 
                selected = (selected < (int)options.size() - 1) ? selected + 1 : 0;
            }
        } else if (ch == '\r' || ch == '\n') {
            // munculin kursornya lagi pas udah selesai nentuin pilihan
            std::cout << "\033[?25h" << std::flush;
            return selected;
=======
    string input;
    while (true) {
        getline(cin, input);
        bool valid = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) { valid = false; break; }
>>>>>>> ebf0d88b9941d1c50ebe3fc3f075b1196c8a1441
        }
        if (valid) return stoll(input);
        cout << "\033[1;31m[!] Masukkan angka yang valid: \033[0m";
    }
}