#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include "../struktur_data.h"


void clearScreenUser();
void pauseScreenUser();
std::string getValidStringUser();
long long getValidNumberUser();
int inquirerMenuUser(std::string title, std::vector<std::string> options);


void displayItems(std::vector<Barang>* items);
void orderItem(std::vector<Barang>* items, std::vector<Rute>* routes, std::vector<Pesanan>* trxs, std::vector<Pengguna>* users);
void payOrder(std::vector<Pesanan>* trxs, std::vector<Barang>* db_barang, std::vector<Rute>* db_rute, std::vector<Pengguna>* db_user);
void displayHistory(std::vector<Pesanan>* trxs);
void cekStatusPesanan(std::vector<Pesanan>* trxs);


void userMenu(std::vector<Barang>* items, std::vector<Rute>* routes, std::vector<Pesanan>* trxs, std::vector<Pengguna>* users);

#endif