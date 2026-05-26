#ifndef HASH_TABLES_H
#define HASH_TABLES_H
#include <string>

using namespace std;

class HashTables {
    string* table;
    int size;
    int hashFunction(int key);

public:
    HashTables(int s);
    ~HashTables();

    void insert(int key, std::string value);
    bool retrieve(int key, std::string& out);
    void updateCache(int key, std::string value);
    void display();

    static void demo();
    static void hashMenu(int choice);
};

#endif
