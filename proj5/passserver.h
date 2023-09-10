#ifndef PASSSERVER_H
#define PASSSERVER_H

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include "hashtable.h"

using namespace std;
using namespace cop4530;

class PassServer {
    public:
    PassServer(size_t size = 101);
    ~PassServer();
    bool load(const char *filename);
    bool addUser(std::pair<string, string> &kv);
    bool addUser(std::pair<string, string> &&kv);
    bool removeUser(const string &k);
    bool changePassword(const std::pair<string, string> &p, const string &newpassword);
    bool find(const string &user) const;
    void dump();
    size_t size() const;
    bool write_to_file(const char *filename) const;

private:
    string encrypt(const string &str);
    HashTable<string, string> password = 101;
};

#endif