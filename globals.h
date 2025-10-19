#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include "bank.h"
#include "buyer.h"
#include "seller.h"
#include "order.h"

// Global objects defined in main.cpp
extern Bank eshopBank;
extern std::vector<Buyer> buyers;
extern std::vector<seller> sellers;
extern std::vector<Order> orders;

#endif // GLOBALS_H
