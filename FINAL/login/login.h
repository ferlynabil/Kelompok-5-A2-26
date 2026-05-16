#ifndef LOGIN_H
#define LOGIN_H

#include <vector>
#include <string>
#include "../struktur_data.h"

bool menuLogin(std::vector<Pengguna>& database_user, std::string& user_aktif, std::string& role_aktif);
// Parameter ini WAJIB 4 supaya main.cpp tidak marah
void menuRegister(std::vector<Pengguna>& database_user, std::vector<Barang>& db_barang, std::vector<Pesanan>& db_pesanan, std::vector<Rute>& db_rute);

#endif