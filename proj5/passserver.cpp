#include "passserver.h"
#include "hashtable.h"

// constructor, create a hash table of the specified size
PassServer::PassServer(size_t size) : password(size) { }

// destructor
PassServer::~PassServer() {
    password.clear();
}

// load a password file into the HashTable object
bool PassServer::load(const char *filename) {
    return password.load(filename);
}

// add a new username and password
bool PassServer::addUser(std::pair<string, string> &kv) {
    return password.insert(make_pair(kv.first, encrypt(kv.second)));
}

// move version of addUser
bool PassServer::addUser(std::pair<string, string> &&kv) {
    return password.insert(std::move(make_pair(kv.first, encrypt(kv.second))));
}

// delete an existing user with username k
bool PassServer::removeUser(const string &k) {
    return password.remove(k);
}

// change an existing user's password
bool PassServer::changePassword(const std::pair<string, string> &p, const string &newpassword) {

    if(password.match(make_pair(p.first, encrypt(p.second))) && encrypt(p.second) != encrypt(newpassword)){
        password.insert(make_pair(p.first, encrypt(newpassword)));
        return true;
    }
    return false;
}

// check if a user exists
bool PassServer::find(const string &user) const {
    return password.contains(user);
}

// show the structure and contents of the HashTable object to the screen
void PassServer::dump() {
    password.dump();
}

// return the size of the HashTable
size_t PassServer::size() const {
    return password.size();
}

// save the username and password combination into a file
bool PassServer::write_to_file(const char *filename) const {
    return password.write_to_file(filename);
}

// encrypt the parameter str and return the encrypted string
string PassServer::encrypt(const string& str) {
    char salt[] = "$1$########";
    char * pass = new char [ 100 ];
    strcpy(pass, crypt(str.c_str(), salt));
    string newPass(pass);
    newPass.erase(0,12);
    return newPass;
}
