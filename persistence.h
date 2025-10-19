#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>

// Exposed for main and other modules
void loadData();
void saveData();

// individual helpers (if needed elsewhere)
void saveBuyers();
void loadBuyers();
void saveSellers();
void loadSellers();
void saveInventory();
void loadInventory();
void saveOrders();
void loadOrders();

// file names (if someone needs them)
extern const std::string BUYERS_FILE;
extern const std::string SELLERS_FILE;
extern const std::string INVENTORY_FILE;
extern const std::string ORDERS_FILE;

#endif // PERSISTENCE_H
