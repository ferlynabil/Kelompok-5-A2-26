#ifndef LOGIN_H
#define LOGIN_H

#include <vector>
#include <string>
#include "../struktur_data.h"

// Fungsi untuk proses login, return true jika berhasil
bool menuLogin(std::vector<Pengguna>& database_user, std::string& user_aktif, std::string& role_aktif);

// Fungsi untuk proses registrasi akun baru
void menuRegister(std::vector<Pengguna>& database_user, std::vector<Barang>& db_barang, std::vector<Pesanan>& db_pesanan, std::vector<Rute>& db_rute);

#endif