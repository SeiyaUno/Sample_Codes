// SkipListSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the keys and their
// values.  Instead, you'll need to implement your own dynamically-allocated
// nodes, with pointers connecting them.  You can, however, use other parts of
// the C++ Standard Library -- including <random>, notably.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them, though you can make changes to them, if you'd like.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <optional>
#include <random>
#include "Set.hpp"




// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    static SkipListKey normal(const ElementType& element);
    static SkipListKey normal(const std::optional<ElementType>& element);
    static SkipListKey negInf();
    static SkipListKey posInf();

    SkipListKey();
    std::optional<ElementType> getElement();

    bool operator==(const SkipListKey& other) const;
    bool operator==(const ElementType& other) const;

    bool operator<(const SkipListKey& other) const;
    bool operator<(const ElementType& other) const;

private:
    SkipListKey(SkipListKind kind, std::optional<ElementType> element);

private:
    SkipListKind kind;
    std::optional<ElementType> element;
};


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::normal(const ElementType& element)
{
    return SkipListKey{SkipListKind::Normal, std::make_optional(element)};
}

template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::normal(const std::optional<ElementType>& element)
{
    return SkipListKey{SkipListKind::Normal, element};
}


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::negInf()
{
    return SkipListKey{SkipListKind::NegInf, std::nullopt};
}


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::posInf()
{
    return SkipListKey{SkipListKind::PosInf, std::nullopt};
}

//default constructor
template <typename ElementType>
SkipListKey<ElementType>::SkipListKey()
{
    
}

template <typename ElementType>
std::optional<ElementType> SkipListKey<ElementType>::getElement() {
    if(kind == SkipListKind::Normal) 
    {
        return element;
    }
    else {
        return std::nullopt;
    }
}


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, std::optional<ElementType> element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || *element == *other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const ElementType& other) const
{
    return kind == SkipListKind::Normal && *element == other;
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && *element < *other.element);
    }
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const ElementType& other) const
{
    return kind == SkipListKind::NegInf
        || (kind == SkipListKind::Normal && *element < other);
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level).  However, by setting things up
// this way, we have a way to control things more carefully in our
// testing (as you can, as well).
//
// DO NOT MAKE CHANGES TO THE SIGNATURES OF THE MEMBER FUNCTIONS OF
// THE "level tester" CLASSES.  You can add new member functions or even
// whole new level tester classes, but the ones declared below are part
// of how we test your implementation, so they need to maintain the
// parts of the interface that are declared already.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();

    bool shouldOccupyNextLevel(const ElementType& element) override;
    std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    ~SkipListSet() noexcept override;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;


private:
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;

    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    class Node {
    public:
        Node() {
            next = nullptr;
            down = nullptr;
        }

        Node(int l, const ElementType& elm) {
            next = nullptr;
            down = nullptr;
            level = l;
            indicatorAndData = SkipListKey<ElementType>::normal(elm);
        }

        Node(int l, const std::optional<ElementType>& elm) {
            next = nullptr;
            down = nullptr;
            level = l;
            indicatorAndData = SkipListKey<ElementType>::normal(elm);
        }

        Node(SkipListKind kind, int l) {
            next = nullptr;
            down = nullptr;
            level = l;

            if(kind == SkipListKind::PosInf) {
                indicatorAndData = SkipListKey<ElementType>::posInf();
            }
            else {
                indicatorAndData = SkipListKey<ElementType>::negInf();
            }
        }

        Node* next;
        Node* down; 
        SkipListKey<ElementType> indicatorAndData;
        int level;
    };
    Node* root;     //this should point to the first node at the top level, which is negInf
    int listSize;   //This does not count duplicate elements on another level
    int totalLevel;

    //Helper functions
    void insertBtw(Node* before, std::optional<ElementType> element, Node* after);
    void insertBelow(Node* before, Node* above, std::optional<ElementType> element, Node* after);
    void copyAll(const SkipListSet& s);
};


//By default, this one and the one below is called
template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : SkipListSet{std::make_unique<RandomSkipListLevelTester<ElementType>>()}
{
    
}

//By default, this one and the one above is called
//If SkipListLevelTester object is passed as a parameter when constructing an object
//only this function will be called
template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}
{
    listSize = 0;
    totalLevel = 1;

    Node* posInfinity = new Node(SkipListKind::PosInf, 0);
    Node* negInfinity = new Node(SkipListKind::NegInf, 0);
    root = negInfinity;
    negInfinity->next = posInfinity;
}


template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
    Node* current = root;

    while(root != nullptr) {
        root = root->down;

        while(current != nullptr) {
            Node* temp = current;
            current = current->next;

            delete temp;
        }
        current = root;
    }
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
{
    //create top level
    listSize = 0;
    totalLevel = 0;
    Node* posInfinity = new Node(SkipListKind::PosInf, s.root->level);
    Node* negInfinity = new Node(SkipListKind::NegInf, s.root->level);
    root = negInfinity;
    negInfinity->next = posInfinity;

    copyAll(s);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
{
    root = s.root;
    listSize = s.listSize;
    totalLevel = s.totalLevel;

    s.root = nullptr;
    s.listSize = 0;
    s.totalLevel = 0;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    if(this == &s) {  //if it is self assigning
        return *this;
    }
    copyAll(s);

    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
    Node* temp = root;
    int lS = listSize;
    int tL = totalLevel;

    root = s.root;
    listSize = s.listSize;
    totalLevel = s.totalLevel;

    s.root = temp;
    s.listSize = lS;
    s.totalLevel = tL;

    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
    if(contains(element)) {
        return;
    }

    Node* current = root;
    bool insertionNotDone = true;
    while(insertionNotDone) {
        if(SkipListKey<ElementType>::normal(element) < current->next->indicatorAndData) {
            if(current->level == 0) {
                //if current is at level 0
                insertBtw(current, element, current->next);
                listSize++;
                insertionNotDone = false;
            }
            else {
                current = current->down;
            }
        }
        else {
            current = current->next;
        }
    } 

    //flip a coin and add
    Node* oneAdded = current->next;
    int nextLevel = 1;      //There should be at least one level
    while(levelTester->shouldOccupyNextLevel(element)) {
        nextLevel++;
        current = root; 
        if(nextLevel > totalLevel) {
            //if this node is the first to go up another level
            //current should be at the level below the new level
            totalLevel++;
            
            Node* posInfinity = new Node(SkipListKind::PosInf, oneAdded->level + 1);
            Node* negInfinity = new Node(SkipListKind::NegInf, oneAdded->level + 1);
            Node* node = new Node(oneAdded->level + 1, element);

            negInfinity->down = root;
            negInfinity->next = node;
            node->next = posInfinity;
            node->down = oneAdded;
            root = negInfinity;

            //move current to one node before PosInf
            while(!(current->next->indicatorAndData == SkipListKey<ElementType>::posInf()))  {
                current = current->next;
            }

            posInfinity->down = current->next;
            oneAdded = node;
        }
        else {
            //move down levels
            for(int i = 0; i < totalLevel - nextLevel; i++) {
                current = current->down;
            }

            bool notInserted = true;
            while(notInserted) {
                if(SkipListKey<ElementType>::normal(element) < current->next->indicatorAndData ) {
                    notInserted = false;
                    Node* oneAbove = new Node(nextLevel - 1, element);
                    oneAbove->next = current->next;
                    oneAbove->down = oneAdded;
                    current->next = oneAbove;

                    oneAdded = oneAbove;
                }
                else {
                    current = current->next;
                }
            }
        }
    }
}


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{
    if(root->next->indicatorAndData == SkipListKey<ElementType>::posInf()) {
        //if skip list is empty
        return false;
    }

    Node* current = root;
    bool found = false;
    
    while(!found) {
        if(current->indicatorAndData == SkipListKey<ElementType>::normal(element)) {
            found = true;
        }
        else if(SkipListKey<ElementType>::normal(element) < current->next->indicatorAndData) {
            if(current->down == nullptr || current->indicatorAndData == SkipListKey<ElementType>::posInf()) {
                //if we cannot go further down or reached posInf
                break;
            }
            else {
                current = current->down;
            }
        }
        else {
            if(current->next != nullptr) {
                current = current->next;
            }
        }
    }

    return found;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    return listSize;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    return totalLevel;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    int l = static_cast<int>(level);
    unsigned int count = 0;

    if(l + 1 > totalLevel) {
        return count;
    }
 
    Node* current  = root;
    //move down levels
    for(int i = 0; i < totalLevel - l - 1; i++ ) {
        current = current->down;
    }

    current = current->next;    //skip negInf
    while(!(current->indicatorAndData == SkipListKey<ElementType>::posInf())) {
        count++;
        current = current->next;
    }

    return count;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    int l = static_cast<int>(level);

    if(l + 1 > totalLevel) {
        return false;
    }
 
    Node* current  = root;
    //move down levels
    for(int i = 0; i < totalLevel - l - 1; i++ ) {
        current = current->down;
    }

    current = current->next;    //skip negInf
    while(!(current->indicatorAndData == SkipListKey<ElementType>::posInf())) {
        if(current->indicatorAndData == SkipListKey<ElementType>::normal(element)) {
            return true;
        }
        current = current->next;
    }

    return false;
}

template <typename ElementType>
void SkipListSet<ElementType>::insertBtw(Node* before, std::optional<ElementType> element, Node* after) {
    Node* node = new Node(before->level, element);
    before->next = node;
    node->next = after;
    node->down = nullptr;

    //This is statementis for copy constructing
    if(before->down != nullptr) {
        insertBelow(before->down, node, element, after->down);
    }
}


template <typename ElementType>
void SkipListSet<ElementType>::insertBelow(Node* before, Node* above, std::optional<ElementType> element, Node* after) {
    Node* node = new Node(before->level, element);
    before->next = node;
    node->next = after;
    above->down = node;
    node->down = nullptr;

    if(before->down != nullptr) {
        insertBelow(before->down, node, element, after->down);
    }
}

//To call this fucntion, there should be at least an empty Skip lilst
template <typename ElementType>
void SkipListSet<ElementType>::copyAll(const SkipListSet<ElementType>& s) {

    listSize = s.listSize;
    totalLevel = s.totalLevel;
    Node* currentOrig = s.root->next;   //currentOrig is always one node ahead of currentCopy
    Node* leftEdgeOrig = s.root;

    Node* currentCopy = root;
    Node* leftEdgeCopy = root;          //always points to negInf
    Node* rightEdgeCopy = root->next;   //rightEdgeCopy should be pointing to posInf

    //create all infinity
    for(int i = totalLevel - 2; i >= 0; i--) {
        Node* posInfinityBelow = new Node(SkipListKind::PosInf, i);
        Node* negInfinityBelow = new Node(SkipListKind::NegInf, i);
        leftEdgeCopy->down = negInfinityBelow;
        negInfinityBelow->next = posInfinityBelow;
        leftEdgeCopy->next->down = posInfinityBelow;

        //move down
        leftEdgeCopy = leftEdgeCopy->down;
    }

    leftEdgeCopy = root;

    while(leftEdgeOrig != nullptr) {
        
        while(currentOrig != nullptr && currentCopy->next != nullptr) {
            
            if(!(currentOrig->indicatorAndData == currentCopy->next->indicatorAndData)) {
                insertBtw(currentCopy, currentOrig->indicatorAndData.getElement(), currentCopy->next);
            }

            currentOrig = currentOrig->next;
            currentCopy = currentCopy->next;
        }
        
        leftEdgeOrig = leftEdgeOrig->down;
        currentOrig = leftEdgeOrig;
        //currentOrig is always one node ahead of currentCopy
        if(currentOrig != nullptr) {
            currentOrig = currentOrig->next;
        }

        leftEdgeCopy = leftEdgeCopy->down;
        rightEdgeCopy= rightEdgeCopy->down;
        currentCopy = leftEdgeCopy;
    }
}

#endif

