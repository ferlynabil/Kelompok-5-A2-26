#ifndef VALIDASI_H
#define VALIDASI_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <thread>
#include <chrono>

using namespace std;

inline void tidur(int detik) {
    this_thread::sleep_for(chrono::seconds(detik));
}

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline string trim(const string &s) {
    int awal = 0;
    int akhir = (int)s.length() - 1;
    while (awal <= akhir && s[awal] == ' ') awal++;
    while (akhir >= awal && s[akhir] == ' ') akhir--;
    if (awal > akhir) return "";
    return s.substr(awal, akhir - awal + 1);
}

inline void validasiTidakKosong(const string &nilai, const string &field) {
    if (trim(nilai).empty())
        throw runtime_error(field + " tidak boleh kosong.");
}

inline void validasiMinPanjang(const string &nilai, int min, const string &field) {
    if ((int)nilai.length() < min)
        throw runtime_error(field + " minimal " + to_string(min) + " karakter.");
}

inline void validasiMaksPanjang(const string &nilai, int maks, const string &field) {
    if ((int)nilai.length() > maks)
        throw runtime_error(field + " maksimal " + to_string(maks) + " karakter.");
}

inline void validasiUsername(const string &username, const string &field) {
    validasiTidakKosong(username, field);
    validasiMinPanjang(username, 3, field);  
    validasiMaksPanjang(username, 20, field); 
    if (username.find(' ') != string::npos)
        throw runtime_error(field + " tidak boleh mengandung spasi.");
    for (int i = 0; i < (int)username.length(); i++) {
        char c = username[i];
        if (!isalnum(c) && c != '_' && c != '.')
            throw runtime_error(field + " hanya boleh huruf, angka, '_', atau '.'.");
    }
}
#endif