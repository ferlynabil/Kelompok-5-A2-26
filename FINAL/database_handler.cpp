#include "database_handler.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

void muatData(std::vector<Barang>& db_barang, std::vector<Pesanan>& db_pesanan, std::vector<Rute>& db_rute, std::vector<Pengguna>& db_user) {

    std::ifstream f_barang("barang.json");
    if (f_barang.is_open()) {
        json j; f_barang >> j;
        for (auto& item : j) {
            db_barang.push_back({item["id_barang"], item["nama_barang"], item["harga"], item["satuan"], item["berat"], item["stok"]});
        }
        f_barang.close();
    }
    

    std::ifstream f_rute("rute.json");
    if (f_rute.is_open() && f_rute.peek() != std::ifstream::traits_type::eof()) {
        json j; f_rute >> j;
        for (auto& item : j) {
            Rute r;

            r.id_rute = item.value("id_rute", "");
            r.kota_asal = item.value("kota_asal", "Samarinda");
            r.tujuan = item.value("tujuan", "");
            r.jarak = item.value("jarak", 0.0);
            r.biaya_reguler = item.value("biaya_reguler", 0.0);
            r.estimasi_reguler = item.value("estimasi_reguler", "-");
            r.biaya_standar = item.value("biaya_standar", 0.0);
            r.estimasi_standar = item.value("estimasi_standar", "-");
            r.biaya_premium = item.value("biaya_premium", 0.0);
            r.estimasi_premium = item.value("estimasi_premium", "-");
            
            db_rute.push_back(r);
        }
        f_rute.close();
    }

    std::ifstream f_pesanan("pesanan.json");
    if (f_pesanan.is_open()) {
        json j; f_pesanan >> j;
        for (auto& item : j) {
            db_pesanan.push_back({item["id_pesanan"], item["nama_barang"], item["jumlah"], item["total_bayar"], item["tipe_beli"], item["status"]});
        }
        f_pesanan.close();
    }
    

    std::ifstream f_user("user.json");
    if (f_user.is_open()) {
        json j; f_user >> j;
        for (auto& item : j) {
            db_user.push_back({item["username"], item["password"], item["nama_lengkap"], item["no_telp"], item["alamat"], item["role"]});
        }
        f_user.close();
    }
}

void simpanData(const std::vector<Barang>& db_barang, const std::vector<Pesanan>& db_pesanan, const std::vector<Rute>& db_rute, const std::vector<Pengguna>& db_user) {
    json j_b = json::array(), j_p = json::array(), j_r = json::array(), j_u = json::array();
    
    for (const auto& b : db_barang) j_b.push_back({{"id_barang", b.id_barang}, {"nama_barang", b.nama_barang}, {"harga", b.harga}, {"satuan", b.satuan}, {"berat", b.berat}, {"stok", b.stok}});
    

    for (const auto& r : db_rute) {
        j_r.push_back({
            {"id_rute", r.id_rute}, 
            {"kota_asal", r.kota_asal}, 
            {"tujuan", r.tujuan}, 
            {"jarak", r.jarak},
            {"biaya_reguler", r.biaya_reguler},
            {"estimasi_reguler", r.estimasi_reguler},
            {"biaya_standar", r.biaya_standar},
            {"estimasi_standar", r.estimasi_standar},
            {"biaya_premium", r.biaya_premium},
            {"estimasi_premium", r.estimasi_premium}
        });
    }
    
    for (const auto& p : db_pesanan) j_p.push_back({{"id_pesanan", p.id_pesanan}, {"nama_barang", p.nama_barang}, {"jumlah", p.jumlah}, {"total_bayar", p.total_bayar}, {"tipe_beli", p.tipe_beli}, {"status", p.status}});
    
    for (const auto& u : db_user) j_u.push_back({{"username", u.username}, {"password", u.password}, {"nama_lengkap", u.nama_lengkap}, {"no_telp", u.telepon}, {"alamat", u.alamat}, {"role", u.role}});

    std::ofstream("barang.json") << j_b.dump(4);
    std::ofstream("rute.json") << j_r.dump(4);
    std::ofstream("pesanan.json") << j_p.dump(4);
    std::ofstream("user.json") << j_u.dump(4);
}