// ===== INITIAL DATA SYSTEM =====
const DEFAULT_USERS = [
    { username: "Admin", password: "123", nama_lengkap: "Admin Gudang", no_telp: "0811", alamat: "Samarinda", role: "Admin" },
    { username: "Nuron", password: "037", nama_lengkap: "Nuron", no_telp: "0812", alamat: "Samarinda", role: "Customer" },
    { username: "ewok", password: "0987654321", nama_lengkap: "ewok", no_telp: "1234567890987", alamat: "ewok city", role: "Customer" },
    { username: "Nulaa", password: "Nulaa0090", nama_lengkap: "Nurul", no_telp: "0833345687", alamat: "Samarinda", role: "Customer" }
];

const DEFAULT_BARANG = [
    { id_barang: "SDG-001", nama_barang: "Semen Tiga Roda", harga: 100000.0, satuan: "Sak", berat: 10.0, stok: 5 }
];

const DEFAULT_RUTE = [
    { id_rute: "R01", kota_asal: "Samarinda", tujuan: "Bali", jarak: 150.0, biaya_reguler: 0.0, estimasi_reguler: "-", biaya_standar: 150000.0, estimasi_standar: "5 Hari", biaya_premium: 300000.0, estimasi_premium: "3 Hari" },
    { id_rute: "R02", kota_asal: "Samarinda", tujuan: "Tenggarong", jarak: 25.0, biaya_reguler: 10000.0, estimasi_reguler: "2 Hari", biaya_standar: 15000.0, estimasi_standar: "1 Hari", biaya_premium: 0.0, estimasi_premium: "-" }
];

const DEFAULT_PESANAN = [
    { id_pesanan: "TRX001", nama_barang: "Semen Tiga Roda", jumlah: 2, total_bayar: 500000.0, tipe_beli: "Kirim Ekspedisi", status: "Lunas" },
    { id_pesanan: "TRX002", nama_barang: "Semen Tiga Roda", jumlah: 3, total_bayar: 300000.0, tipe_beli: "Ambil di Toko", status: "Lunas" }
];

// Initialize localStorage databases
function initDatabase() {
    if (!localStorage.getItem("molotov_users")) localStorage.setItem("molotov_users", JSON.stringify(DEFAULT_USERS));
    if (!localStorage.getItem("molotov_barang")) localStorage.setItem("molotov_barang", JSON.stringify(DEFAULT_BARANG));
    if (!localStorage.getItem("molotov_rute")) localStorage.setItem("molotov_rute", JSON.stringify(DEFAULT_RUTE));
    if (!localStorage.getItem("molotov_pesanan")) localStorage.setItem("molotov_pesanan", JSON.stringify(DEFAULT_PESANAN));
}

initDatabase();

// Load data functions
const db = {
    getUsers: () => JSON.parse(localStorage.getItem("molotov_users")),
    saveUsers: (data) => localStorage.setItem("molotov_users", JSON.stringify(data)),
    getBarang: () => JSON.parse(localStorage.getItem("molotov_barang")),
    saveBarang: (data) => localStorage.setItem("molotov_barang", JSON.stringify(data)),
    getRute: () => JSON.parse(localStorage.getItem("molotov_rute")),
    saveRute: (data) => localStorage.setItem("molotov_rute", JSON.stringify(data)),
    getPesanan: () => JSON.parse(localStorage.getItem("molotov_pesanan")),
    savePesanan: (data) => localStorage.setItem("molotov_pesanan", JSON.stringify(data))
};

// ===== STATE MANAGEMENT =====
let currentUser = null;
let activeTabName = "";

// ===== CLOCK & UI INITIALIZATION =====
function startClock() {
    const liveClock = document.getElementById("live-clock");
    setInterval(() => {
        const now = new Date();
        liveClock.innerText = now.toLocaleString("id-ID", {
            weekday: 'long', year: 'numeric', month: 'long', day: 'numeric',
            hour: '2-digit', minute: '2-digit', second: '2-digit'
        });
    }, 1000);
}
startClock();

// Toast Notifications
function showToast(message, type = "success") {
    const container = document.getElementById("toast-container");
    const toast = document.createElement("div");
    toast.className = `toast toast-${type}`;
    toast.innerText = message;
    container.appendChild(toast);
    setTimeout(() => {
        toast.remove();
    }, 3000);
}

// Modal Management
function showModal(title, contentHtml) {
    document.getElementById("modal-title").innerText = title;
    document.getElementById("modal-body").innerHTML = contentHtml;
    document.getElementById("modal-overlay").classList.add("active");
}
function closeModal() {
    document.getElementById("modal-overlay").classList.remove("active");
}

// ===== AUTHENTICATION LOGIC =====
function switchAuth(type) {
    const loginForm = document.getElementById("login-form");
    const registerForm = document.getElementById("register-form");
    if (type === "register") {
        loginForm.classList.remove("active");
        registerForm.classList.add("active");
    } else {
        registerForm.classList.remove("active");
        loginForm.classList.add("active");
    }
}

function togglePassword(id, btn) {
    const input = document.getElementById(id);
    const eyeOpen = btn.querySelector(".eye-open");
    const eyeClosed = btn.querySelector(".eye-closed");
    if (input.type === "password") {
        input.type = "text";
        eyeOpen.classList.add("hidden");
        eyeClosed.classList.remove("hidden");
    } else {
        input.type = "password";
        eyeOpen.classList.remove("hidden");
        eyeClosed.classList.add("hidden");
    }
}

// Validate Username
function validateUsername(username) {
    if (username.length < 3 || username.length > 20) return "Username harus antara 3 - 20 karakter.";
    if (username.includes(" ")) return "Username tidak boleh mengandung spasi.";
    if (!/^[a-zA-Z0-9_.]+$/.test(username)) return "Username hanya boleh huruf, angka, '_' atau '.'.";
    return null;
}

// Handle Login
document.getElementById("login-form").onsubmit = function (e) {
    e.preventDefault();
    const usernameInput = document.getElementById("login-username").value.trim();
    const passwordInput = document.getElementById("login-password").value;

    const users = db.getUsers();
    const user = users.find(u => u.username.toLowerCase() === usernameInput.toLowerCase() && u.password === passwordInput);

    if (user) {
        currentUser = user;
        sessionStorage.setItem("molotov_current_user", JSON.stringify(currentUser));
        showToast(`Selamat datang kembali, ${user.nama_lengkap}!`, "success");
        enterApp();
    } else {
        showToast("Username atau password salah.", "error");
    }
};

// Handle Register
document.getElementById("register-form").onsubmit = function (e) {
    e.preventDefault();
    const nama = document.getElementById("reg-nama").value.trim();
    const username = document.getElementById("reg-username").value.trim();
    const telepon = document.getElementById("reg-telepon").value.trim();
    const alamat = document.getElementById("reg-alamat").value.trim();
    const password = document.getElementById("reg-password").value;
    const konfirmasi = document.getElementById("reg-konfirmasi").value;

    // Validations
    if (nama.length < 3 || nama.length > 50) return showToast("Nama Lengkap harus antara 3-50 karakter.", "error");
    if (/[0-9]/.test(nama)) return showToast("Nama Lengkap tidak boleh mengandung angka.", "error");
    if (/[^a-zA-Z\s]/.test(nama)) return showToast("Nama Lengkap tidak boleh mengandung simbol/karakter khusus.", "error");

    const usernameErr = validateUsername(username);
    if (usernameErr) return showToast(usernameErr, "error");

    const users = db.getUsers();
    if (users.some(u => u.username.toLowerCase() === username.toLowerCase())) {
        return showToast("Username sudah terdaftar.", "error");
    }

    if (!/^\d+$/.test(telepon)) return showToast("No Telepon hanya boleh berisi angka.", "error");
    if (telepon.length < 9 || telepon.length > 13) return showToast("No Telepon harus antara 9-13 digit.", "error");
    if (alamat.length === 0 || alamat.length > 100) return showToast("Alamat harus antara 1-100 karakter.", "error");
    if (password.length < 8 || password.length > 30) return showToast("Password harus antara 8-30 karakter.", "error");
    if (password !== konfirmasi) return showToast("Konfirmasi password tidak cocok.", "error");

    // Add new Customer
    users.push({ username, password, nama_lengkap: nama, no_telp: telepon, alamat, role: "Customer" });
    db.saveUsers(users);

    showToast("Registrasi berhasil! Silakan masuk.", "success");
    document.getElementById("register-form").reset();
    switchAuth("login");
};

// ===== SIDEBAR NAVIGATION MANAGER =====
function toggleSidebar() {
    document.getElementById("sidebar").classList.toggle("open");
}

function enterApp() {
    document.getElementById("auth-screen").classList.remove("active");
    document.getElementById("app-screen").classList.add("active");

    // User details update
    document.getElementById("user-name").innerText = currentUser.nama_lengkap;
    document.getElementById("user-role").innerText = currentUser.role === "Admin" ? "Administrator" : "Customer";
    document.getElementById("user-avatar").innerText = currentUser.nama_lengkap.charAt(0).toUpperCase();

    // Render Navigation Items
    const sidebarNav = document.getElementById("sidebar-nav");
    sidebarNav.innerHTML = "";

    const adminMenu = [
        { label: "Dashboard", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path d="M10.707 2.293a1 1 0 00-1.414 0l-7 7a1 1 0 001.414 1.414L4 10.414V17a1 1 0 001 1h2a1 1 0 001-1v-2a1 1 0 011-1h2a1 1 0 011 1v2a1 1 0 001 1h2a1 1 0 001-1v-6.586l.293.293a1 1 0 001.414-1.414l-7-7z"/></svg>`, id: "admin-dashboard" },
        { label: "Kelola Barang", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path d="M7 3a1 1 0 000 2h6a1 1 0 100-2H7zM4 7a1 1 0 011-1h10a1 1 0 110 2H5a1 1 0 01-1-1zM2 11a2 2 0 012-2h12a2 2 0 012 2v4a2 2 0 01-2 2H4a2 2 0 01-2-2v-4z"/></svg>`, id: "admin-barang" },
        { label: "Kelola Rute", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M12 1.586l-4 4v12.828l4-4V1.586zM3.707 3.293A1 1 0 002 4v10a1 1 0 00.293.707L6 18.414V5.586L3.707 3.293zM17.707 5.293L14 1.586v12.828l2.293 2.293A1 1 0 0018 16V6a1 1 0 00-.293-.707z" clip-rule="evenodd"/></svg>`, id: "admin-rute" },
        { label: "Konfirmasi Pesanan", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm3.707-9.293a1 1 0 00-1.414-1.414L9 10.586 7.707 9.293a1 1 0 00-1.414 1.414l2 2a1 1 0 001.414 0l4-4z" clip-rule="evenodd"/></svg>`, id: "admin-konfirmasi" },
        { label: "Pembayaran & Ekspedisi", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path d="M8 16.5a1.5 1.5 0 11-3 0 1.5 1.5 0 013 0zM15 16.5a1.5 1.5 0 11-3 0 1.5 1.5 0 013 0z"/><path d="M3 4a1 1 0 00-1 1v10a1 1 0 001 1h1.05a2.5 2.5 0 014.9 0H10a1 1 0 001-1V5a1 1 0 00-1-1H3zM14 7a1 1 0 00-1 1v3.95a2.5 2.5 0 014.9 0H18V9l-4-2z"/></svg>`, id: "admin-pembayaran" }
    ];

    const customerMenu = [
        { label: "Dashboard", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path d="M10.707 2.293a1 1 0 00-1.414 0l-7 7a1 1 0 001.414 1.414L4 10.414V17a1 1 0 001 1h2a1 1 0 001-1v-2a1 1 0 011-1h2a1 1 0 011 1v2a1 1 0 001 1h2a1 1 0 001-1v-6.586l.293.293a1 1 0 001.414-1.414l-7-7z"/></svg>`, id: "customer-dashboard" },
        { label: "Lihat Barang", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 2a4 4 0 00-4 4v1H5a1 1 0 00-.994.89l-1 9A1 1 0 004 18h12a1 1 0 00.994-1.11l-1-9A1 1 0 0015 7h-1V6a4 4 0 00-4-4zm2 5V6a2 2 0 10-4 0v1h4zm-6 3a1 1 0 112 0 1 1 0 01-2 0zm7-1a1 1 0 100 2 1 1 0 000-2z" clip-rule="evenodd"/></svg>`, id: "customer-lihat" },
        { label: "Pesan Barang", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path d="M3 1a1 1 0 000 2h1.22l.305 1.222a.997.997 0 00.01.042l1.358 5.43-.893.892C3.74 11.846 4.632 14 6.414 14H15a1 1 0 100-2H6.414l1-1H14a1 1 0 00.894-.553l3-6A1 1 0 0017 3H6.28l-.31-1.243A1 1 0 005 1H3zM16 16.5a1.5 1.5 0 11-3 0 1.5 1.5 0 013 0zM6.5 18a1.5 1.5 0 100-3 1.5 1.5 0 000 3z"/></svg>`, id: "customer-pesan" },
        { label: "Status Pesanan", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M18 10a8 8 0 11-16 0 8 8 0 0116 0zm-7-4a1 1 0 11-2 0 1 1 0 012 0zm-1 9a1 1 0 01-1-1v-4a1 1 0 112 0v4a1 1 0 01-1 1z" clip-rule="evenodd"/></svg>`, id: "customer-status" },
        { label: "Bayar Pesanan", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path d="M4 4a2 2 0 00-2 2v1h16V6a2 2 0 00-2-2H4zM18 9H2v5a2 2 0 002 2h12a2 2 0 002-2V9zM4 13a1 1 0 011-1h1a1 1 0 110 2H5a1 1 0 01-1-1zm5-1a1 1 0 100 2h3a1 1 0 100-2H9z"/></svg>`, id: "customer-bayar" },
        { label: "Riwayat Transaksi", icon: `<svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm1-12a1 1 0 10-2 0v4a1 1 0 00.293.707l2.8 2.8a1 1 0 101.414-1.414L11 9.586V6z" clip-rule="evenodd"/></svg>`, id: "customer-riwayat" }
    ];

    const currentMenu = currentUser.role === "Admin" ? adminMenu : customerMenu;

    currentMenu.forEach(item => {
        const navLink = document.createElement("div");
        navLink.className = "nav-item";
        navLink.innerHTML = `${item.icon}<span>${item.label}</span>`;
        navLink.onclick = () => activeTab(item.id);
        navLink.setAttribute("data-tab-id", item.id);
        sidebarNav.appendChild(navLink);
    });

    // Default tab
    activeTab(currentMenu[0].id);
}

function activeTab(tabId) {
    activeTabName = tabId;
    sessionStorage.setItem("molotov_active_tab", tabId);

    // Nav active styling
    document.querySelectorAll(".nav-item").forEach(item => {
        if (item.getAttribute("data-tab-id") === tabId) {
            item.classList.add("active");
        } else {
            item.classList.remove("active");
        }
    });

    // Set page title
    const activeLabel = document.querySelector(`.nav-item[data-tab-id="${tabId}"] span`).innerText;
    document.getElementById("page-title").innerText = activeLabel;

    // Render tab content
    renderTabContent(tabId);

    // Close sidebar on mobile
    document.getElementById("sidebar").classList.remove("open");
}

function logout() {
    currentUser = null;
    sessionStorage.removeItem("molotov_current_user");
    sessionStorage.removeItem("molotov_active_tab");
    document.getElementById("app-screen").classList.remove("active");
    document.getElementById("auth-screen").classList.add("active");
    document.getElementById("login-form").reset();
    showToast("Anda telah keluar dari aplikasi.", "info");
}

// ===== TAB RENDERING DISPATCHER =====
function renderTabContent(tabId) {
    const area = document.getElementById("content-area");
    area.innerHTML = "";

    switch (tabId) {
        // Admin tabs
        case "admin-dashboard": renderAdminDashboard(area); break;
        case "admin-barang": renderAdminBarang(area); break;
        case "admin-rute": renderAdminRute(area); break;
        case "admin-konfirmasi": renderAdminKonfirmasi(area); break;
        case "admin-pembayaran": renderAdminLayanan(area); break;

        // Customer tabs
        case "customer-dashboard": renderCustomerDashboard(area); break;
        case "customer-lihat": renderCustomerLihatBarang(area); break;
        case "customer-pesan": renderCustomerPesanBarang(area); break;
        case "customer-status": renderCustomerStatus(area); break;
        case "customer-bayar": renderCustomerBayar(area); break;
        case "customer-riwayat": renderCustomerRiwayat(area); break;
    }
}

// ===== VIEW RENDERING DEFINITIONS =====

// 1. ADMIN DASHBOARD
function renderAdminDashboard(container) {
    const barang = db.getBarang();
    const rute = db.getRute();
    const pesanan = db.getPesanan();
    const pendingOrders = pesanan.filter(p => p.status === "Menunggu Konfirmasi").length;
    const lunasOrders = pesanan.filter(p => p.status === "Lunas").length;

    container.innerHTML = `
        <div class="dashboard-grid">
            <div class="stat-card">
                <div class="stat-icon orange"><svg viewBox="0 0 20 20" fill="currentColor"><path d="M7 3a1 1 0 000 2h6a1 1 0 100-2H7zM4 7a1 1 0 011-1h10a1 1 0 110 2H5a1 1 0 01-1-1zM2 11a2 2 0 012-2h12a2 2 0 012 2v4a2 2 0 01-2 2H4a2 2 0 01-2-2v-4z"/></svg></div>
                <div class="stat-label">Total Barang</div>
                <div class="stat-value">${barang.length}</div>
            </div>
            <div class="stat-card">
                <div class="stat-icon cyan"><svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M12 1.586l-4 4v12.828l4-4V1.586z" clip-rule="evenodd"/></svg></div>
                <div class="stat-label">Total Rute</div>
                <div class="stat-value">${rute.length}</div>
            </div>
            <div class="stat-card">
                <div class="stat-icon yellow"><svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm3.707-9.293a1 1 0 00-1.414-1.414L9 10.586 7.707 9.293a1 1 0 00-1.414 1.414l2 2a1 1 0 001.414 0l4-4z" clip-rule="evenodd"/></svg></div>
                <div class="stat-label">Menunggu Konfirmasi</div>
                <div class="stat-value">${pendingOrders}</div>
            </div>
            <div class="stat-card">
                <div class="stat-icon green"><svg viewBox="0 0 20 20" fill="currentColor"><path d="M4 4a2 2 0 00-2 2v1h16V6a2 2 0 00-2-2H4z"/></svg></div>
                <div class="stat-label">Lunas (Belum Dikirim)</div>
                <div class="stat-value">${lunasOrders}</div>
            </div>
        </div>

        <div class="section-header">
            <h3>Akses Cepat Admin</h3>
        </div>
        <div class="quick-actions">
            <div class="quick-action-card" onclick="activeTab('admin-barang')">
                <div class="qa-icon" style="background:var(--accent-glow); color:var(--accent)">
                    <svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 3a1 1 0 011 1v5h5a1 1 0 110 2h-5v5a1 1 0 11-2 0v-5H4a1 1 0 110-2h5V4a1 1 0 011-1z" clip-rule="evenodd"/></svg>
                </div>
                <div>
                    <div class="qa-text">Kelola Gudang</div>
                    <div class="qa-desc">Tambah & update data barang.</div>
                </div>
            </div>
            <div class="quick-action-card" onclick="activeTab('admin-konfirmasi')">
                <div class="qa-icon" style="background:var(--yellow-bg); color:var(--yellow)">
                    <svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M5 2a1 1 0 011 1v1h1a1 1 0 010 2H6v1a1 1 0 01-2 0V6H3a1 1 0 010-2h1V3a1 1 0 011-1zm1.293 9.707a1 1 0 010-1.414l3-3a1 1 0 011.414 0l3 3a1 1 0 01-1.414 1.414L11 9.414V13a1 1 0 11-2 0V9.414l-1.293 1.293a1 1 0 01-1.414 0z" clip-rule="evenodd"/></svg>
                </div>
                <div>
                    <div class="qa-text">Konfirmasi Order</div>
                    <div class="qa-desc">Setujui pesanan pelanggan.</div>
                </div>
            </div>
            <div class="quick-action-card" onclick="activeTab('admin-pembayaran')">
                <div class="qa-icon" style="background:var(--green-bg); color:var(--green)">
                    <svg viewBox="0 0 20 20" fill="currentColor"><path d="M8 16.5a1.5 1.5 0 11-3 0 1.5 1.5 0 013 0zM15 16.5a1.5 1.5 0 11-3 0 1.5 1.5 0 013 0z"/></svg>
                </div>
                <div>
                    <div class="qa-text">Proses Ekspedisi</div>
                    <div class="qa-desc">Kirim barang yang lunas.</div>
                </div>
            </div>
        </div>
    `;
}

// 2. ADMIN KELOLA BARANG
function renderAdminBarang(container) {
    const barang = db.getBarang();
    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Daftar Inventaris Barang</h3>
                <div class="table-actions">
                    <div class="search-box">
                        <svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M8 4a4 4 0 100 8 4 4 0 000-8zM2 8a6 6 0 1110.89 3.476l4.817 4.817a1 1 0 01-1.414 1.414l-4.816-4.816A6 6 0 012 8z" clip-rule="evenodd"/></svg>
                        <input type="text" placeholder="Cari barang..." oninput="filterBarangTable(this.value)">
                    </div>
                    <button class="btn btn-primary" onclick="showFormAddBarang()">
                        <svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 5a1 1 0 011 1v3h3a1 1 0 110 2h-3v3a1 1 0 11-2 0v-3H6a1 1 0 110-2h3V6a1 1 0 011-1z" clip-rule="evenodd"/></svg>
                        <span>Tambah Barang</span>
                    </button>
                </div>
            </div>
            <div style="overflow-x:auto;">
                <table id="barang-table">
                    <thead>
                        <tr>
                            <th>ID Barang</th>
                            <th>Nama Barang</th>
                            <th>Harga</th>
                            <th>Satuan</th>
                            <th>Berat / Qty</th>
                            <th>Stok</th>
                            <th style="text-align:right;">Aksi</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${renderBarangRows(barang)}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

function renderBarangRows(list) {
    if (list.length === 0) return `<tr><td colspan="7" class="table-empty">Belum ada barang di gudang.</td></tr>`;
    return list.map(b => `
        <tr>
            <td><span class="id-text">${b.id_barang}</span></td>
            <td style="font-weight:600;">${b.nama_barang}</td>
            <td><span class="price-text">Rp${b.harga.toLocaleString("id-ID")}</span></td>
            <td>${b.satuan}</td>
            <td>${b.berat} kg</td>
            <td><span class="badge ${b.stok > 3 ? 'badge-green' : 'badge-red'}">${b.stok} unit</span></td>
            <td style="text-align:right;">
                <button class="btn btn-secondary btn-sm" onclick="showFormEditBarang('${b.id_barang}')" style="margin-right:6px;">Edit</button>
                <button class="btn btn-danger btn-sm" onclick="deleteBarang('${b.id_barang}')">Hapus</button>
            </td>
        </tr>
    `).join("");
}

function filterBarangTable(val) {
    const term = val.toLowerCase();
    const filtered = db.getBarang().filter(b => b.nama_barang.toLowerCase().includes(term) || b.id_barang.toLowerCase().includes(term));
    document.querySelector("#barang-table tbody").innerHTML = renderBarangRows(filtered);
}

function showFormAddBarang() {
    const html = `
        <form id="form-add-barang" onsubmit="saveNewBarang(event)">
            <div class="input-group">
                <label>Nama Barang</label>
                <div class="input-wrapper">
                    <input type="text" id="add-nama" placeholder="Contoh: Semen Gresik" required>
                </div>
            </div>
            <div class="input-group">
                <label>Kategori Berat</label>
                <div class="select-wrapper">
                    <select id="add-kategori" required>
                        <option value="RGN">Ringan (RGN)</option>
                        <option value="SDG" selected>Sedang (SDG)</option>
                        <option value="BRT">Berat (BRT)</option>
                    </select>
                </div>
            </div>
            <div class="input-group">
                <label>Harga (Rp)</label>
                <div class="input-wrapper">
                    <input type="number" id="add-harga" placeholder="Masukkan harga" min="1" required>
                </div>
            </div>
            <div class="input-group">
                <label>Satuan</label>
                <div class="select-wrapper">
                    <select id="add-satuan" required>
                        <option value="Kilogram (kg)">Kilogram (kg)</option>
                        <option value="Gram (g)">Gram (g)</option>
                        <option value="Liter (L)">Liter (L)</option>
                        <option value="Buah/Pcs">Buah/Pcs</option>
                        <option value="Sak" selected>Sak</option>
                        <option value="Meter (m)">Meter (m)</option>
                    </select>
                </div>
            </div>
            <div class="input-group">
                <label>Berat per Satuan (kg)</label>
                <div class="input-wrapper">
                    <input type="number" id="add-berat" placeholder="Contoh: 10" step="0.1" min="0.1" required>
                </div>
            </div>
            <div class="input-group">
                <label>Stok Awal</label>
                <div class="input-wrapper">
                    <input type="number" id="add-stok" placeholder="Contoh: 5" min="0" required>
                </div>
            </div>
            <div class="btn-row">
                <button type="button" class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button type="submit" class="btn btn-primary">Simpan Barang</button>
            </div>
        </form>
    `;
    showModal("Tambah Barang Baru", html);
}

function saveNewBarang(e) {
    e.preventDefault();
    const nama = document.getElementById("add-nama").value.trim();
    const kategori = document.getElementById("add-kategori").value;
    const harga = parseFloat(document.getElementById("add-harga").value);
    const satuan = document.getElementById("add-satuan").value;
    const berat = parseFloat(document.getElementById("add-berat").value);
    const stok = parseInt(document.getElementById("add-stok").value);

    if (!nama) return showToast("Nama barang tidak boleh kosong.", "error");

    const barang = db.getBarang();
    
    // Auto increment ID
    const matches = barang.filter(b => b.id_barang.startsWith(kategori));
    let nextNum = matches.length + 1;
    let nextId = `${kategori}-${String(nextNum).padStart(3, '0')}`;
    while (barang.some(b => b.id_barang === nextId)) {
        nextNum++;
        nextId = `${kategori}-${String(nextNum).padStart(3, '0')}`;
    }

    barang.push({ id_barang: nextId, nama_barang: nama, harga, satuan, berat, stok });
    db.saveBarang(barang);
    showToast(`Barang ${nama} berhasil ditambahkan!`, "success");
    closeModal();
    renderTabContent(activeTabName);
}

function showFormEditBarang(id) {
    const barangList = db.getBarang();
    const item = barangList.find(b => b.id_barang === id);
    if (!item) return showToast("Barang tidak ditemukan.", "error");

    const html = `
        <form id="form-edit-barang" onsubmit="saveEditedBarang(event, '${id}')">
            <div class="input-group">
                <label>ID Barang</label>
                <div class="input-wrapper" style="opacity:0.6;">
                    <input type="text" value="${item.id_barang}" readonly>
                </div>
            </div>
            <div class="input-group">
                <label>Nama Barang</label>
                <div class="input-wrapper">
                    <input type="text" id="edit-nama" value="${item.nama_barang}" required>
                </div>
            </div>
            <div class="input-group">
                <label>Harga (Rp)</label>
                <div class="input-wrapper">
                    <input type="number" id="edit-harga" value="${item.harga}" min="1" required>
                </div>
            </div>
            <div class="input-group">
                <label>Satuan</label>
                <div class="select-wrapper">
                    <select id="edit-satuan" required>
                        <option value="Kilogram (kg)" ${item.satuan === 'Kilogram (kg)' ? 'selected' : ''}>Kilogram (kg)</option>
                        <option value="Gram (g)" ${item.satuan === 'Gram (g)' ? 'selected' : ''}>Gram (g)</option>
                        <option value="Liter (L)" ${item.satuan === 'Liter (L)' ? 'selected' : ''}>Liter (L)</option>
                        <option value="Buah/Pcs" ${item.satuan === 'Buah/Pcs' ? 'selected' : ''}>Buah/Pcs</option>
                        <option value="Sak" ${item.satuan === 'Sak' ? 'selected' : ''}>Sak</option>
                        <option value="Meter (m)" ${item.satuan === 'Meter (m)' ? 'selected' : ''}>Meter (m)</option>
                    </select>
                </div>
            </div>
            <div class="input-group">
                <label>Berat per Satuan (kg)</label>
                <div class="input-wrapper">
                    <input type="number" id="edit-berat" value="${item.berat}" step="0.1" min="0.1" required>
                </div>
            </div>
            <div class="input-group">
                <label>Stok</label>
                <div class="input-wrapper">
                    <input type="number" id="edit-stok" value="${item.stok}" min="0" required>
                </div>
            </div>
            <div class="btn-row">
                <button type="button" class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button type="submit" class="btn btn-primary">Simpan Perubahan</button>
            </div>
        </form>
    `;
    showModal("Edit Barang", html);
}

function saveEditedBarang(e, id) {
    e.preventDefault();
    const nama = document.getElementById("edit-nama").value.trim();
    const harga = parseFloat(document.getElementById("edit-harga").value);
    const satuan = document.getElementById("edit-satuan").value;
    const berat = parseFloat(document.getElementById("edit-berat").value);
    const stok = parseInt(document.getElementById("edit-stok").value);

    const barangList = db.getBarang();
    const idx = barangList.findIndex(b => b.id_barang === id);
    if (idx !== -1) {
        barangList[idx] = { ...barangList[idx], nama_barang: nama, harga, satuan, berat, stok };
        db.saveBarang(barangList);
        showToast("Perubahan data barang berhasil disimpan.", "success");
        closeModal();
        renderTabContent(activeTabName);
    }
}

function deleteBarang(id) {
    if (confirm(`Apakah Anda yakin ingin menghapus barang dengan ID ${id}?`)) {
        const barangList = db.getBarang().filter(b => b.id_barang !== id);
        db.saveBarang(barangList);
        showToast("Barang telah dihapus dari database.", "info");
        renderTabContent(activeTabName);
    }
}

// 3. ADMIN KELOLA RUTE
function renderAdminRute(container) {
    const rute = db.getRute();
    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Daftar Rute Pengiriman</h3>
                <div class="table-actions">
                    <button class="btn btn-primary" onclick="showFormAddRute()">
                        <svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 5a1 1 0 011 1v3h3a1 1 0 110 2h-3v3a1 1 0 11-2 0v-3H6a1 1 0 110-2h3V6a1 1 0 011-1z" clip-rule="evenodd"/></svg>
                        <span>Tambah Rute</span>
                    </button>
                </div>
            </div>
            <div style="overflow-x:auto;">
                <table>
                    <thead>
                        <tr>
                            <th>ID Rute</th>
                            <th>Kota Asal</th>
                            <th>Kota Tujuan</th>
                            <th>Jarak</th>
                            <th>Reguler</th>
                            <th>Standar</th>
                            <th>Premium</th>
                            <th style="text-align:right;">Aksi</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${renderRuteRows(rute)}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

function renderRuteRows(list) {
    if (list.length === 0) return `<tr><td colspan="8" class="table-empty">Belum ada rute pengiriman.</td></tr>`;
    return list.map(r => `
        <tr>
            <td><span class="id-text">${r.id_rute}</span></td>
            <td>${r.kota_asal}</td>
            <td style="font-weight:600;">${r.tujuan}</td>
            <td>${r.jarak} km</td>
            <td>${r.biaya_reguler > 0 ? `Rp${r.biaya_reguler.toLocaleString("id-ID")} (${r.estimasi_reguler})` : '<span class="text-muted">-</span>'}</td>
            <td>${r.biaya_standar > 0 ? `Rp${r.biaya_standar.toLocaleString("id-ID")} (${r.estimasi_standar})` : '<span class="text-muted">-</span>'}</td>
            <td>${r.biaya_premium > 0 ? `Rp${r.biaya_premium.toLocaleString("id-ID")} (${r.estimasi_premium})` : '<span class="text-muted">-</span>'}</td>
            <td style="text-align:right;">
                <button class="btn btn-secondary btn-sm" onclick="showFormEditRute('${r.id_rute}')" style="margin-right:6px;">Edit</button>
                <button class="btn btn-danger btn-sm" onclick="deleteRute('${r.id_rute}')">Hapus</button>
            </td>
        </tr>
    `).join("");
}

function showFormAddRute() {
    const html = `
        <form id="form-add-rute" onsubmit="saveNewRute(event)">
            <div class="input-group">
                <label>ID Rute</label>
                <div class="input-wrapper">
                    <input type="text" id="add-r-id" placeholder="Contoh: R03" required>
                </div>
            </div>
            <div class="input-group">
                <label>Kota Asal</label>
                <div class="input-wrapper" style="opacity:0.6;">
                    <input type="text" value="Samarinda" readonly>
                </div>
            </div>
            <div class="input-group">
                <label>Kota Tujuan</label>
                <div class="input-wrapper">
                    <input type="text" id="add-r-tujuan" placeholder="Contoh: Balikpapan" required>
                </div>
            </div>
            <div class="input-group">
                <label>Jarak (km)</label>
                <div class="input-wrapper">
                    <input type="number" id="add-r-jarak" placeholder="Contoh: 120" min="1" required>
                </div>
            </div>
            <h4 style="margin:16px 0 8px; font-size:0.9rem; color:var(--accent);">Biaya & Estimasi Layanan</h4>
            <div class="input-row">
                <div class="input-group">
                    <label>Biaya Reguler (0 jika tidak ada)</label>
                    <div class="input-wrapper"><input type="number" id="add-r-reg-biaya" value="0" min="0"></div>
                </div>
                <div class="input-group">
                    <label>Estimasi Reguler</label>
                    <div class="input-wrapper"><input type="text" id="add-r-reg-est" value="-"></div>
                </div>
            </div>
            <div class="input-row">
                <div class="input-group">
                    <label>Biaya Standar</label>
                    <div class="input-wrapper"><input type="number" id="add-r-std-biaya" value="0" min="0"></div>
                </div>
                <div class="input-group">
                    <label>Estimasi Standar</label>
                    <div class="input-wrapper"><input type="text" id="add-r-std-est" value="-"></div>
                </div>
            </div>
            <div class="input-row">
                <div class="input-group">
                    <label>Biaya Premium</label>
                    <div class="input-wrapper"><input type="number" id="add-r-prem-biaya" value="0" min="0"></div>
                </div>
                <div class="input-group">
                    <label>Estimasi Premium</label>
                    <div class="input-wrapper"><input type="text" id="add-r-prem-est" value="-"></div>
                </div>
            </div>
            <div class="btn-row">
                <button type="button" class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button type="submit" class="btn btn-primary">Simpan Rute</button>
            </div>
        </form>
    `;
    showModal("Tambah Rute Pengiriman Baru", html);
}

function saveNewRute(e) {
    e.preventDefault();
    const id = document.getElementById("add-r-id").value.trim().toUpperCase();
    const tujuan = document.getElementById("add-r-tujuan").value.trim();
    const jarak = parseFloat(document.getElementById("add-r-jarak").value);

    const b_reg = parseFloat(document.getElementById("add-r-reg-biaya").value);
    const e_reg = document.getElementById("add-r-reg-est").value.trim();
    const b_std = parseFloat(document.getElementById("add-r-std-biaya").value);
    const e_std = document.getElementById("add-r-std-est").value.trim();
    const b_prem = parseFloat(document.getElementById("add-r-prem-biaya").value);
    const e_prem = document.getElementById("add-r-prem-est").value.trim();

    const rute = db.getRute();
    if (rute.some(r => r.id_rute === id)) return showToast(`ID Rute ${id} sudah ada.`, "error");

    rute.push({
        id_rute: id, kota_asal: "Samarinda", tujuan, jarak,
        biaya_reguler: b_reg, estimasi_reguler: e_reg,
        biaya_standar: b_std, estimasi_standar: e_std,
        biaya_premium: b_prem, estimasi_premium: e_prem
    });
    db.saveRute(rute);
    showToast(`Rute ke ${tujuan} berhasil ditambahkan!`, "success");
    closeModal();
    renderTabContent(activeTabName);
}

function showFormEditRute(id) {
    const ruteList = db.getRute();
    const r = ruteList.find(x => x.id_rute === id);
    if (!r) return showToast("Rute tidak ditemukan.", "error");

    const html = `
        <form id="form-edit-rute" onsubmit="saveEditedRute(event, '${id}')">
            <div class="input-group">
                <label>ID Rute</label>
                <div class="input-wrapper" style="opacity:0.6;">
                    <input type="text" value="${r.id_rute}" readonly>
                </div>
            </div>
            <div class="input-group">
                <label>Kota Tujuan</label>
                <div class="input-wrapper">
                    <input type="text" id="edit-r-tujuan" value="${r.tujuan}" required>
                </div>
            </div>
            <div class="input-group">
                <label>Jarak (km)</label>
                <div class="input-wrapper">
                    <input type="number" id="edit-r-jarak" value="${r.jarak}" min="1" required>
                </div>
            </div>
            <h4 style="margin:16px 0 8px; font-size:0.9rem; color:var(--accent);">Biaya & Estimasi Layanan</h4>
            <div class="input-row">
                <div class="input-group">
                    <label>Biaya Reguler</label>
                    <div class="input-wrapper"><input type="number" id="edit-r-reg-biaya" value="${r.biaya_reguler}" min="0"></div>
                </div>
                <div class="input-group">
                    <label>Estimasi Reguler</label>
                    <div class="input-wrapper"><input type="text" id="edit-r-reg-est" value="${r.estimasi_reguler}"></div>
                </div>
            </div>
            <div class="input-row">
                <div class="input-group">
                    <label>Biaya Standar</label>
                    <div class="input-wrapper"><input type="number" id="edit-r-std-biaya" value="${r.biaya_standar}" min="0"></div>
                </div>
                <div class="input-group">
                    <label>Estimasi Standar</label>
                    <div class="input-wrapper"><input type="text" id="edit-r-std-est" value="${r.estimasi_standar}"></div>
                </div>
            </div>
            <div class="input-row">
                <div class="input-group">
                    <label>Biaya Premium</label>
                    <div class="input-wrapper"><input type="number" id="edit-r-prem-biaya" value="${r.biaya_premium}" min="0"></div>
                </div>
                <div class="input-group">
                    <label>Estimasi Premium</label>
                    <div class="input-wrapper"><input type="text" id="edit-r-prem-est" value="${r.estimasi_premium}"></div>
                </div>
            </div>
            <div class="btn-row">
                <button type="button" class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button type="submit" class="btn btn-primary">Simpan Perubahan</button>
            </div>
        </form>
    `;
    showModal("Edit Rute Pengiriman", html);
}

function saveEditedRute(e, id) {
    e.preventDefault();
    const tujuan = document.getElementById("edit-r-tujuan").value.trim();
    const jarak = parseFloat(document.getElementById("edit-r-jarak").value);

    const b_reg = parseFloat(document.getElementById("edit-r-reg-biaya").value);
    const e_reg = document.getElementById("edit-r-reg-est").value.trim();
    const b_std = parseFloat(document.getElementById("edit-r-std-biaya").value);
    const e_std = document.getElementById("edit-r-std-est").value.trim();
    const b_prem = parseFloat(document.getElementById("edit-r-prem-biaya").value);
    const e_prem = document.getElementById("edit-r-prem-est").value.trim();

    const ruteList = db.getRute();
    const idx = ruteList.findIndex(r => r.id_rute === id);
    if (idx !== -1) {
        ruteList[idx] = { ...ruteList[idx], tujuan, jarak, biaya_reguler: b_reg, estimasi_reguler: e_reg, biaya_standar: b_std, estimasi_standar: e_std, biaya_premium: b_prem, estimasi_premium: e_prem };
        db.saveRute(ruteList);
        showToast("Perubahan data rute berhasil disimpan.", "success");
        closeModal();
        renderTabContent(activeTabName);
    }
}

function deleteRute(id) {
    if (confirm(`Apakah Anda yakin ingin menghapus rute dengan ID ${id}?`)) {
        const ruteList = db.getRute().filter(r => r.id_rute !== id);
        db.saveRute(ruteList);
        showToast("Rute telah dihapus.", "info");
        renderTabContent(activeTabName);
    }
}

// 4. ADMIN KONFIRMASI PESANAN
function renderAdminKonfirmasi(container) {
    const pesanan = db.getPesanan();
    const pending = pesanan.filter(p => p.status === "Menunggu Konfirmasi");

    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Konfirmasi Pesanan Masuk</h3>
            </div>
            <div style="overflow-x:auto;">
                <table>
                    <thead>
                        <tr>
                            <th>ID Transaksi</th>
                            <th>Nama Barang</th>
                            <th>Jumlah</th>
                            <th>Total Tagihan</th>
                            <th>Tipe Beli</th>
                            <th>Status</th>
                            <th style="text-align:right;">Tindakan</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${renderKonfirmasiRows(pending)}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

function renderKonfirmasiRows(list) {
    if (list.length === 0) return `<tr><td colspan="7" class="table-empty">Tidak ada pesanan baru yang menunggu konfirmasi.</td></tr>`;
    return list.map(p => `
        <tr>
            <td><span class="id-text">${p.id_pesanan}</span></td>
            <td style="font-weight:600;">${p.nama_barang}</td>
            <td>${p.jumlah} pcs</td>
            <td><span class="price-text">Rp${p.total_bayar.toLocaleString("id-ID")}</span></td>
            <td>${p.tipe_beli}</td>
            <td><span class="badge badge-yellow">${p.status}</span></td>
            <td style="text-align:right;">
                <button class="btn btn-success btn-sm" onclick="approveOrder('${p.id_pesanan}')" style="margin-right:6px;">Terima</button>
                <button class="btn btn-danger btn-sm" onclick="rejectOrder('${p.id_pesanan}')">Tolak</button>
            </td>
        </tr>
    `).join("");
}

function approveOrder(id) {
    const pesanan = db.getPesanan();
    const idx = pesanan.findIndex(p => p.id_pesanan === id);
    if (idx !== -1) {
        pesanan[idx].status = "Menunggu Pembayaran";
        db.savePesanan(pesanan);
        showToast(`Pesanan ${id} Diterima. Status berubah menjadi Menunggu Pembayaran.`, "success");
        renderTabContent(activeTabName);
    }
}

function rejectOrder(id) {
    if (confirm(`Apakah Anda yakin ingin menolak pesanan ${id}?`)) {
        const pesanan = db.getPesanan();
        const idx = pesanan.findIndex(p => p.id_pesanan === id);
        if (idx !== -1) {
            // Return stock back to inventory
            const barang = db.getBarang();
            const bIdx = barang.findIndex(b => b.nama_barang === pesanan[idx].nama_barang);
            if (bIdx !== -1) {
                barang[bIdx].stok += pesanan[idx].jumlah;
                db.saveBarang(barang);
            }

            pesanan[idx].status = "Ditolak";
            db.savePesanan(pesanan);
            showToast(`Pesanan ${id} ditolak.`, "info");
            renderTabContent(activeTabName);
        }
    }
}

// 5. ADMIN PEMBAYARAN & EKSPEDISI (LAYANAN PEMBAYARAN DAN PENGIRIMAN)
function renderAdminLayanan(container) {
    const pesanan = db.getPesanan();
    // Orders ready to be dispatched or logged (Lunas and Lunas & Dikirim status)
    const paidOrders = pesanan.filter(p => p.status === "Lunas" || p.status === "Lunas & Dikirim");

    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Layanan Ekspedisi & Logistik</h3>
            </div>
            <div style="overflow-x:auto;">
                <table>
                    <thead>
                        <tr>
                            <th>ID Transaksi</th>
                            <th>Nama Barang</th>
                            <th>Jumlah</th>
                            <th>Total Tagihan</th>
                            <th>Tipe Beli</th>
                            <th>Status Transaksi</th>
                            <th style="text-align:right;">Aksi Cargo</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${renderLayananRows(paidOrders)}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

function renderLayananRows(list) {
    if (list.length === 0) return `<tr><td colspan="7" class="table-empty">Belum ada pesanan lunas.</td></tr>`;
    return list.map(p => `
        <tr>
            <td><span class="id-text">${p.id_pesanan}</span></td>
            <td style="font-weight:600;">${p.nama_barang}</td>
            <td>${p.jumlah} pcs</td>
            <td><span class="price-text">Rp${p.total_bayar.toLocaleString("id-ID")}</span></td>
            <td>${p.tipe_beli}</td>
            <td><span class="badge ${p.status === 'Lunas & Dikirim' ? 'badge-blue' : 'badge-green'}">${p.status}</span></td>
            <td style="text-align:right;">
                ${p.status === 'Lunas' && p.tipe_beli.includes("Ekspedisi") ?
                    `<button class="btn btn-primary btn-sm" onclick="shipOrder('${p.id_pesanan}')">Kirim Barang</button>` :
                    p.status === 'Lunas' ? `<span style="font-size:0.8rem; color:var(--text-muted);">Siap Diambil Pembeli</span>` :
                    `<span style="font-size:0.8rem; color:var(--green); font-weight:600;">Dalam Pengiriman</span>`
                }
            </td>
        </tr>
    `).join("");
}

function shipOrder(id) {
    const pesanan = db.getPesanan();
    const idx = pesanan.findIndex(p => p.id_pesanan === id);
    if (idx !== -1) {
        pesanan[idx].status = "Lunas & Dikirim";
        db.savePesanan(pesanan);
        showToast(`Pesanan ${id} berhasil dikirim ke logistik cargo!`, "success");
        renderTabContent(activeTabName);
    }
}

// ================= CUSTOMER SECTIONS =================

// 1. CUSTOMER DASHBOARD
function renderCustomerDashboard(container) {
    const pesanan = db.getPesanan();
    // Just count total purchases of current user (matching their ordered items)
    // In our simplified app, the orders aren't hard-tied to usernames but we can display general customer counts.
    // To make it look extremely premium, let's filter pesanan context (since the C++ code has transactions that we can assume belong to the current active session user or just list them all)
    
    container.innerHTML = `
        <div class="dashboard-grid">
            <div class="stat-card">
                <div class="stat-icon orange"><svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 2a4 4 0 00-4 4v1H5a1 1 0 00-.994.89l-1 9A1 1 0 004 18h12a1 1 0 00.994-1.11l-1-9A1 1 0 0015 7h-1V6a4 4 0 00-4-4zm2 5V6a2 2 0 10-4 0v1h4zm-6 3a1 1 0 112 0 1 1 0 01-2 0zm7-1a1 1 0 100 2 1 1 0 000-2z" clip-rule="evenodd"/></svg></div>
                <div class="stat-label">Barang Tersedia</div>
                <div class="stat-value">${db.getBarang().filter(b => b.stok > 0).length} Macam</div>
            </div>
            <div class="stat-card">
                <div class="stat-icon cyan"><svg viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm1-12a1 1 0 10-2 0v4a1 1 0 00.293.707l2.8 2.8a1 1 0 101.414-1.414L11 9.586V6z" clip-rule="evenodd"/></svg></div>
                <div class="stat-label">Total Belanjaan Anda</div>
                <div class="stat-value">${pesanan.length} Pesanan</div>
            </div>
        </div>

        <div class="section-header">
            <h3>Selamat Datang di MOLOTOV, ${currentUser.nama_lengkap}!</h3>
        </div>
        <p style="color:var(--text-secondary); margin-bottom:20px; line-height:1.6;">
            Silakan pilih menu di sidebar untuk melihat katalog material, melakukan pemesanan baru, memproses pembayaran, atau melacak rute pengiriman cargo logistik.
        </p>

        <div class="quick-actions">
            <div class="quick-action-card" onclick="activeTab('customer-lihat')">
                <div class="qa-icon" style="background:var(--accent-glow); color:var(--accent)">
                    <svg viewBox="0 0 20 20" fill="currentColor"><path d="M10 12a2 2 0 100-4 2 2 0 000 4z"/></svg>
                </div>
                <div>
                    <div class="qa-text">Katalog Material</div>
                    <div class="qa-desc">Lihat harga & spesifikasi barang.</div>
                </div>
            </div>
            <div class="quick-action-card" onclick="activeTab('customer-pesan')">
                <div class="qa-icon" style="background:var(--green-bg); color:var(--green)">
                    <svg viewBox="0 0 20 20" fill="currentColor"><path d="M3 1a1 1 0 000 2h1.22l.305 1.222a.997.997 0 00.01.042l1.358 5.43"/></svg>
                </div>
                <div>
                    <div class="qa-text">Buat Pesanan</div>
                    <div class="qa-desc">Pesan & pilih ekspedisi cargo.</div>
                </div>
            </div>
        </div>
    `;
}

// 2. CUSTOMER LIHAT BARANG (WITH SORTING)
let currentSortMode = "default";
function renderCustomerLihatBarang(container) {
    const barang = db.getBarang();
    
    // Sort logic
    let sorted = [...barang];
    if (currentSortMode === "nama") {
        sorted.sort((a,b) => a.nama_barang.localeCompare(b.nama_barang));
    } else if (currentSortMode === "harga-asc") {
        sorted.sort((a,b) => a.harga - b.harga);
    } else if (currentSortMode === "harga-desc") {
        sorted.sort((a,b) => b.harga - a.harga);
    } else if (currentSortMode === "stok") {
        sorted.sort((a,b) => a.stok - b.stok);
    }

    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Katalog Material Bangunan</h3>
                <div class="table-actions">
                    <div class="select-wrapper">
                        <select onchange="sortCustomerBarang(this.value)">
                            <option value="default" ${currentSortMode === 'default' ? 'selected' : ''}>Tanpa Pengurutan</option>
                            <option value="nama" ${currentSortMode === 'nama' ? 'selected' : ''}>Nama Barang (A - Z)</option>
                            <option value="harga-asc" ${currentSortMode === 'harga-asc' ? 'selected' : ''}>Harga (Termurah - Termahal)</option>
                            <option value="harga-desc" ${currentSortMode === 'harga-desc' ? 'selected' : ''}>Harga (Termahal - Termurah)</option>
                            <option value="stok" ${currentSortMode === 'stok' ? 'selected' : ''}>Stok (Paling Sedikit)</option>
                        </select>
                    </div>
                </div>
            </div>
            <div style="overflow-x:auto;">
                <table>
                    <thead>
                        <tr>
                            <th>ID Barang</th>
                            <th>Nama Barang</th>
                            <th>Harga Satuan</th>
                            <th>Berat / Qty</th>
                            <th>Satuan</th>
                            <th>Stok Tersedia</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${sorted.map(b => `
                            <tr>
                                <td><span class="id-text">${b.id_barang}</span></td>
                                <td style="font-weight:600;">${b.nama_barang}</td>
                                <td><span class="price-text">Rp${b.harga.toLocaleString("id-ID")}</span></td>
                                <td>${b.berat} kg</td>
                                <td>${b.satuan}</td>
                                <td><span class="badge ${b.stok > 0 ? 'badge-green' : 'badge-red'}">${b.stok > 0 ? `${b.stok} unit` : 'Habis'}</span></td>
                            </tr>
                        `).join("")}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

function sortCustomerBarang(mode) {
    currentSortMode = mode;
    renderTabContent(activeTabName);
}

// 3. CUSTOMER PESAN BARANG
let selectedProductToOrder = null;
let selectedRouteToShip = null;
let selectedShippingTier = null;

function renderCustomerPesanBarang(container) {
    const barang = db.getBarang().filter(b => b.stok > 0);
    const rute = db.getRute();

    container.innerHTML = `
        <div style="max-width:680px; margin:0 auto;" class="glass-card">
            <div style="padding:28px;">
                <h3 style="margin-bottom:20px; font-weight:700; font-size:1.2rem;">Formulir Pemesanan Baru</h3>
                
                <div class="input-group">
                    <label>Pilih Barang</label>
                    <div class="select-wrapper">
                        <select id="order-product" onchange="updatePesanProductDetails(this.value)" required>
                            <option value="">-- Pilih Material --</option>
                            ${barang.map(b => `<option value="${b.id_barang}">${b.nama_barang} (Stok: ${b.stok} ${b.satuan}) - Rp${b.harga.toLocaleString("id-ID")}</option>`).join("")}
                        </select>
                    </div>
                </div>

                <div id="product-preview-box" style="display:none; padding:12px; background:var(--surface); border-radius:var(--radius-sm); margin-bottom:16px; font-size:0.9rem;">
                    <!-- Product details dynamic -->
                </div>

                <div class="input-group">
                    <label>Jumlah Pesanan</label>
                    <div class="input-wrapper">
                        <input type="number" id="order-qty" placeholder="Masukkan jumlah" min="1" oninput="calculateOrderTotal()" required>
                    </div>
                </div>

                <div class="input-group">
                    <label>Metode Pembelian</label>
                    <div class="select-wrapper">
                        <select id="order-method" onchange="updateOrderShippingSection(this.value)" required>
                            <option value="Ambil di Toko">Ambil Sendiri di Toko</option>
                            <option value="Kirim Ekspedisi">Kirim via Ekspedisi Cargo</option>
                        </select>
                    </div>
                </div>

                <!-- Shipping Rute & Service Selector -->
                <div id="shipping-details-section" style="display:none; border:1px solid var(--border); padding:16px; border-radius:var(--radius); margin-bottom:16px;">
                    <div class="input-group">
                        <label>Rute Pengiriman (Asal: Samarinda)</label>
                        <div class="select-wrapper">
                            <select id="order-route" onchange="updateShippingTiers(this.value)">
                                <option value="">-- Pilih Kota Tujuan --</option>
                                ${rute.map(r => `<option value="${r.id_rute}">${r.tujuan} (${r.jarak} km)</option>`).join("")}
                            </select>
                        </div>
                    </div>

                    <div class="input-group" id="shipping-tier-group" style="display:none;">
                        <label>Pilih Layanan & Ongkir</label>
                        <div class="select-wrapper">
                            <select id="order-shipping-tier" onchange="calculateOrderTotal()">
                                <!-- Dynamic options -->
                            </select>
                        </div>
                    </div>
                </div>

                <div style="background:var(--accent-glow); border-radius:var(--radius); padding:16px; display:flex; align-items:center; justify-content:between; margin-bottom:20px;">
                    <div>
                        <div style="font-size:0.8rem; color:var(--text-secondary); text-transform:uppercase; letter-spacing:0.5px;">Total Tagihan</div>
                        <div style="font-size:1.4rem; font-weight:800; color:var(--accent-light);" id="order-grand-total">Rp0</div>
                    </div>
                </div>

                <button class="btn btn-primary btn-block" onclick="submitCustomerOrder()">
                    <span>Konfirmasi Buat Pesanan</span>
                </button>
            </div>
        </div>
    `;
}

function updatePesanProductDetails(id) {
    const box = document.getElementById("product-preview-box");
    if (!id) {
        box.style.display = "none";
        selectedProductToOrder = null;
        calculateOrderTotal();
        return;
    }
    const item = db.getBarang().find(b => b.id_barang === id);
    selectedProductToOrder = item;
    box.style.display = "block";
    box.innerHTML = `
        <strong>Detail Material:</strong><br>
        • ID: ${item.id_barang}<br>
        • Harga: Rp${item.harga.toLocaleString("id-ID")} per ${item.satuan}<br>
        • Berat Satuan: ${item.berat} kg<br>
        • Stok Tersedia: ${item.stok} unit
    `;
    document.getElementById("order-qty").max = item.stok;
    calculateOrderTotal();
}

function updateOrderShippingSection(method) {
    const section = document.getElementById("shipping-details-section");
    if (method === "Kirim Ekspedisi") {
        section.style.display = "block";
    } else {
        section.style.display = "none";
        document.getElementById("order-route").value = "";
        document.getElementById("shipping-tier-group").style.display = "none";
        selectedRouteToShip = null;
        selectedShippingTier = null;
    }
    calculateOrderTotal();
}

function updateShippingTiers(routeId) {
    const tierGroup = document.getElementById("shipping-tier-group");
    const tierSelect = document.getElementById("order-shipping-tier");
    tierSelect.innerHTML = "";

    if (!routeId) {
        tierGroup.style.display = "none";
        selectedRouteToShip = null;
        calculateOrderTotal();
        return;
    }

    const route = db.getRute().find(r => r.id_rute === routeId);
    selectedRouteToShip = route;
    tierGroup.style.display = "block";

    let hasAnyService = false;
    if (route.biaya_reguler > 0) {
        tierSelect.innerHTML += `<option value="reguler" data-cost="${route.biaya_reguler}">Reguler (Rp${route.biaya_reguler.toLocaleString("id-ID")}) - ${route.estimasi_reguler}</option>`;
        hasAnyService = true;
    }
    if (route.biaya_standar > 0) {
        tierSelect.innerHTML += `<option value="standar" data-cost="${route.biaya_standar}">Standar (Rp${route.biaya_standar.toLocaleString("id-ID")}) - ${route.estimasi_standar}</option>`;
        hasAnyService = true;
    }
    if (route.biaya_premium > 0) {
        tierSelect.innerHTML += `<option value="premium" data-cost="${route.biaya_premium}">Premium (Rp${route.biaya_premium.toLocaleString("id-ID")}) - ${route.estimasi_premium}</option>`;
        hasAnyService = true;
    }

    if (!hasAnyService) {
        tierSelect.innerHTML = `<option value="">Tidak ada pengiriman tersedia</option>`;
    }
    calculateOrderTotal();
}

function calculateOrderTotal() {
    let grand = 0;
    const qtyInput = document.getElementById("order-qty");
    const qty = parseInt(qtyInput.value) || 0;

    if (selectedProductToOrder && qty > 0) {
        grand += selectedProductToOrder.harga * qty;
    }

    const method = document.getElementById("order-method").value;
    if (method === "Kirim Ekspedisi" && selectedRouteToShip) {
        const tierSelect = document.getElementById("order-shipping-tier");
        const activeOption = tierSelect.options[tierSelect.selectedIndex];
        if (activeOption) {
            const cost = parseFloat(activeOption.getAttribute("data-cost")) || 0;
            grand += cost;
        }
    }

    document.getElementById("order-grand-total").innerText = `Rp${grand.toLocaleString("id-ID")}`;
}

function submitCustomerOrder() {
    if (!selectedProductToOrder) return showToast("Silakan pilih material terlebih dahulu.", "error");

    const qty = parseInt(document.getElementById("order-qty").value);
    if (!qty || qty <= 0) return showToast("Jumlah pesanan tidak valid.", "error");
    if (qty > selectedProductToOrder.stok) return showToast(`Jumlah melebihi stok yang ada (${selectedProductToOrder.stok}).`, "error");

    const method = document.getElementById("order-method").value;
    let grandTotal = selectedProductToOrder.harga * qty;
    let finalMethod = method;

    if (method === "Kirim Ekspedisi") {
        if (!selectedRouteToShip) return showToast("Silakan pilih kota rute tujuan logistik.", "error");
        const tierSelect = document.getElementById("order-shipping-tier");
        const activeOption = tierSelect.options[tierSelect.selectedIndex];
        if (!activeOption) return showToast("Layanan pengiriman tidak tersedia.", "error");
        
        const cost = parseFloat(activeOption.getAttribute("data-cost")) || 0;
        grandTotal += cost;
        finalMethod = `Diantar (${activeOption.innerText.split('(')[0].trim()})`;
    } else {
        finalMethod = "Ditempat";
    }

    if (confirm(`Apakah Anda yakin ingin memesan ${qty} ${selectedProductToOrder.satuan} ${selectedProductToOrder.nama_barang} dengan total tagihan Rp${grandTotal.toLocaleString("id-ID")}?`)) {
        // Decrement stock
        const barang = db.getBarang();
        const bIdx = barang.findIndex(b => b.id_barang === selectedProductToOrder.id_barang);
        if (bIdx !== -1) {
            barang[bIdx].stok -= qty;
            db.saveBarang(barang);
        }

        // Save order
        const pesanan = db.getPesanan();
        const nextTrxId = `TRX${String(pesanan.length + 1).padStart(3, '0')}`;
        pesanan.push({
            id_pesanan: nextTrxId,
            nama_barang: selectedProductToOrder.nama_barang,
            jumlah: qty,
            total_bayar: grandTotal,
            tipe_beli: finalMethod,
            status: "Menunggu Konfirmasi"
        });
        db.savePesanan(pesanan);

        showToast(`Pesanan berhasil dibuat! Kode: ${nextTrxId}`, "success");
        activeTab("customer-status");
    }
}

// 4. CUSTOMER STATUS PESANAN
function renderCustomerStatus(container) {
    const pesanan = db.getPesanan();
    
    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Status Pelacakan Pesanan</h3>
            </div>
            <div style="overflow-x:auto;">
                <table>
                    <thead>
                        <tr>
                            <th>ID Transaksi</th>
                            <th>Nama Barang</th>
                            <th>Qty</th>
                            <th>Total Tagihan</th>
                            <th>Metode</th>
                            <th>Status Transaksi</th>
                            <th style="text-align:right;">Aksi</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${pesanan.map(p => {
                            let badgeClass = "badge-yellow";
                            if (p.status === "Menunggu Pembayaran") badgeClass = "badge-cyan";
                            if (p.status === "Lunas") badgeClass = "badge-green";
                            if (p.status === "Lunas & Dikirim") badgeClass = "badge-blue";
                            if (p.status === "Ditolak") badgeClass = "badge-red";

                            return `
                                <tr>
                                    <td><span class="id-text">${p.id_pesanan}</span></td>
                                    <td style="font-weight:600;">${p.nama_barang}</td>
                                    <td>${p.jumlah} pcs</td>
                                    <td><span class="price-text">Rp${p.total_bayar.toLocaleString("id-ID")}</span></td>
                                    <td>${p.tipe_beli}</td>
                                    <td><span class="badge ${badgeClass}">${p.status}</span></td>
                                    <td style="text-align:right;">
                                        <button class="btn btn-secondary btn-sm" onclick="showCustomerOrderDetail('${p.id_pesanan}')">Detail</button>
                                    </td>
                                </tr>
                            `;
                        }).join("")}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

function showCustomerOrderDetail(id) {
    const p = db.getPesanan().find(x => x.id_pesanan === id);
    if (!p) return;

    let alertMsg = "";
    if (p.status === "Menunggu Konfirmasi") {
        alertMsg = `<div style="background:var(--yellow-bg); color:var(--yellow); padding:10px; border-radius:var(--radius-sm); font-size:0.85rem; margin-top:12px;">Pesanan Anda sedang menunggu disetujui Admin.</div>`;
    } else if (p.status === "Menunggu Pembayaran") {
        alertMsg = `
            <div style="background:var(--cyan-bg); color:var(--cyan); padding:10px; border-radius:var(--radius-sm); font-size:0.85rem; margin-top:12px; margin-bottom:12px;">Silakan lakukan pembayaran agar pesanan segera dikirim.</div>
            <button class="btn btn-primary btn-block" onclick="closeModal(); activeTab('customer-bayar');">Bayar Sekarang</button>
        `;
    } else if (p.status === "Lunas") {
        alertMsg = `<div style="background:var(--green-bg); color:var(--green); padding:10px; border-radius:var(--radius-sm); font-size:0.85rem; margin-top:12px;">Pembayaran Berhasil. Silakan ambil di toko / tunggu logistik cargo memproses.</div>`;
    } else if (p.status === "Lunas & Dikirim") {
        alertMsg = `<div style="background:var(--blue-bg); color:var(--blue); padding:10px; border-radius:var(--radius-sm); font-size:0.85rem; margin-top:12px;">Barang sedang dalam perjalanan logistik ke alamat Anda.</div>`;
    } else if (p.status === "Ditolak") {
        alertMsg = `<div style="background:var(--red-bg); color:var(--red); padding:10px; border-radius:var(--radius-sm); font-size:0.85rem; margin-top:12px;">Pesanan Anda ditolak. Silakan hubungi admin.</div>`;
    }

    const html = `
        <div style="font-size:0.95rem;">
            <p style="margin-bottom:8px;"><strong>ID Pesanan:</strong> <span class="id-text">${p.id_pesanan}</span></p>
            <p style="margin-bottom:8px;"><strong>Material:</strong> ${p.nama_barang}</p>
            <p style="margin-bottom:8px;"><strong>Jumlah:</strong> ${p.jumlah} pcs</p>
            <p style="margin-bottom:8px;"><strong>Metode:</strong> ${p.tipe_beli}</p>
            <p style="margin-bottom:8px;"><strong>Total Belanja:</strong> <span class="price-text">Rp${p.total_bayar.toLocaleString("id-ID")}</span></p>
            <p><strong>Status Transaksi:</strong> <span class="badge badge-yellow">${p.status}</span></p>
            ${alertMsg}
        </div>
    `;
    showModal("Detail Pesanan Pelanggan", html);
}

// 5. CUSTOMER BAYAR PESANAN
function renderCustomerBayar(container) {
    const pesanan = db.getPesanan();
    const unpaid = pesanan.filter(p => p.status === "Menunggu Pembayaran");

    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Bayar Tagihan Pesanan</h3>
            </div>
            <div style="overflow-x:auto;">
                <table>
                    <thead>
                        <tr>
                            <th>ID Transaksi</th>
                            <th>Nama Barang</th>
                            <th>Qty</th>
                            <th>Total Tagihan</th>
                            <th>Status</th>
                            <th style="text-align:right;">Aksi</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${unpaid.map(p => `
                            <tr>
                                <td><span class="id-text">${p.id_pesanan}</span></td>
                                <td style="font-weight:600;">${p.nama_barang}</td>
                                <td>${p.jumlah} pcs</td>
                                <td><span class="price-text">Rp${p.total_bayar.toLocaleString("id-ID")}</span></td>
                                <td><span class="badge badge-cyan">${p.status}</span></td>
                                <td style="text-align:right;">
                                    <button class="btn btn-primary btn-sm" onclick="payWithCashForm('${p.id_pesanan}')">Bayar</button>
                                </td>
                            </tr>
                        `).join("")}
                        ${unpaid.length === 0 ? '<tr><td colspan="6" class="table-empty">Tidak ada tagihan yang siap dibayar saat ini.</td></tr>' : ''}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

function payWithCashForm(id) {
    const p = db.getPesanan().find(x => x.id_pesanan === id);
    if (!p) return;

    const html = `
        <form onsubmit="processOrderPayment(event, '${id}')">
            <div style="font-size:0.95rem; margin-bottom:16px;">
                <p style="margin-bottom:6px;"><strong>ID Pesanan:</strong> <span class="id-text">${p.id_pesanan}</span></p>
                <p style="margin-bottom:6px;"><strong>Material:</strong> ${p.nama_barang}</p>
                <p style="margin-bottom:6px;"><strong>Tagihan:</strong> <span class="price-text">Rp${p.total_bayar.toLocaleString("id-ID")}</span></p>
            </div>
            <div class="input-group">
                <label>Nominal Pembayaran (Rp)</label>
                <div class="input-wrapper">
                    <input type="number" id="cash-input" placeholder="Masukkan jumlah uang" min="${p.total_bayar}" required>
                </div>
            </div>
            <div class="btn-row">
                <button type="button" class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button type="submit" class="btn btn-primary">Konfirmasi Pembayaran</button>
            </div>
        </form>
    `;
    showModal("Proses Pembayaran", html);
}

function processOrderPayment(e, id) {
    e.preventDefault();
    const cash = parseFloat(document.getElementById("cash-input").value);
    const pesanan = db.getPesanan();
    const idx = pesanan.findIndex(p => p.id_pesanan === id);

    if (idx !== -1) {
        const cost = pesanan[idx].total_bayar;
        if (cash < cost) {
            return showToast("Uang tidak cukup.", "error");
        }
        const change = cash - cost;
        pesanan[idx].status = "Lunas";
        db.savePesanan(pesanan);

        showToast("Pembayaran berhasil!", "success");
        closeModal();

        // Show invoice details with change
        const html = `
            <div style="font-size:0.95rem; text-align:center; padding:10px;">
                <div style="color:var(--green); font-size:2rem; margin-bottom:12px;">✓</div>
                <h4 style="margin-bottom:14px;">Pembayaran Lunas!</h4>
                <p style="margin-bottom:6px;">Uang Diterima: <strong>Rp${cash.toLocaleString("id-ID")}</strong></p>
                <p style="margin-bottom:16px;">Kembalian: <strong style="color:var(--green);">Rp${change.toLocaleString("id-ID")}</strong></p>
                <button class="btn btn-primary" onclick="closeModal(); activeTab('customer-status');">Selesai</button>
            </div>
        `;
        setTimeout(() => {
            showModal("Kuitansi Pembayaran", html);
        }, 300);

        renderTabContent(activeTabName);
    }
}

// 6. CUSTOMER RIWAYAT TRANSAKSI
function renderCustomerRiwayat(container) {
    const pesanan = db.getPesanan();

    container.innerHTML = `
        <div class="table-container">
            <div class="table-header">
                <h3>Riwayat Transaksi Belanja</h3>
            </div>
            <div style="overflow-x:auto;">
                <table>
                    <thead>
                        <tr>
                            <th>ID Transaksi</th>
                            <th>Nama Barang</th>
                            <th>Jumlah</th>
                            <th>Total Belanja</th>
                            <th>Tipe Beli</th>
                            <th>Status Transaksi</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${pesanan.map(p => `
                            <tr>
                                <td><span class="id-text">${p.id_pesanan}</span></td>
                                <td style="font-weight:600;">${p.nama_barang}</td>
                                <td>${p.jumlah} pcs</td>
                                <td><span class="price-text">Rp${p.total_bayar.toLocaleString("id-ID")}</span></td>
                                <td>${p.tipe_beli}</td>
                                <td><span class="badge ${p.status === 'Lunas & Dikirim' || p.status === 'Lunas' ? 'badge-green' : p.status === 'Ditolak' ? 'badge-red' : 'badge-yellow'}">${p.status}</span></td>
                            </tr>
                        `).join("")}
                        ${pesanan.length === 0 ? '<tr><td colspan="6" class="table-empty">Belum ada riwayat transaksi belanja.</td></tr>' : ''}
                    </tbody>
                </table>
            </div>
        </div>
    `;
}

// ===== REAL-TIME SYNC ACROSS TABS =====
window.addEventListener('storage', (e) => {
    if (e.key && e.key.startsWith('molotov_')) {
        if (currentUser && document.getElementById("app-screen").classList.contains("active")) {
            // Re-render the active tab when data in localStorage changes from another tab
            renderTabContent(activeTabName);
        }
    }
});

// ===== RESTORE SESSION ON PAGE LOAD =====
window.addEventListener('DOMContentLoaded', () => {
    const savedUser = sessionStorage.getItem("molotov_current_user");
    if (savedUser) {
        currentUser = JSON.parse(savedUser);
        enterApp();
        const savedTab = sessionStorage.getItem("molotov_active_tab");
        if (savedTab) {
            // We use setTimeout to ensure sidebar navigation items are fully rendered first
            setTimeout(() => activeTab(savedTab), 50);
        }
    }
});
