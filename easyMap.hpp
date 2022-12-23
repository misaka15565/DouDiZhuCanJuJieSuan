#ifndef EASY_MAP_H
#define EASY_MAP_H

#include <unordered_map>


template <typename K, typename V>
class easyMAP {
public:
    easyMAP() {
    }

    ~easyMAP() {
    }
    bool isEmpty() {
        return map_.empty();
    }

    bool insert(const K &key, const V &value) {
        auto ret = map_.insert(std::pair<K, V>(key, value));
        return ret.second;
    }

    bool find(const K &key, V &value) {
        bool ret = false;
        auto iter = map_.find(key);
        if (iter != map_.end()) {
            value = iter->second;
            ret = true;
        }
        return ret;
    }

private:
    std::unordered_map<K, V> map_;
};

#endif