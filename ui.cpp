#include "ui.h"
#include "globals.h"
#include "persistence.h"
#include <iostream>
#include <limits>
using namespace std;

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void registerUser() {
    string name, address, phone, email;
    cout << "\n--- New User Registration ---" << endl;
    cout << "Enter your full name: "; getline(cin, name);
    cout << "Enter your address: "; getline(cin, address);
    cout << "Enter your phone number: "; getline(cin, phone);
    cout << "Enter your email: "; getline(cin, email);
    BankCustomer& newAccountRef = eshopBank.createAccount(name, address, phone, email);
    Buyer newBuyer(newAccountRef.getId(), name, &newAccountRef);
    buyers.push_back(newBuyer);
    cout << "Buyer account created successfully! Your User ID is " << newAccountRef.getId() << "." << endl;
    char createSeller; cout << "Do you want to create a Seller account as well? (y/n): "; cin >> createSeller; clearInputBuffer();
    if (createSeller == 'y' || createSeller == 'Y') {
        string sName, sAddr, sPhone, sMail;
        cout << "\n--- Seller Account Details ---" << endl;
        cout << "Enter your store name: "; getline(cin, sName);
        cout << "Enter your store address: "; getline(cin, sAddr);
        cout << "Enter your store phone number: "; getline(cin, sPhone);
        cout << "Enter your store email: "; getline(cin, sMail);
        seller newSeller(buyers.back(), sellers.size() + 1, sName, sAddr, sPhone, sMail);
        sellers.push_back(newSeller);
        saveBuyers(); saveSellers(); saveInventory();
        cout << "Seller account created. Logging you in..." << endl;
        sellerMenu(sellers.back());
    } else {
        saveBuyers();
        cout << "Registration complete. Logging you in..." << endl;
        buyerMenu(buyers.back());
    }
}

void loginUser() {
    int id; cout << "\n--- Login ---" << endl; cout << "Enter your user ID: "; cin >> id;
    if (cin.fail()) { cout << "Invalid ID format." << endl; cin.clear(); clearInputBuffer(); return; }
    clearInputBuffer();
    for (seller& s : sellers) { if (s.getId() == id) { cout << "\nLogin successful. Welcome, " << s.getName() << " (Seller)!"; sellerMenu(s); return; } }
    for (Buyer& b : buyers) { if (b.getId() == id) { cout << "\nLogin successful. Welcome, " << b.getName() << " (Buyer)!"; buyerMenu(b); return; } }
    cout << "Login failed. User ID not found." << endl;
}

void buyerMenu(Buyer& buyer) {
    int choice; bool accountDeleted = false;
    do {
        cout << "\n\n--- Buyer Menu ---" << endl;
        cout << "1. Check Account Status\n2. Upgrade Account to Seller\n3. Banking Functions\n4. Browse Store\n5. View Cart & Order\n6. Payment\n7. Delete Account\n8. Logout\nEnter your choice: ";
        cin >> choice; if (cin.fail()) { cout << "Invalid input." << endl; cin.clear(); clearInputBuffer(); choice = 0; } else { clearInputBuffer(); }
        switch (choice) {
            case 1: checkAccountStatus(buyer); break;
            case 2: upgradeToSeller(buyer); break;
            case 3: bankingFunctions(buyer); break;
            case 4: browseStore(buyer); break;
            case 5: orderMenu(buyer); break;
            case 6: paymentMenu(buyer); break;
            case 7: accountDeleted = deleteAccount(buyer); if (accountDeleted) { choice = 8; } break;
            case 8: { char confirm; cout << "Are you sure you want to logout? (y/n): "; cin >> confirm; clearInputBuffer(); if (confirm == 'y' || confirm == 'Y') { cout << "Logging out..." << endl; } else { choice = 0; } break; }
            default: cout << "Invalid option." << endl; break;
        }
    } while (choice != 8 && !accountDeleted);
}

bool deleteAccount(Buyer& buyer) {
    cout << "\n--- Delete Account ---" << endl << "WARNING: This action is irreversible and will delete all your data." << endl;
    char confirm; cout << "Are you sure you want to delete your account? (y/n): "; cin >> confirm; clearInputBuffer();
    if (confirm == 'y' || confirm == 'Y') {
        int userId = buyer.getId();
        auto seller_it = remove_if(sellers.begin(), sellers.end(), [userId](const seller& s) { return s.getId() == userId; });
        if (seller_it != sellers.end()) sellers.erase(seller_it, sellers.end());
        auto buyer_it = remove_if(buyers.begin(), buyers.end(), [userId](const Buyer& b) { return b.getId() == userId; });
        if (buyer_it != buyers.end()) buyers.erase(buyer_it, buyers.end());
        eshopBank.deleteAccountById(userId); saveBuyers(); saveSellers();
        cout << "Account has been successfully deleted. Returning to main menu." << endl; return true;
    } else { cout << "Account deletion cancelled. Returning to your menu." << endl; return false; }
}

void checkAccountStatus(Buyer& buyer) {
    cout << "\n--- Account Status ---" << endl;
    cout << "User ID: " << buyer.getId() << "\nName: " << buyer.getName() << "\nRole: Buyer"; bool isSeller = false;
    for (const auto& s : sellers) { if (s.getId() == buyer.getId()) { cout << " & Seller" << endl << "\n--- Store Details ---" << endl << "Store Name: " << s.getStoreName() << "\nStore Address: " << s.getStoreAddress() << "\nStore Phone: " << s.getStorePhoneNumber() << "\nStore Email: " << s.getStoreEmail() << endl; isSeller = true; break; } }
    if (!isSeller) {
        cout << endl;
    }
    cout << "\n--- Banking Details ---" << endl;
    buyer.getAccount()->printInfo();
}

void upgradeToSeller(Buyer& buyer) {
    if (buyer.getAccount()->getBalance() <= 0) { cout << "Upgrade failed. You must have a positive balance." << endl; return; }
    for (const auto& s : sellers) { if (s.getId() == buyer.getId()) { cout << "You are already a seller." << endl; return; } }
    string sName, sAddr, sPhone, sMail; cout << "\n--- Upgrade to Seller Account ---" << endl;
    cout << "Enter your store name: "; getline(cin, sName);
    cout << "Enter your store address: "; getline(cin, sAddr);
    cout << "Enter your store phone number: "; getline(cin, sPhone);
    cout << "Enter your store email: "; getline(cin, sMail);
    seller newSeller(buyer, sellers.size() + 1, sName, sAddr, sPhone, sMail);
    sellers.push_back(newSeller); cout << "Congratulations! You are now a seller." << endl << "Please log out and log in again to access the seller menu." << endl;
}

void bankingFunctions(Buyer& buyer) {
    int choice; do { cout << "\n--- Banking Functions ---" << endl << "Current Balance: $" << buyer.getAccount()->getBalance() << endl << "1. Deposit\n2. Withdraw\n3. View Transaction History\n4. Return to Buyer Menu\nEnter your choice: "; cin >> choice; if (cin.fail()) { cout << "Invalid input." << endl; cin.clear(); clearInputBuffer(); choice = 0; } else { clearInputBuffer(); }
        switch (choice) { case 1: { double amount; cout << "Enter amount to deposit: $"; cin >> amount; if (!cin.fail() && amount > 0) { buyer.getAccount()->addBalance(amount); cout << "Deposit successful." << endl; saveBuyers(); } else { cout << "Invalid amount." << endl; } clearInputBuffer(); break; } case 2: { double amount; cout << "Enter amount to withdraw: $"; cin >> amount; if (!cin.fail() && amount > 0) { buyer.getAccount()->withdrawBalance(amount); saveBuyers(); } else { cout << "Invalid amount." << endl; } clearInputBuffer(); break; } case 3: buyer.getAccount()->printTransactionHistory(); break; case 4: break; default: cout << "Invalid option." << endl; break; } } while (choice != 4);
}

void browseStore(Buyer& buyer) {
    if (sellers.empty()) { cout << "\nThere are no stores available at the moment." << endl; return; }
    cout << "\n--- Available Stores ---" << endl; for (const auto& s : sellers) { cout << s.getSellerId() << ". " << s.getStoreName() << endl; }
    int storeId; cout << "Select a store to browse (enter ID): "; cin >> storeId; clearInputBuffer(); seller* selectedSeller = nullptr; for (auto& s : sellers) { if (s.getSellerId() == storeId) { selectedSeller = &s; break; } }
    if (selectedSeller) { selectedSeller->viewItems(); if (!selectedSeller->getItems().empty()) { int itemId; cout << "Enter the ID of the item to add to cart (or 0 to go back): "; cin >> itemId; clearInputBuffer(); if (itemId != 0) { bool itemFound = false; for (const auto& item : selectedSeller->getItems()) { if (item.getId() == itemId) { buyer.getCart().addItem(item, selectedSeller->getSellerId()); itemFound = true; break; } } if (!itemFound) { cout << "Item ID not found in this store." << endl; } } } } else { cout << "Invalid store ID." << endl; }
}

void orderMenu(Buyer& buyer) {
    if (buyer.getCart().isEmpty()) { cout << "\nYour cart is empty." << endl; return; }
    int choice; do { buyer.getCart().viewCart(); cout << "\n--- Order Options ---" << endl << "1. Checkout\n2. Remove an Item from Cart\n3. Back to Menu\nEnter your choice: "; cin >> choice; if (cin.fail()) { cout << "Invalid input." << endl; cin.clear(); clearInputBuffer(); choice = 0; } else { clearInputBuffer(); }
        switch (choice) { case 1: { vector<Item> itemsInCart; int sellerId = -1; if (!buyer.getCart().getItems().empty()) { sellerId = buyer.getCart().getItems()[0].second; } for(const auto& pair : buyer.getCart().getItems()){ itemsInCart.push_back(pair.first); } int newOrderId = orders.size() + 1; Order newOrder(newOrderId, buyer.getId(), sellerId, itemsInCart); orders.push_back(newOrder); saveOrders(); buyer.getCart().clearCart(); cout << "Checkout successful! An invoice has been generated." << endl << "Your Order ID is " << newOrderId << ". Please proceed to Payment menu." << endl; return; } case 2: { if (buyer.getCart().isEmpty()) { cout << "Your cart is already empty." << endl; break; } int itemIdToRemove; cout << "Enter the ID of the item to remove: "; cin >> itemIdToRemove; clearInputBuffer(); buyer.getCart().removeItem(itemIdToRemove); break; } case 3: break; default: cout << "Invalid option." << endl; break; } } while (choice != 3);
}

void paymentMenu(Buyer& buyer) {
    cout << "\n--- Your Invoices ---" << endl; bool hasInvoice = false; for (const auto& order : orders) { if (order.getBuyerId() == buyer.getId() && order.getStatus() == UNPAID) { cout << "Order ID: " << order.getOrderId() << ", Total: $" << order.getTotalAmount() << ", Status: " << order.getStatusString() << endl; hasInvoice = true; } }
    if (!hasInvoice) { cout << "You have no unpaid invoices." << endl; return; }
    int orderId; cout << "Enter Order ID to pay: "; cin >> orderId; clearInputBuffer(); Order* orderToPay = nullptr; for (auto& o : orders) { if (o.getOrderId() == orderId && o.getBuyerId() == buyer.getId()) { orderToPay = &o; break; } }
    if (orderToPay && orderToPay->getStatus() == UNPAID) { double total = orderToPay->getTotalAmount(); double balance = buyer.getAccount()->getBalance(); cout << "Invoice Total: $" << total << endl << "Your Balance: $" << balance << endl; if (balance >= total) { char confirm; cout << "Confirm payment? (y/n): "; cin >> confirm; clearInputBuffer(); if (confirm == 'y' || confirm == 'Y') { buyer.getAccount()->withdrawBalance(total); for (auto& s : sellers) { if (s.getSellerId() == orderToPay->getSellerId()) { s.getAccount()->addBalance(total); break; } } orderToPay->setStatus(PAID); cout << "Payment successful! Your order status is now PAID." << endl; saveBuyers(); saveSellers(); saveOrders(); } else { cout << "Payment cancelled." << endl; } } else { cout << "Payment failed. Insufficient balance." << endl; } } else { cout << "Invalid Order ID or order is already paid." << endl; }
}

void sellerMenu(seller& currentSeller) {
    int choice; do { cout << "\n\n--- Seller Menu ---" << endl << "1. Add Item to Inventory\n2. Remove Item from Inventory\n3. Check Inventory\n4. View Orders\n5. Logout\nEnter your choice: "; cin >> choice; if (cin.fail()) { cout << "Invalid input." << endl; cin.clear(); clearInputBuffer(); choice = 0; } else { clearInputBuffer(); }
        switch (choice) { case 1: addInventory(currentSeller); break; case 2: removeInventory(currentSeller); break; case 3: currentSeller.viewItems(); break; case 4: viewSellerOrders(currentSeller); break; case 5: { char confirm; cout << "Are you sure you want to logout? (y/n): "; cin >> confirm; clearInputBuffer(); if (confirm == 'y' || confirm == 'Y') { cout << "Logging out..." << endl; } else { choice = 0; } break; } default: cout << "Invalid option." << endl; break; } } while (choice != 5);
}

void addInventory(seller& currentSeller) {
    int id, quantity; string name; double price; cout << "\n--- Add New Item ---" << endl; cout << "Enter Item ID: "; cin >> id; clearInputBuffer(); cout << "Enter Item Name: "; getline(cin, name); cout << "Enter Quantity: "; cin >> quantity; cout << "Enter Price: $"; cin >> price; clearInputBuffer(); currentSeller.addNewItem(id, name, quantity, price); cout << "Item '" << name << "' added to inventory." << endl; saveInventory();
}

void removeInventory(seller& currentSeller) {
    currentSeller.viewItems(); if (currentSeller.getItems().empty()) return; int itemId; cout << "Enter the ID of the item to remove: "; cin >> itemId; clearInputBuffer(); currentSeller.removeItem(itemId); saveInventory();
}

void viewSellerOrders(seller& currentSeller) {
    cout << "\n--- Orders for Your Store ---" << endl; bool hasOrders = false; for (const auto& order : orders) { if (order.getSellerId() == currentSeller.getSellerId() && order.getStatus() == PAID) { cout << "Order ID: " << order.getOrderId() << ", Buyer ID: " << order.getBuyerId() << ", Total: $" << order.getTotalAmount() << ", Status: " << order.getStatusString() << endl; cout << "  Items:" << endl; for(const auto& item : order.getItems()){ cout << "    - " << item.getName() << endl; } hasOrders = true; } } if (!hasOrders) { cout << "You have no paid orders." << endl; }
}
