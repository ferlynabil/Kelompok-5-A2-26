#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

void menuAdmin(vector<Barang>& database_barang, vector<Pesanan>& database_pesanan,
               vector<Rute>& database_rute, vector<Pengguna>& database_user) {

    bool di_menu_admin = true;
    int pilihan = 0; // posisi kursor menu saat ini

    vector<string> list_menu = {
        "Tambah Barang",
        "Lihat Barang",
        "Edit Barang",
        "Hapus Barang",
        "Tambah Rute Pengiriman",
        "Lihat Rute Pengiriman",
        "Edit Rute Pengiriman",
        "Hapus Rute Pengiriman",
        "Konfirmasi Pesanan Baru",
        "Layanan Pembayaran & Pengiriman",
        "Keluar ke Menu Utama"
    };

    while (di_menu_admin) {
        system("cls");
        cout << "=== DASHBOARD ADMIN TOKO BANGUNAN ===\n";
        cout << "Gunakan Panah Atas/Bawah dan Enter:\n\n";

        // menampilkan  semua menu lalu highlight yang sedang dipilih
        for (int i = 0; i < list_menu.size(); i++) {
            if (i == pilihan)
                cout << "  > \033[1;32m" << list_menu[i] << "\033[0m <\n";
            else
                cout << "    " << list_menu[i] << "\n"; 
        }

        char tombol = _getch();

        if (tombol == 72 && pilihan > 0) pilihan--;           // panah atas
        else if (tombol == 80 && pilihan < list_menu.size() - 1) pilihan++; // panah bawah
        else if (tombol == '\r') {
            // jalankan fungsi sesuai menu yang dipilih
            switch (pilihan) {
                case 0: tambahBarang(database_barang, database_pesanan, database_rute, database_user); break; 
                case 1: lihatBarang(database_barang); break;
                case 2: editBarang(database_barang, database_pesanan, database_rute, database_user); break; 
                case 3: hapusBarang(database_barang, database_pesanan, database_rute, database_user); break;
                case 4: tambahRute(database_rute, database_barang, database_pesanan, database_user); break;
                case 5: lihatRute(database_rute); break;
                case 6: editRute(database_rute, database_barang, database_pesanan, database_user); break;
                case 7: hapusRute(database_rute, database_barang, database_pesanan, database_user); break;
                case 8: konfirmasiPesanan(database_pesanan, database_barang, database_rute, database_user); break;
                case 9: layananPembayaranDanPengiriman(database_pesanan, database_barang, database_rute, database_user); break;
                case 10: di_menu_admin = false; break; // keluar dari loop  baru balik ke menu utama
            }
        }
    }
}