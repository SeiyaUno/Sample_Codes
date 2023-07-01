// DoublyLinkedList.hpp
//
// ICS 46 Winter 2022
// Project #2: Time Waits for No One
//
// DoublyLinkedList<ValueType> is a class template that implements a
// doubly-linked list with head and tail pointers, including two kinds
// of iterators: One of them allows viewing and modifying the list's
// contents, while the other allows only viewing them.
//
// Your goal is to implement the entire public interface *exactly* as
// specified below.  Do not modify the signatures of any of the public
// member functions (including the public member functions of the various
// iterator classes) in any way.  We will be running extensive unit
// tests against your implementation, which will not compile unless
// things remain completely unchanged.  As we did in Project #0, we've
// provided you a basic set of unit tests that briefly demonstrate how
// each of the member functions is required to behave; you'll find
// those in the "gtest" directory.
//
// All of the public member functions listed with "noexcept" in their
// signature must be implemented in a way that they never throw exceptions.
// All of the others are expected to make the "strong" exception guarantee,
// which means two things: (1) no memory has leaked, and (2) the contents
// of the list/iterator will not have visibly changed in the event that
// an exception has been thrown.
//
// The entire C++ Standard Library is off-limits in your implementation
// of this class.  DO NOT submit a version of this file (or any file
// that it includes) that includes any C++ Standard Library headers.
// (This includes things like adding a print() member function that
// requires <iostream>.)
//
// As is good custom in class templates, keep the interface separate
// from the implementation.  This means the bodies of member functions
// should not be written in the class declaration, but should appear
// below it.  I've placed "dummy" implementations of every public
// member function, though, of course, most of them don't do the
// right thing; but they will save you some typing and demonstrate
// the structure of what you should be writing.

#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include "EmptyException.hpp"
#include "IteratorException.hpp"

template <typename ValueType>
class DoublyLinkedList
{
    // The forward declarations of these classes allows us to establish
    // that they exist, but delay displaying all of the details until
    // later in the file.
    //
    // (This is generally a good style, with the most important details
    // appearing earlier in the class declaration.  That's the same
    // reason why we're implementing the bodies of the member functions
    // outside of the class declaration.)
public:
    class Iterator;
    class ConstIterator;


private:
    struct Node;

public:
    // Initializes this list to be empty.
    DoublyLinkedList() noexcept;

    // Initializes this list as a copy of an existing one.
    DoublyLinkedList(const DoublyLinkedList& list);

    // Initializes this list from an expiring one.
    DoublyLinkedList(DoublyLinkedList&& list) noexcept;


    // Destroys the contents of this list.
    virtual ~DoublyLinkedList() noexcept;


    // Replaces the contents of this list with a copy of the contents
    // of an existing one.
    DoublyLinkedList& operator=(const DoublyLinkedList& list);

    // Replaces the contents of this list with the contents of an
    // expiring one.
    DoublyLinkedList& operator=(DoublyLinkedList&& list) noexcept;


    // addToStart() adds a value to the start of the list, meaning that
    // it will now be the first value, with all subsequent elements still
    // being in the list (after the new value) in the same order.
    void addToStart(const ValueType& value);

    // addToEnd() adds a value to the end of the list, meaning that
    // it will now be the last value, with all subsequent elements still
    // being in the list (before the new value) in the same order.
    void addToEnd(const ValueType& value);


    // removeFromStart() removes a value from the start of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the first one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromStart();

    // removeFromEnd() removes a value from the end of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the last one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromEnd();


    // first() returns the value at the start of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& first() const;
    ValueType& first();


    // last() returns the value at the end of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& last() const;
    ValueType& last();


    // isEmpty() returns true if the list has no values in it, false
    // otherwise.
    bool isEmpty() const noexcept;


    // size() returns the number of values in the list.
    unsigned int size() const noexcept;



    // There are two kinds of iterators supported: Iterators and
    // ConstIterators.  They have similar characteristics; they both
    // allow you to see what values are in the list and move back and
    // forth between them.  The difference is that ConstIterators allow
    // you to see the elements but not modify them, while Iterators also
    // support modification of the list (both by modifying the elements
    // directly, and also by inserting or removing values at arbitrary
    // locations).
    //
    // At any given time, an iterator refers to a value in the list.
    // There are also two additional places it can refer: "past start"
    // and "past end", which are the positions directly before the
    // first value and directly after the last one, respectively.
    // Except with respect to those boundaries, they can be moved
    // both forward and backward.
    //
    // Note, too, that the reason we have a ConstIterator class instead
    // of just saying "const Iterator" is because a "const Iterator"
    // is something different: It's an Iterator object that you can't
    // modify (i.e., you can't move it around).  What a ConstIterator
    // holds constant isn't the iterator; it's the list that's protected
    // by it.


    // iterator() creates a new Iterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    Iterator iterator();


    // constIterator() creates a new ConstIterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    ConstIterator constIterator() const;


public:
    // The IteratorBase class is the base class for our two kinds of
    // iterators.  Because there are so many similarities between them,
    // we write those similarities in a base class, then inherit from
    // that base class to specify only the differences.
    class IteratorBase
    {
    public:
        // Initializes a newly-constructed IteratorBase to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        IteratorBase(const DoublyLinkedList& list) noexcept;


        // moveToNext() moves this iterator forward to the next value in
        // the list.  If the iterator is refrering to the last value, it
        // moves to the "past end" position.  If it is already at the
        // "past end" position, an IteratorException will be thrown.
        void moveToNext();


        // moveToPrevious() moves this iterator backward to the previous
        // value in the list.  If the iterator is refrering to the first
        // value, it moves to the "past start" position.  If it is already
        // at the "past start" position, an IteratorException will be thrown.
        void moveToPrevious();


        // isPastStart() returns true if this iterator is in the "past
        // start" position, false otherwise.
        bool isPastStart() const noexcept;


        // isPastEnd() returns true if this iterator is in the "past end"
        // position, false otherwise.
        bool isPastEnd() const noexcept;
    
    protected:
        // You may want protected member variables and member functions,
        // which will be accessible to the derived classes.
        const DoublyLinkedList<ValueType>& keepInBase; 
        Node* currentItr;
        unsigned int position;   //the position of first element is 1
    };


    class ConstIterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed ConstIterator to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        ConstIterator(const DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        const ValueType& value() const;

    private:
        // You may want private member variables and member functions.
        const DoublyLinkedList<ValueType>& keeper; 
    };


    class Iterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed Iterator to operate on the
        // given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        Iterator(DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        ValueType& value() const;


        // insertBefore() inserts a new value into the list before
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past start" position, an IteratorException
        // is thrown.
        void insertBefore(const ValueType& value);


        // insertAfter() inserts a new value into the list after
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past end" position, an IteratorException
        // is thrown.
        void insertAfter(const ValueType& value);


        // remove() removes the value to which this iterator refers,
        // moving the iterator to refer to either the value after it
        // (if moveToNextAfterward is true) or before it (if
        // moveToNextAfterward is false).  If the iterator is in the
        // "past start" or "past end" position, an IteratorException
        // is thrown.
        void remove(bool moveToNextAfterward = true);

    private:
        // You may want private member variables and member functions.
        DoublyLinkedList<ValueType>& keeper; 
    };


private:
    // A structure that contains the vital parts of a Node in a
    // doubly-linked list, the value and two pointers: one pointing
    // to the previous node (or nullptr if there isn't one) and
    // one pointing to the next node (or nullptr if there isn't
    // one).
    struct Node
    {
        ValueType value;
        Node* prev;
        Node* next;
    };


    // You can feel free to add private member variables and member
    // functions here; there's a pretty good chance you'll need some.
private:
    Node* head;
    Node* tail;
    unsigned int listSize;
};



template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList() noexcept
{
    head = nullptr;
    tail = nullptr;
    listSize = 0;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(const DoublyLinkedList& list)
{
    head = nullptr;
    tail = nullptr;
    listSize = 0;

    //copy from list
    DoublyLinkedList<ValueType>::ConstIterator iterator = list.constIterator();
    while (!iterator.isPastEnd())
    {
        addToEnd(iterator.value());
        iterator.moveToNext();
    }
}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(DoublyLinkedList&& list) noexcept 
        : head(nullptr), tail(nullptr), listSize(0) 
{
    head = list.head;
    tail = list.tail;
    listSize = list.size();

    list.head = nullptr;
    list.tail = nullptr;
    list.listSize = 0;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::~DoublyLinkedList() noexcept
{
    while(!isEmpty()){
        removeFromStart();
    }
}


template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(const DoublyLinkedList& list)
{
    if(this == &list) {  //if it is self assigning
        return *this;
    }

    while(!isEmpty()){
        removeFromStart();
    }

    DoublyLinkedList<ValueType>::ConstIterator iterator = list.constIterator();

    //copy each element
    while (!iterator.isPastEnd())
    {
        addToEnd(iterator.value());
        iterator.moveToNext();
    }

    return *this;
}


template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(DoublyLinkedList&& list) noexcept
{
    if(this == &list) {  //if it is self assigning
        return *this;
    }

    Node* headKeeper;
    Node* tailKeeper;
    unsigned int sizeKeeper;

    //save info temporarily
    headKeeper = head;
    tailKeeper = tail;
    sizeKeeper = listSize;

    //move other list to this list
    head = list.head;
    tail = list.tail;
    listSize = list.size();

    list.head = headKeeper;
    list.tail = tailKeeper;
    list.listSize = sizeKeeper;

    return *this;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::addToStart(const ValueType& value)
{
    Node* node = new Node;
    node->value = value;
    node->prev = nullptr;

    if(isEmpty()) {
        node->next = nullptr;
        head = node;
        tail = node;
    }
    else {
        //connect new node
        node->next = head;
        head->prev = node;
        head = node;
    }
    listSize++; //this is called here since isEmpty() is used in if statement
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::addToEnd(const ValueType& value)
{
    Node* node = new Node;
    node->value = value;
    node->next = nullptr;

    if(isEmpty()) {
        node->prev = nullptr;
        head = node;
        tail = node;
    }
    else {
        //connect new node
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
    listSize++; //this is called here since isEmpty() is used in if statement
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromStart()
{
    if(!isEmpty()) {
        Node* current = head;
        
        if(current->next != nullptr) {
            //this will not be called if there is only one node in the list
            head = current->next;
            head->prev = nullptr;
        }
        
        delete current;
        listSize--;
    }
    else {
        throw EmptyException{};
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromEnd()
{
    if(!isEmpty()) {
        Node* current = tail;

        if(current->prev != nullptr) {
            //this will not be called if there is only one node in the list
            tail = current->prev;
            tail->next = nullptr;
        }

        delete current;
        listSize--;
    }
    else {
        throw EmptyException{};
    }
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::first() const
{
    if(isEmpty()) {
        throw EmptyException{};
    }
    return head->value;
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::first()
{
    if(isEmpty()) {
        throw EmptyException{};
    }
    return head->value;
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::last() const
{
    if(isEmpty()) {
        throw EmptyException{};
    }
    return tail->value;
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::last()
{
    if(isEmpty()) {
        throw EmptyException{};
    }
    return tail->value;
}


template <typename ValueType>
unsigned int DoublyLinkedList<ValueType>::size() const noexcept
{
    return listSize;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::isEmpty() const noexcept
{
    if(listSize == 0) {
        return true;
    }
    else {
        return false;
    }
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::Iterator DoublyLinkedList<ValueType>::iterator()
{
    return Iterator{*this};
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::ConstIterator DoublyLinkedList<ValueType>::constIterator() const
{
    return ConstIterator{*this};
}


template <typename ValueType>
DoublyLinkedList<ValueType>::IteratorBase::IteratorBase(const DoublyLinkedList& list) noexcept 
    : keepInBase(list), currentItr(list.head), position(1)
{
    //the position of first element is 1
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToNext()
{
    if(isPastEnd()) {
        throw IteratorException{};
    }
    else if(isPastStart()) {
        //if itertato is at PastStart positioin
        currentItr = this->keepInBase.head;
    }
    else {
        currentItr = currentItr->next;
    }
    position++;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToPrevious()
{
    if(isPastStart()) {
        throw IteratorException{};
    }
    else if(isPastEnd()) {
        //if itertato is at PastEnd positioin
        currentItr = this->keepInBase.tail;
    }
    else {
        currentItr = currentItr->prev;
    }
    position--;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastStart() const noexcept
{
    if(keepInBase.isEmpty() || position < 1) {
        return true;
    }
    return false;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastEnd() const noexcept
{
    if(keepInBase.isEmpty() || position > keepInBase.size()) {
        return true;
    }
    return false;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::ConstIterator::ConstIterator(const DoublyLinkedList& list) noexcept
    : IteratorBase{list}, keeper(list)
{
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::ConstIterator::value() const
{   
    if(this->isPastStart() || this->isPastEnd()) {
        throw IteratorException{};
    }
    return this->currentItr->value;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::Iterator::Iterator(DoublyLinkedList& list) noexcept
    : IteratorBase{list}, keeper(list)
{
    
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::Iterator::value() const
{   
    if(this->isPastStart() || this->isPastEnd()) {
        throw IteratorException{};
    }
    return this->currentItr->value;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertBefore(const ValueType& value)
{
    if(this->keeper.isEmpty() || this->position < 1) {
        //We cannot insert before the isPastStart boundary
        throw IteratorException{};
    }
    else if(this->position == 1) {
        //if the iterator is at the first element
        this->keeper.addToStart(value);
    }
    else {
        Node* nodeNew;
        nodeNew = new Node;
        nodeNew->value = value;

        Node* previous = this->currentItr->prev;
        previous->next = nodeNew;
        nodeNew->prev = previous;
        nodeNew->next = this->currentItr;
        this->currentItr->prev = nodeNew;
        keeper.listSize++;
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertAfter(const ValueType& value)
{
    if(this->keeper.isEmpty() || this->position > keeper.size()) {
        //We cannot insert after the isPastEnd boundary
        throw IteratorException{};
    }
    else if(this->position == keeper.size()) {
        //if the iterator is at the last element
        this->keeper.addToEnd(value);
    }
    else {
        Node* nodeNew;
        nodeNew = new Node;
        nodeNew->value = value;

        //start connecting nodeNew and other two
        Node* nextOne = this->currentItr->next;
        nextOne->prev = nodeNew;
        nodeNew->next = nextOne;
        nodeNew->prev = this->currentItr;
        this->currentItr->next = nodeNew;
        keeper.listSize++;
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::remove(bool moveToNextAfterward)
{
    if(this->isPastStart() || this->isPastEnd()) {
        //this includes a case where the list is empty.
        throw IteratorException{};
    }
    
    if(this->position == 1) {
        //if iterator is at the first element
        if(moveToNextAfterward) {
            this->currentItr = this->currentItr->next;
        }
        else {
            this->position--;
            this->currentItr = nullptr;
        }
        this->keeper.removeFromStart();
    }
    else if(this->position == this->keeper.size()) {
        //if the iterator is at the last element
        if(moveToNextAfterward) {
            this->currentItr = nullptr;
        }
        else {
            this->moveToPrevious();
        }
        this->keeper.removeFromEnd();
    }
    else {
        //disconnect the node to remove
        Node* before = this->currentItr->prev;
        Node* after = this->currentItr->next;
        before->next = after;
        after->prev = before;

        delete this->currentItr;
        this->keeper.listSize--;

         if(moveToNextAfterward) {
            //in this case position does not change since node will 
            //take the position of the deleted node
            this->currentItr = after;
        }
        else {
            this->currentItr = before;
            this->position--;
        }
    }
}



#endif

