#ifndef STRUKTUR_DATA_H
#define STRUKTUR_DATA_H

#include <string>

struct Pengguna {
    std::string username;
    std::string password;
    std::string nama_lengkap;
    std::string telepon;
    std::string alamat;
    std::string role;
};

struct Barang {
    std::string id_barang;
    std::string nama_barang;
    double harga;
    std::string satuan;
    double berat;
    int stok;
};

struct Rute {
    std::string id_rute;
    std::string kota_asal;
    std::string tujuan;
    double jarak;
    
    double biaya_reguler;
    std::string estimasi_reguler;
    
    double biaya_standar;
    std::string estimasi_standar;
    
    double biaya_premium;
    std::string estimasi_premium;
};

struct Pesanan {
    std::string id_pesanan;
    std::string nama_barang;
    int jumlah;
    double total_bayar;
    std::string tipe_beli;
    std::string status;
};

#endif