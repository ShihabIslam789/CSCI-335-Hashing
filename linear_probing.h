#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H


#include <vector>
#include <algorithm>
#include <functional>
#include <string>



// Linear probing implementation.
template <typename HashedObj>
class HashTableLinear {
public:
    enum EntryType { ACTIVE, EMPTY, DELETED };

    explicit HashTableLinear(size_t size = 101) : array_(NextPrime(size))
    {
        MakeEmpty();
    }

    bool Contains(const HashedObj& x) const {
        return IsActive(FindPos(x));
    }

    void MakeEmpty() {
        current_size_ = 0;
        for (auto& entry : array_)
            entry.info_ = EMPTY;
    }

    bool Insert(const HashedObj& x) {
        // Insert x as active
        size_t current_pos = FindPos(x);
        if (IsActive(current_pos))
            return false;

        array_[current_pos].element_ = x;
        array_[current_pos].info_ = ACTIVE;

        //rehash
        if (++current_size_ > array_.size() / 2)
            Rehash();
        return true;
    }

    bool Insert(HashedObj&& x) {
        // Insert x as active position
        size_t current_pos = FindPos(x);
        if (IsActive(current_pos))
            return false;

        array_[current_pos] = std::move(x);
        array_[current_pos].info_ = ACTIVE;


        if (++current_size_ > array_.size() / 2)
            Rehash();

        return true;
    }

    bool Remove(const HashedObj& x) {
        size_t current_pos = FindPos(x);
        if (!IsActive(current_pos))
            return false;

        array_[current_pos].info_ = DELETED;
        return true;
    }

    
    //  Number of Elements in HashTable
    size_t getSize() const {
        return current_size_;
    }

    
      // size of HashTable   
    size_t getTableSize() const {
        return array_.size();
    }

    // total number of Collisions
    size_t getCollisions() const {
        return collisions_;
    }

    
      // load factor   
    float getLoadFactor() const {
        return (float)current_size_ / (float)array_.size();
    }

    
      //  average number of Collisions  
    float getAverageCollisions() const {
        return (float)getCollisions() / (float)getSize();
    }

    
      // clears number of Collisions
     
    void clearCollisions() {
        collisions_ = 0;
    }

private:
    struct HashEntry {
        HashedObj element_;
        EntryType info_;

        HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
            :element_{ e }, info_{ i } { }

        HashEntry(HashedObj&& e, EntryType i = EMPTY)
            :element_{ std::move(e) }, info_{ i } {}
    };


    std::vector<HashEntry> array_;
    size_t current_size_;

    mutable size_t collisions_ = 0;

    bool IsActive(size_t current_pos) const
    {
        return array_[current_pos].info_ == ACTIVE;
    }

    size_t FindPos(const HashedObj& x) const {
        size_t current_pos = InternalHash(x);

        while (array_[current_pos].info_ != EMPTY &&
            array_[current_pos].element_ != x) {
            collisions_ += 1;
            current_pos += 1;  // chang position  by 1 on each iteration
                             

            if (current_pos >= array_.size())
                current_pos -= array_.size();
        }
        return current_pos;
    }

    void Rehash() {
        std::vector<HashEntry> old_array = array_;

        // Create new doubled empty table
        array_.resize(NextPrime(2 * old_array.size()));
        for (auto& entry : array_)
            entry.info_ = EMPTY;

        
        current_size_ = 0;
        for (auto& entry : old_array)
            if (entry.info_ == ACTIVE)
                Insert(std::move(entry.element_));
    }

    size_t InternalHash(const HashedObj& x) const {
        static std::hash<HashedObj> hf;
        return hf(x) % array_.size();
    }
};

#endif // LINEAR_PROBING_H