#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <vector>
#include "struktur_data.h"


void muatData(std::vector<Barang>& b, std::vector<Pesanan>& p, std::vector<Rute>& r, std::vector<Pengguna>& u);


void simpanData(const std::vector<Barang>& b, const std::vector<Pesanan>& p, const std::vector<Rute>& r, const std::vector<Pengguna>& u);

#endif