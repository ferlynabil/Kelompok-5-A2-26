#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

// 1. UPDATE PARAMETER: Tambahkan db_barang, db_rute, db_user
void konfirmasiPesanan(vector<Pesanan>& daftar_pesanan, vector<Barang>& db_barang, vector<Rute>& db_rute, vector<Pengguna>& db_user) {
    if (daftar_pesanan.empty()) {
        system("cls");
        cout << "\n=== KONFIRMASI PESANAN MASUK ===\n";
        cout << "Tidak ada pesanan untuk dikonfirmasi.\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin >> ws, dummy);
        return;
    }

    system("cls");
    cout << "\n=== DAFTAR PESANAN MENUNGGU KONFIRMASI ===\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << setw(10) << left << "ID" 
         << setw(20) << "Nama Barang" 
         << setw(10) << "Jumlah" 
         << setw(15) << "Total" 
         << setw(15) << "Status\n";
    cout << "-------------------------------------------------------------------------------\n";

    bool ada_pesanan_baru = false;
    for (const auto& p : daftar_pesanan) {
        if (p.status == "Menunggu Konfirmasi") {
            cout << setw(10) << left << p.id_pesanan 
                 << setw(20) << p.nama_barang 
                 << setw(10) << p.jumlah 
                 << setw(15) << fixed << setprecision(0) << p.total_bayar 
                 << setw(15) << p.status << "\n";
            ada_pesanan_baru = true;
        }
    }

    if (!ada_pesanan_baru) {
        cout << "\n[~] Semua pesanan sudah dikonfirmasi.\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin >> ws, dummy);
        return;
    }
    cout << "-------------------------------------------------------------------------------\n";

    string id_cari;
    cout << "\nMasukkan ID Pesanan yang ingin dikonfirmasi (Ketik '0' untuk batal): ";
    cin >> ws;
    getline(cin, id_cari);

    if (id_cari == "0") return;

    auto it = daftar_pesanan.begin();
    bool ditemukan = false;
    for (; it != daftar_pesanan.end(); ++it) {
        if (it->id_pesanan == id_cari && it->status == "Menunggu Konfirmasi") {
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        cout << "\n[-] ID Pesanan tidak ditemukan atau sudah dikonfirmasi.\n";
        system("pause");
        return;
    }

    cout << "\nApakah Anda ingin mengkonfirmasi pesanan [" << it->nama_barang << "]? (y/n): ";
    char pilih;
    cin >> pilih;

    if (pilih == 'y' || pilih == 'Y') {
        // Mengubah status
        it->status = "Menunggu Pembayaran";
        
        // 2. AUTO-UPDATE JSON DETIK INI JUGA
        // Urutan parameter: Barang, Pesanan, Rute, User
        simpanData(db_barang, daftar_pesanan, db_rute, db_user);

        cout << "\n[+] Pesanan berhasil dikonfirmasi!\n";
        cout << "[+] Data Pesanan di JSON otomatis diperbarui!\n";
        cout << "[+] Pembeli sekarang dapat melakukan pembayaran.\n";
    } else {
        cout << "\n[~] Konfirmasi dibatalkan.\n";
    }

    system("pause");
}