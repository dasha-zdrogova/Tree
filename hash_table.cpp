#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <random>
#include <algorithm>
#include <set>

using namespace std;

class Hash_table {
    public:
    int capacity;
    list<pair<int, int>> *item;

    bool isPrime(int n) {
        if (n == 0 || n == 1) return false;
        if (n % 2 == 0) n++;
        for (int i = 2; i <= sqrt(n); ++i){
            if (n % i == 0) return false;
        }
        return true;
    }

    int getPrime(int n) {
        if (n % 2 == 0) n++;
        if (!isPrime(n)) return getPrime(n + 2);
        else return n;
    }

    int hashFunction(int key) {
        return key % capacity;
    }

    int search(int key) {
        int index = hashFunction(key);
        list<pair<int, int>>::iterator i;
        for (i = item[index].begin(); i != item[index].end(); ++i) {
            if ((*i).first == key) break;
        }
        if (i != item[index].end()) return (*i).second;
        else return 0;
    }

    void insert(int key, int data) {
        int index = hashFunction(key);
        item[index].push_back(make_pair(key, data));
    }

    void erase(int key) {
        int index = hashFunction(key);
        list<pair<int, int>>::iterator i;
        for (i = item[index].begin(); i != item[index].end(); ++i) {
            if ((*i).first == key) break;
        }
        if (i != item[index].end()) item[index].erase(i);
    }

    Hash_table(int n) {
        capacity = getPrime(n);
        item = new list<pair<int, int>>[capacity];
    }
};

int main(){
    set<int> S;

    Hash_table t(1000);
    cout<<"Вставка:"<<endl;
    for(int i = 0; i < 10000; i++) {
        int x = rand();
        S.insert(x);
        t.insert(x, x+1);
    }

    cout<<"Поиск(есть):"<<endl;
    for(int i = 0; i < 1000; i++){
        int x = rand()%S.size();
        set<int>::iterator j = S.begin();
        advance(j,x);
        if(t.search(*j) != *j+1) cout<<"плохо ищет"<<endl;
    }

    cout<<"Поиск(нет):"<<endl;
    for(int i = 0; i < 1000; i++) {
        int x = rand();
        if(S.count(x) == 0 && t.search(x) != 0) cout<<"плохо ищет"<<endl;
    }

    cout<<"Удаление:"<<endl;
    for(int i = 0; i < 1000 ; i++) {
        int x = rand()%S.size();
        set<int>::iterator j = S.begin();
        advance(j,x);
        t.erase(*j);
    }
    return 0;
}