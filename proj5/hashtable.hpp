// *********** public member functions ****************

// constructor
template<typename K, typename V>
HashTable<K, V>::HashTable(size_t size) : listSize{prime_below(size)} {
    hashList.resize(listSize);
    currentSize = 0;
}

// destructor
template<typename K, typename V>
HashTable<K, V>::~HashTable() {
    makeEmpty();
}

// check if key k is in the hash table
template<typename K, typename V>
bool HashTable<K, V>::contains(const K &k) const {
    size_t index = myhash(k);
    for (auto itr = hashList[index].begin(); itr != hashList[index].end(); itr++) {
        if (itr->first == k) // k is in the hash table
            return true;
    }
    return false; // k is not in the hash table
}

// check if key-value pair is in the hash table
template<typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv) {
    auto & whichList = hashList[myhash(kv.first)];
    return(find(begin(whichList), end(whichList), kv) != end(whichList));
}

// adds the key-value pair kv into the hash table
template<typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> &kv) {
    auto & whichList = hashList[myhash(kv)];

    if(find(begin(whichList), end(whichList), kv) != end(whichList))
        return false;

    whichList.push_back(kv);

    // rehash
    if(++currentSize > hashList.size())
        rehash();

    return true;
}

// move version of insert
template<typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> &&kv) {
    auto & whichList = hashList[myhash(kv.first)];

    if(find(begin(whichList), end(whichList), kv) != end(whichList))
        return false;

    whichList.push_back(std::move(kv));

    // rehash
    if(++currentSize > hashList.size())
        rehash();

    return true;
}

// delete the key k and the corresponding value if it is in the hash table
template<typename K, typename V>
bool HashTable<K, V>::remove(const K &k) {
    size_t index = myhash(k);
    for (auto itr = hashList[index].begin(); itr != hashList[index].end(); itr++) {
        if (itr->first == k) {
            hashList[index].erase(itr);
            currentSize--;
            return true;
        }
    }
    return false;
}

// delete all elements in the hash table
template<typename K, typename V>
void HashTable<K, V>::clear() {
    makeEmpty();
}

// load the content of the file with name filename into the hash table
template<typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
    ifstream inputFile;
    inputFile.open(filename);
    K key;
    V val;
    // error opening file
    if (!inputFile.is_open()) {
        cout << "Error loading the file...\n";
        return false;
    } else {
        while (inputFile >> key >> val) {
            pair<K, V> new_pair;
            new_pair.first = key;
            new_pair.second = val;
            size_t index = myhash(key);
            hashList[index].push_back(new_pair);
        }
    }
    inputFile.close();
    return true;
}

// display all entries in the hash table
template<typename K, typename V>
void HashTable<K, V>::dump() {
    for (int i = 0; i < listSize; i++) {
        cout << "v[" << i << "]";
        for (auto itr = hashList[i].begin(); itr != hashList[i].end(); itr++) {
            cout << ": " << itr->first << ' ' << itr->second;
        }
        cout << endl;
    }
    cout << endl;
}

// return the size of the hash table
template<typename K, typename V>
size_t HashTable<K, V>::size() const {
    return currentSize;
}

// write all elements in the hash table into a file with name filename
template<typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const {
    ofstream outputFile;
    outputFile.open(filename);
    // error opening file
    if (!outputFile.is_open()) {
        cout << "Error creating file \n";
        return false;
    } else {
        for (int i = 0; i < listSize; i++) {
            for (auto itr = hashList[i].begin(); itr != hashList[i].end(); itr++) {
                outputFile << itr->first << ' ' << itr->second << endl;
            }
        }
    }
    outputFile.close();
    return true;
}

// *************** private member functions *******************

// delete all elements in the hash table
template<typename K, typename V>
void HashTable<K, V>::makeEmpty() {
    for (auto &thisList : hashList)
        thisList.clear();
}

// rehash, called when the number of elements in the hash table is greater than the size of the vector
template<typename K, typename V>
void HashTable<K, V>::rehash() {
    auto oldList = hashList;

    // double table size
    hashList.resize(prime_below(2 * hashList.size()));
    makeEmpty();
    currentSize = 0;

    // move elements to larger table
    for (auto &thisList : oldList)
        for (auto &kv : thisList)
            insert(std::move(kv));
}

// return the index of the vector entry where k should be stored
template<typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const {
    static hash<K> hf;
    return hf(k) % hashList.size();
}

// returns the largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template<typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n) {
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

// sets all prime number indexes to 1. Called by method prime_below(n)
template<typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long> & vprimes) {
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