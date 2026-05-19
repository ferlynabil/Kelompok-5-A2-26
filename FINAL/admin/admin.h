#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include "../struktur_data.h"


void tambahBarang(std::vector<Barang>& daftar_barang, std::vector<Pesanan>& db_pesanan, std::vector<Rute>& db_rute, std::vector<Pengguna>& db_user);
void lihatBarang(const std::vector<Barang>& daftar_barang);
void editBarang(std::vector<Barang>& daftar_barang, std::vector<Pesanan>& db_pesanan, std::vector<Rute>& db_rute, std::vector<Pengguna>& db_user);
void hapusBarang(std::vector<Barang>& daftar_barang, std::vector<Pesanan>& db_pesanan, std::vector<Rute>& db_rute, std::vector<Pengguna>& db_user);


void konfirmasiPesanan(std::vector<Pesanan>& daftar_pesanan, std::vector<Barang>& db_barang, std::vector<Rute>& db_rute, std::vector<Pengguna>& db_user);
void layananPembayaranDanPengiriman(std::vector<Pesanan>& daftar_pesanan, std::vector<Barang>& db_barang, std::vector<Rute>& db_rute, std::vector<Pengguna>& db_user);


void tambahRute(std::vector<Rute>& daftar_rute, std::vector<Barang>& db_barang, std::vector<Pesanan>& db_pesanan, std::vector<Pengguna>& db_user);
void lihatRute(const std::vector<Rute>& daftar_rute);
void editRute(std::vector<Rute>& daftar_rute, std::vector<Barang>& db_barang, std::vector<Pesanan>& db_pesanan, std::vector<Pengguna>& db_user);
void hapusRute(std::vector<Rute>& daftar_rute, std::vector<Barang>& db_barang, std::vector<Pesanan>& db_pesanan, std::vector<Pengguna>& db_user);


void menuAdmin(std::vector<Barang>& database_barang, std::vector<Pesanan>& database_pesanan, std::vector<Rute>& database_rute, std::vector<Pengguna>& database_user);

#endif