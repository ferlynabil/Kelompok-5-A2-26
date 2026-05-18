#include "user.h"
#include <cctype>
#include <cstdlib>
#include <conio.h>
#include "../database_handler.h"

void clearScreenUser() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void pauseScreenUser() {
    std::string dummy;
    std::cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
    std::getline(std::cin, dummy);
}

std::string getValidStringUser() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        while (!input.empty() && input.front() == ' ') input.erase(input.begin());
        while (!input.empty() && input.back()  == ' ') input.pop_back();

        if (input.empty()) {
            std::cout << "\033[1;31mInput tidak boleh kosong. Masukkan kembali: \033[0m";
            continue;
        }
        bool valid = true;
        for (char c : input) {
            if (!std::isalnum((unsigned char)c) && c != ' ') {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "\033[1;31mKarakter khusus tidak diizinkan. Masukkan kembali: \033[0m";
            continue;
        }
        break;
    }
    return input;
}

long long getValidNumberUser() {
    std::string input;
    long long num = -1;
    while (true) {
        std::getline(std::cin, input);
        while (!input.empty() && input.front() == ' ') input.erase(input.begin());
        while (!input.empty() && input.back()  == ' ') input.pop_back();

        if (input.empty()) {
            std::cout << "\033[1;31mInput tidak boleh kosong. Masukkan kembali: \033[0m";
            continue;
        }
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
        if (num < 0) {
            std::cout << "\033[1;31mNominal tidak boleh minus. Masukkan kembali: \033[0m";
            continue;
        }
        break;
    }
    return num;
}

int inquirerMenuUser(std::string title, std::vector<std::string> options) {
    int selected = 0;
    std::cout << "\033[?25l" << std::flush;

    while (true) {
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

        char key = _getch();
        if (key == 0 || (unsigned char)key == 224) {
            key = _getch();
            if (key == 72 && selected > 0)                           selected--;
            else if (key == 80 && selected < (int)options.size()-1)  selected++;
        } else if (key == 13) {
            std::cout << "\033[?25h" << std::flush;
            return selected;
        }
    }
}

std::string inputPassword() {
    std::string password = "";
    char ch;

    while (true) {
        ch = _getch();

        if (ch == 0 || (unsigned char)ch == 224) {
            _getch();
            continue;
        }

        if (ch == '\r') {
            std::cout << std::endl;
            break;
        } 
        else if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } 
        else {
            password += ch;
            std::cout << '*';
        }
    }
    return password;
}