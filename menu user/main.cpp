#include "shop.h"

int main() {
    std::vector<Item> dbItems;
    std::vector<Route> dbRoutes;
    std::vector<Transaction> dbTransactions;

    std::vector<Item>* ptrItems = &dbItems;
    std::vector<Route>* ptrRoutes = &dbRoutes;
    std::vector<Transaction>* ptrTrxs = &dbTransactions;

    initData(ptrItems, ptrRoutes, ptrTrxs);
    userMenu(ptrItems, ptrRoutes, ptrTrxs);

    return 0;
}