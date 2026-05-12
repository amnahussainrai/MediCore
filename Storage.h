// Storage.h
#pragma once

template <typename T>
class Storage {
private:
    T data[100];
    int count;
public:
    Storage() : count(0) {}

    void add(T item) {
        if (count < 100) data[count++] = item;
    }

    T* findByID(int id) {
        for (int i = 0; i < count; i++)
            if (data[i].getid() == id) return &data[i];
        return nullptr;
    }

    void removeByID(int id) {
        for (int i = 0; i < count; i++) {
            if (data[i].getid() == id) {
                for (int j = i; j < count - 1; j++)
                    data[j] = data[j + 1];
                count--;
                return;
            }
        }
    }

    T* getAll() { return data; }
    int getSize() { return count; }
};