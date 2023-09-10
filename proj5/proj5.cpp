#include <iostream>
#include <utility>
#include <string>
#include "passserver.h"

using namespace cop4530;
using namespace std;

void Menu();
unsigned long prime_below(unsigned long n);
void setPrimes(vector<unsigned long> & vprimes);

// function to print the menu
void Menu()
{
    cout << "\n\n";
    cout << "l - Load From File" << endl;
    cout << "a - Add User" << endl;
    cout << "r - Remove User" << endl;
    cout << "c - Change User Password" << endl;
    cout << "f - Find User" << endl;
    cout << "d - Dump HashTable" << endl;
    cout << "s - HashTable Size" << endl;
    cout << "w - Write to Password File" << endl;
    cout << "x - Exit program" << endl;
    cout << "\nEnter choice : ";
}


unsigned long prime_below(unsigned long n) {
    if (n > max_prime) {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime) {
        return max_prime;
    }
    if (n <= 1) {
        std::cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    std::vector<unsigned long> v(n + 1);
    setPrimes(v);
    while (n > 2) {
        if (v[n] == 1)
            return n;
        --n;
    }

    return 2;
}

void setPrimes(std::vector<unsigned long> & vprimes) {
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for (i = 2; i * i < n; ++i) {
        if (vprimes[i] == 1)
            for (j = i + i; j < n; j += i)
                vprimes[j] = 0;
    }
}

int main()
{
    // variables
    int size = 0;
    string choice;
    string file;
    string user_name;
    string pass_word;

    // user inputs size of hash table
    cout << "Enter preferred hash table capacity (integer): ";
    cin >> size;

    // if user input is less than or equal to 1
    if (size <= 1) {
        cout << "** input too small\n";
        cout << "set to default capacity\n";
        size = 101;
    }
    cout << prime_below(size) << endl;
    // pass in hash table size
    PassServer passServer(size);

    // do while loop that prints menu until the user decides to exit
    do {
        Menu(); // print menu
        cin >> choice; // user choice

        // load from file
        if (choice == "l") {
            cout << "choice: l\n";
            cout << "Enter password file name to load from: ";
            cin >> file;

            if (!passServer.load(file.c_str())) // file error
                cout << "Error: Cannot open file " << file;

            cout << endl;
        }
            // add user
        else if (choice == "a") {
            cout << "choice: a\n";
            cout << "Enter username: ";
            cin >> user_name;
            cout << "Enter password: ";
            cin >> pass_word;
            if (passServer.addUser(make_pair(user_name, pass_word)))
                cout << "\nUser " << user_name << " added.\n";
            else
                cout
                        << "\n*****Error: User already exists. Could not add user\n"; // print error message if user already exists
        }
            // remove user
        else if (choice == "r") {
            cout << "choice: r\n";
            cout << "Enter username: ";
            cin >> user_name;
            if (passServer.removeUser(user_name))
                cout << "User " << user_name << " deleted.\n";
            else
                cout << "*****Error: User not found.  Could not delete user\n";
        }
            // change user password
        else if (choice == "c") {
            cout << "choice: c\n";
            string newPassword;
            cout << "Enter username: ";
            cin >> user_name;
            cout << "Enter password: ";
            cin >> user_name;
            cout << "Enter new password: ";
            cin >> newPassword;
            if (passServer.changePassword(make_pair(user_name, pass_word), newPassword)) {
                cout << "\nPassword changed for user " << user_name;
            } else
                cout << "\n*****Error: Could not change user password\n";
        }
            // find user
        else if (choice == "f") {
            cout << "choice: f\n";
            cout << "Enter username: ";
            cin >> user_name;
            if (passServer.find(user_name))
                cout << "\nUser '" << user_name << "' found.\n";
            else
                cout << "\nUser '" << user_name << "' not found.\n";
        }
            // dump hash table
        else if (choice == "d"){
            cout << "choice: d\n";
            passServer.dump();
        }
            // hash table size
        else if (choice == "s") {
            cout << "choice: s\n";
            cout << "Size of hashtable: " << passServer.size() << endl;
        }

            // write to password file
        else if (choice == "w") {
            cout << "choice: w\n";
            cout << "Enter password file name to write to: ";
            cin >> file;
            if(!passServer.write_to_file(file.c_str()))
                cout << "Error: File write failure\n";
        }
            // exit program
        else if (choice == "x") {
            cout << "choice: x";
            return 0;
        }

            // user enters a value not included in the menu
        else
            cout << "*****Error: Invalid entry.  Try again\n";

    } while (choice != "x"); // run until user enters x

    return 0;

}
