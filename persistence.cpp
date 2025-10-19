#include "persistence.h"
#include "globals.h"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include <climits>
#include <iostream>

using namespace std;

const string BUYERS_FILE = "data_buyers.csv";
const string SELLERS_FILE = "data_sellers.csv";
const string INVENTORY_FILE = "data_inventory.csv";
const string ORDERS_FILE = "data_orders.csv";

static inline string sanitizePipe(const string& s){ string t=s; for(auto& c:t){ if(c=='|') c='/'; if(c=='\n'||c=='\r') c=' '; } return t; }
static inline string sanitizeAny(const string& s){ string t=s; for(auto& c:t){ if(c=='|'||c==';'||c=='~') c='/'; if(c=='\n'||c=='\r') c=' '; } return t; }
static inline vector<string> splitPipe(const string& line){ vector<string> parts; string cur; for(char c:line){ if(c=='|'){ parts.push_back(cur); cur.clear(); } else cur.push_back(c);} parts.push_back(cur); return parts; }
static inline vector<string> splitWithDelim(const string& line, char d){ vector<string> parts; string cur; for(char c:line){ if(c==d){ parts.push_back(cur); cur.clear(); } else cur.push_back(c);} parts.push_back(cur); return parts; }
static inline bool tryParseInt(const string& s, int& out){ try{ size_t idx=0; long long v=stoll(s,&idx); if(idx!=s.size()) return false; if(v < INT_MIN || v > INT_MAX) return false; out = static_cast<int>(v); return true; } catch(...){ return false; } }
static inline bool tryParseDouble(const string& s, double& out){ try{ size_t idx=0; double v=stod(s,&idx); if(idx!=s.size()) return false; out = v; return true; } catch(...){ return false; } }
static inline string statusToStr(OrderStatus st){ switch(st){ case UNPAID: return "UNPAID"; case PAID: return "PAID"; case CANCELLED: return "CANCELLED"; case COMPLETED: return "COMPLETED"; default: return "UNPAID"; } }
static inline OrderStatus strToStatus(const string& s){ if(s=="UNPAID") {return UNPAID;} else if(s=="PAID") {return PAID;} else if(s=="CANCELLED") {return CANCELLED;} else if(s=="COMPLETED") {return COMPLETED;} else {return UNPAID;} }

void saveBuyers(){ ofstream out(BUYERS_FILE, ios::trunc); if(!out) return; out.setf(std::ios::fixed); out << setprecision(2); for(const auto& b : buyers){ auto* acc = b.getAccount(); if(!acc) continue; out << b.getId() << '|' << sanitizePipe(b.getName()) << '|' << sanitizePipe(acc->getAddress()) << '|' << sanitizePipe(acc->getPhoneNumber()) << '|' << sanitizePipe(acc->getEmail()) << '|' << acc->getBalance() << "\n"; } }
void loadBuyers(){ ifstream in(BUYERS_FILE); if(!in) return; string line; while(getline(in, line)){ if(line.empty()) continue; auto parts = splitPipe(line); if(parts.size() < 6) continue; int id=0; if(!tryParseInt(parts[0], id)) continue; string name=parts[1], addr=parts[2], phone=parts[3], email=parts[4]; double balance=0; tryParseDouble(parts[5], balance); BankCustomer& acc = eshopBank.createAccountWithId(id, name, addr, phone, email, balance); Buyer b(id, name, &acc); buyers.push_back(b);} }

void saveSellers(){ ofstream out(SELLERS_FILE, ios::trunc); if(!out) return; for(const auto& s : sellers){ out << s.getSellerId() << '|' << s.getId() << '|' << sanitizePipe(s.getStoreName()) << '|' << sanitizePipe(s.getStoreAddress()) << '|' << sanitizePipe(s.getStorePhoneNumber()) << '|' << sanitizePipe(s.getStoreEmail()) << "\n"; } }
void loadSellers(){ ifstream in(SELLERS_FILE); if(!in) return; string line; while(getline(in, line)){ if(line.empty()) continue; auto parts = splitPipe(line); if(parts.size() < 6) continue; int sellerId=0, userId=0; if(!tryParseInt(parts[0], sellerId)) continue; if(!tryParseInt(parts[1], userId)) continue; string sName=parts[2], sAddr=parts[3], sPhone=parts[4], sMail=parts[5]; Buyer* owner=nullptr; for(auto& b : buyers){ if(b.getId()==userId){ owner=&b; break; } } if(!owner) continue; seller s(*owner, sellerId, sName, sAddr, sPhone, sMail); sellers.push_back(s);} }

void saveInventory(){ ofstream out(INVENTORY_FILE, ios::trunc); if(!out) return; out.setf(std::ios::fixed); out << setprecision(2); for(const auto& s : sellers){ for(const auto& it : s.getItems()){ out << s.getSellerId() << '|' << it.getId() << '|' << sanitizePipe(it.getName()) << '|' << it.getQuantity() << '|' << it.getPrice() << "\n"; } } }
void loadInventory(){ ifstream in(INVENTORY_FILE); if(!in) return; string line; while(getline(in, line)){ if(line.empty()) continue; auto parts = splitPipe(line); if(parts.size() < 5) continue; int sellerId=0, itemId=0, qty=0; double price=0; if(!tryParseInt(parts[0], sellerId)) continue; if(!tryParseInt(parts[1], itemId)) continue; string name=parts[2]; tryParseInt(parts[3], qty); tryParseDouble(parts[4], price); for(auto& s : sellers){ if(s.getSellerId()==sellerId){ s.addNewItem(itemId, name, qty, price); break; } } } }

void saveOrders(){ ofstream out(ORDERS_FILE, ios::trunc); if(!out) return; out.setf(std::ios::fixed); out << setprecision(2); for(const auto& o : orders){ out << o.getOrderId() << '|' << o.getBuyerId() << '|' << o.getSellerId() << '|' << statusToStr(o.getStatus()) << '|' << o.getTotalAmount() << "\n"; out << 'I' << '|' << o.getOrderId(); for(const auto& it : o.getItems()){ out << '|' << it.getId() << ';' << sanitizeAny(it.getName()) << ';' << it.getPrice(); } out << "\n"; } }
void loadOrders(){ ifstream in(ORDERS_FILE); if(!in) return; string line; unordered_map<int, vector<Item>> itemsByOrder; struct OrdRow{ int id,bid,sid; OrderStatus st; double total;}; vector<OrdRow> rows; while(getline(in, line)){ if(line.empty()) continue; auto parts = splitPipe(line); if(parts.empty()) continue; if(parts[0]=="I"){ int oid=0; if(!tryParseInt(parts[1], oid)) continue; vector<Item> its; for(size_t i=2;i<parts.size();++i){ auto f = splitWithDelim(parts[i], ';'); if(f.size()<3) continue; int iid=0; if(!tryParseInt(f[0], iid)) continue; string nm=f[1]; double pr=0; tryParseDouble(f[2], pr); its.emplace_back(iid, nm, 1, pr);} itemsByOrder[oid]=its; } else { if(parts.size()<5) continue; int id=0,bid=0,sid=0; double total=0; if(!tryParseInt(parts[0], id)) continue; if(!tryParseInt(parts[1], bid)) continue; if(!tryParseInt(parts[2], sid)) continue; tryParseDouble(parts[4], total); OrdRow r{id,bid,sid,strToStatus(parts[3]), total}; rows.push_back(r);} } for(const auto& r : rows){ auto it = itemsByOrder.find(r.id); vector<Item> itemsVec; if(it!=itemsByOrder.end()) itemsVec = it->second; Order o(r.id, r.bid, r.sid, itemsVec); if(r.st != UNPAID) o.setStatus(r.st); orders.push_back(o);} }

void loadData(){ buyers.clear(); sellers.clear(); orders.clear(); loadBuyers(); loadSellers(); loadInventory(); loadOrders(); }
void saveData(){ saveBuyers(); saveSellers(); saveInventory(); saveOrders(); }
