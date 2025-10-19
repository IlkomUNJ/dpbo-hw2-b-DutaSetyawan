#ifndef ORDER_H
#define ORDER_H

#include "item.h"
#include <vector>
#include <string>

using namespace std;

enum OrderStatus { UNPAID, PAID, CANCELLED, COMPLETED };

class Order {
private:
    int orderId;
    int buyerId;
    int sellerId;
    vector<Item> items;
    double totalAmount;
    OrderStatus status;

public:
    Order(int id, int bId, int sId, const vector<Item>& cartItems)
        : orderId(id), buyerId(bId), sellerId(sId), items(cartItems), status(UNPAID) {
        totalAmount = 0;
        for (const auto& item : items) {
            totalAmount += item.getPrice();
        }
    }

    int getOrderId() const { return orderId; }
    int getBuyerId() const { return buyerId; }
    int getSellerId() const { return sellerId; }
    double getTotalAmount() const { return totalAmount; }
    OrderStatus getStatus() const { return status; }
    const vector<Item>& getItems() const { return items; }

    void setStatus(OrderStatus newStatus) {
        status = newStatus;
    }

    string getStatusString() const {
        switch (status) {
            case UNPAID: return "Unpaid";
            case PAID: return "Paid";
            case CANCELLED: return "Cancelled";
            case COMPLETED: return "Completed";
            default: return "Unknown";
        }
    }
};

#endif