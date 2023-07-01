// HashSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;

    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node {
        ElementType data;
        unsigned int originalIndex; //keep an element's original index in case an element is moved
        bool continueSearch;    //If this is true, algorighm should search backwards to 
                                //check other nodes
        bool isEmpty;           //true if a node does not conatain data
        
        Node() {
            continueSearch = false;
            isEmpty = true;
        }
        
    };

    Node* table;                //This is the array for storing data
    unsigned int hashSize;      //Keep number of elements in hash table
    unsigned int currentCapacity;

    const float SIZE_THRESHOLD = 0.8;

    //Helper function
    void makeTableBigger();
    unsigned int updateIndex(const unsigned int i) const;
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    hashSize = 0;
    currentCapacity = DEFAULT_CAPACITY;
    table = new Node[currentCapacity];
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    delete [] table;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction /*impl_::HashSet__undefinedHashFunction<ElementType>*/}
{
    currentCapacity = s.currentCapacity;
    hashSize = s.hashSize;
    table = new Node[currentCapacity];

    //copy each element
    for(int i = 0; i < currentCapacity; i++) {
        if(!s.table[i].isEmpty) {
            table[i].data = s.table[i].data;
            table[i].originalIndex = s.table[i].originalIndex;
            table[i].continueSearch = s.table[i].continueSearch;
            table[i].isEmpty = s.table[i].isEmpty;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{std::move(s.hashFunction)/*impl_::HashSet__undefinedHashFunction<ElementType>*/}
{
    currentCapacity = s.currentCapacity;
    hashSize = s.hashSize;
    table = s.table;

    s.table = nullptr;
    s.currentCapacity = 0;
    s.hashSize = 0;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if(this == &s) {  //if it is self assigning
        return *this;
    }

    delete [] table;

    currentCapacity = s.currentCapacity;
    hashSize = s.hashSize;
    table = new Node[currentCapacity];

    for(int i = 0; i < currentCapacity; i++) {
        if(!s.table[i].isEmpty) {
            table[i].data = s.table[i].data;
            table[i].originalIndex = s.table[i].originalIndex;
            table[i].continueSearch = s.table[i].continueSearch;
            table[i].isEmpty = s.table[i].isEmpty;
        }
    }

    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    Node* tmpTable = table;
    unsigned int tmpSize = hashSize;
    unsigned int tmpCapacity = currentCapacity;

    currentCapacity = s.currentCapacity;
    hashSize = s.hashSize;
    table = s.table;

    s.table = tmpTable;
    s.currentCapacity = tmpCapacity;
    s.hashSize = tmpSize;

    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    //if a key is already in the hashtable do nothing
    if(contains(element)) {
        return;
    }

    unsigned int index = hashFunction(element) % currentCapacity;
    unsigned int origIndex = index;
    bool notDone = true;

    while(notDone) {

        if(table[index].isEmpty) {
            table[index].data = element;
            table[index].isEmpty = false;
            table[index].originalIndex = origIndex;

            notDone = false;
        }
        else {
            //Not empty
            if(table[index].originalIndex == origIndex) {
                //change continueSearch for searching elements
                table[index].continueSearch = true;
            }

            index = updateIndex(index);
        }
    }

    hashSize++;
    //check the occupancy percentage
    float ratio = hashSize / currentCapacity;
    if(ratio > SIZE_THRESHOLD) {
        makeTableBigger();
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int index = hashFunction(element) % currentCapacity;
    unsigned int origIndex = index;

    if(hashSize == 0) {
        return false;
    }

    do
    {
        if(!table[index].isEmpty) {
            if(table[index].data == element) {
                return true;
            }
            else if(table[index].originalIndex == origIndex && !table[index].continueSearch) {
                //in this case there is no more possible nodes to search
                break;
            }
        }
        else if(table[index].isEmpty && !table[index].continueSearch) {
             break;
        }

        index = updateIndex(index);

    } while(origIndex != index);
    
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return hashSize;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    unsigned int count = 0;
    unsigned int origIndex = index;

    do
    {
        if(!table[index].isEmpty) {
            if(table[index].originalIndex == origIndex) {
                count++;
            }
        }
        else {
            break;
        }

        index = updateIndex(index);

    } while (origIndex != index);
    
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    unsigned int i = hashFunction(element) % currentCapacity;

    if(contains(element) && i == index) {
        return true;
    }
    else {
        return false;
    }
}


//Helper functions
template <typename ElementType>
void HashSet<ElementType>::makeTableBigger() {

    unsigned int oldCapacity = currentCapacity;
    currentCapacity = currentCapacity * 2 + 1;
    Node* oldTable = table;
    table = new Node[currentCapacity];

    for(unsigned int i = 0; i < oldCapacity; i++) {
        if(!oldTable[i].isEmpty) {
             add(oldTable[i].data);
        }
    }

    delete [] oldTable;
}

template <typename ElementType>
unsigned int HashSet<ElementType>::updateIndex(const unsigned int i) const {
    unsigned int index  = i;

    if(index == 0) {
        //if the index reached the beginning of the array
        index = currentCapacity - 1;
    }
    else {
        index--;
    }
    return index;
}

#endif