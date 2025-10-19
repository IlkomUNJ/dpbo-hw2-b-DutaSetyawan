#ifndef UI_H
#define UI_H

#include "buyer.h"
#include "seller.h"

void clearInputBuffer();
void registerUser();
void loginUser();
void buyerMenu(Buyer& buyer);
void checkAccountStatus(Buyer& buyer);
void upgradeToSeller(Buyer& buyer);
void bankingFunctions(Buyer& buyer);
void browseStore(Buyer& buyer);
void orderMenu(Buyer& buyer);
void paymentMenu(Buyer& buyer);
bool deleteAccount(Buyer& buyer);
void sellerMenu(seller& currentSeller);
void addInventory(seller& currentSeller);
void removeInventory(seller& currentSeller);
void viewSellerOrders(seller& currentSeller);

#endif // UI_H
