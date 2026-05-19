#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include "../struktur_data.h"

<<<<<<< HEAD
// fungsi tampilan terminal bersih dan rapi
=======
>>>>>>> ebf0d88b9941d1c50ebe3fc3f075b1196c8a1441
void clearScreenUser();
void pauseScreenUser();
std::string getValidStringUser();
long long getValidNumberUser();
int inquirerMenuUser(std::string title, std::vector<std::string> options);
std::string inputPassword();

<<<<<<< HEAD
// fitur utama yang bisa dipakai sama pelanggan toko
=======
>>>>>>> ebf0d88b9941d1c50ebe3fc3f075b1196c8a1441
void displayItems(std::vector<Barang>* items);
void orderItem(std::vector<Barang>* items, std::vector<Rute>* routes, std::vector<Pesanan>* trxs, std::vector<Pengguna>* users);
void payOrder(std::vector<Pesanan>* trxs, std::vector<Barang>* db_barang, std::vector<Rute>* db_rute, std::vector<Pengguna>* db_user);
void displayHistory(std::vector<Pesanan>* trxs);
void cekStatusPesanan(std::vector<Pesanan>* trxs);

<<<<<<< HEAD
// penghubung buat nampilin menu utama pelanggan
=======
>>>>>>> ebf0d88b9941d1c50ebe3fc3f075b1196c8a1441
void userMenu(std::vector<Barang>* items, std::vector<Rute>* routes, std::vector<Pesanan>* trxs, std::vector<Pengguna>* users);

#endif