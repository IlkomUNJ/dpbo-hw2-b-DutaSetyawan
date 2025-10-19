#pragma once
#include "buyer.h"
#include "item.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class seller : public Buyer {
private:
    int sellerId;
    string storeName;
    string storeAddress;
    string storePhoneNumber;
    string storeEmail;
    vector<Item> items;

public:
    seller(Buyer buyer, int sellerId, const string& sName, const string& sAddr, const string& sPhone, const string& sMail)
        : Buyer(buyer.getId(), buyer.getName(), buyer.getAccount()),
          sellerId(sellerId), storeName(sName), storeAddress(sAddr),
          storePhoneNumber(sPhone), storeEmail(sMail) {}

    int getSellerId() const { return sellerId; }
    const string& getStoreName() const { return storeName; }
    const string& getStoreAddress() const { return storeAddress; }
    const string& getStorePhoneNumber() const { return storePhoneNumber; }
    const string& getStoreEmail() const { return storeEmail; }
    vector<Item>& getItems() { return items; }
    const vector<Item>& getItems() const { return items; }

    void addNewItem(int newId, const string& newName, int newQuantity, double newPrice) {
        Item newItem(newId, newName, newQuantity, newPrice);
        items.push_back(newItem);
    }

    void removeItem(int itemId) {
        auto it = remove_if(items.begin(), items.end(), [itemId](const Item& item) {
            return item.getId() == itemId;
        });

        if (it != items.end()) {
            items.erase(it, items.end());
            cout << "Item with ID " << itemId << " has been removed." << endl;
        } else {
            cout << "Item with ID " << itemId << " not found in your cart." << endl;
        }
    }

    void viewItems() const {
        cout << "\n--- Inventory for " << storeName << " ---" << endl;
        if (items.empty()) {
            cout << "Inventory is empty." << endl;
        } else {
            for (const auto& item : items) {
                cout << "ID: " << item.getId() << ", Name: " << item.getName()
                     << ", Quantity: " << item.getQuantity() << ", Price: $" << item.getPrice() << endl;
            }
        }
    }
};