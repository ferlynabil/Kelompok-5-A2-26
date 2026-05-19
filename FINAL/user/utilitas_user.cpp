#include "user.h"
#include <cctype>
#include <cstdlib>
#include <conio.h>
#include "../database_handler.h"

// bersihin layar terminal biar selalu keliatan fresh pas pindah menu
void clearScreenUser() {
    std::cout << "\033[2J\033[H" << std::flush;
}

// jeda bentar buat nungguin respon dari user
void pauseScreenUser() {
    std::string dummy;
    std::cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
    std::getline(std::cin, dummy);
}

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
            }
        }
        if (!valid) {
            std::cout << "\033[1;31mInput mengandung karakter ga valid. Masukkan kembali: \033[0m";
            continue;
        }
        return input;
    }
}

// fungsi buat nangkap input khusus angka, biar program ga error atau crash kalau salah ketik huruf
long long getValidNumberUser() {
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
        }
    }
}