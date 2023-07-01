// AVLSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"


template <typename ElementType>
class AVLSet : public Set<ElementType>
{

public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node {
        ElementType data;
        Node* left;
        Node* right;
        int distanceFromLeaf;  //distance from leaf node
    };
    Node* root;
    //int treeHeight;
    int treeSize;
    bool haveToBalance; //enable balance function is this is true

    //helper functions
    void copyNode(Node*& current, Node*& insertPos);
    struct Node* insertNode(Node*& current, Node*& node);
    struct Node* rightRotation(Node*& rotationPoint);
    struct Node* leftRotation(Node*& rotationPoint);
    void updateDistanceFromLeaf(Node*& node);
    int getRightLeftDifference(Node*& node);
    void preorderHelp(VisitFunction visit, Node*& current) const;
    void inorderHelp(VisitFunction visit, Node*& current) const;
    void postorderHelp(VisitFunction visit, Node*& current) const;
    void cleaner(Node*& current);
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    root = nullptr;
    treeSize = 0;
    haveToBalance = shouldBalance;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    //traverse all nodes and delete them
    if(root != nullptr) {
        cleaner(root);
    }
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    if(s.root != nullptr) {
        Node* node = new Node;
        node->data = s.root->data;
        node->left = nullptr;
        node->right = nullptr;
        node->distanceFromLeaf = s.root->distanceFromLeaf;
        treeSize = 1;
        haveToBalance = true;

        root = node;

        if(s.root->left != nullptr) {
            Node* leftBelow = new Node;
            root->left = leftBelow;
            treeSize++;
            copyNode(s.root->left, root->left);
        }
        
        if(s.root->right != nullptr) {
            Node* rightBelow = new Node;
            root->right = rightBelow;
            treeSize++;
            copyNode(s.root->right, root->right);
        }
    }
    else {
        root = nullptr;
        treeSize = 0;
        haveToBalance = true;
    }
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    root = s.root;
    treeSize = s.treeSize;
    haveToBalance = s.haveToBalance;

    s.root = nullptr;
    s.treeSize = 0;
    s.haveToBalance = false;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this == &s) {  //if it is self assigning
        return *this;
    }

    haveToBalance = s.haveToBalance;

    if(s.root != nullptr) {
        if(root == nullptr) {
            //if the tree is empty
            Node* node = new Node;
            node->data = s.root->data;
            node->left = nullptr;
            node->right = nullptr;
            node->distanceFromLeaf = s.root->distanceFromLeaf;
            root = node;
        }
        else {
            root->data = s.root->data;
            root->distanceFromLeaf = s.root->distanceFromLeaf;
        }

        treeSize = 1;

        if(s.root->left != nullptr) {
            Node* leftBelow = new Node;
            root->left = leftBelow;
            treeSize++;
            copyNode(s.root->left, root->left);
        }
        
        if(s.root->right != nullptr) {
            Node* rightBelow = new Node;
            root->right = rightBelow;
            treeSize++;
            copyNode(s.root->right, root->right);
        }
    }

    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    Node* temp = root;
    int numTemp = treeSize;
    bool hTB = haveToBalance;

    root = s.root;
    treeSize = s.treeSize;
    haveToBalance = s.haveToBalance;

    s.root = temp;
    s.treeSize = numTemp;
    s.haveToBalance = hTB;

    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    //this function is for instructor-use
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{   
    Node* node = new Node;
    node->data = element;
    node->left = nullptr;
    node->right = nullptr;
    node->distanceFromLeaf = 0;

    if(root == nullptr) {
        //if the tree is empty
        root = node;
    }
    else {
        root = insertNode(root, node);
        updateDistanceFromLeaf(root);
    }

    treeSize++;
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{   
    if(root != nullptr) {  //if the tree is not empty
        Node* current = root;
        while (current != nullptr)
        {
            if(current->data == element){
                return true;
            }
            else if(current->data < element) {
                current = current->right;
            }
            else {
                current = current->left;
            }
        }
    }
    
    return false;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return treeSize;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    if(root == nullptr) {
        return -1;
    }
    else {
        return root->distanceFromLeaf;
    }
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    visit(root->data);

    if(root->left != nullptr) {
        preorderHelp(visit, root->left);
    }

    if(root->right != nullptr) {
        preorderHelp(visit, root->right);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    if(root->left != nullptr) {
        inorderHelp(visit, root->left);
    }

    visit(root->data);

    if(root->right != nullptr) {
        inorderHelp(visit, root->right);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    if(root->left != nullptr) {
        postorderHelp(visit, root->left);
    }

    if(root->right != nullptr) {
        postorderHelp(visit, root->right);
    }

    visit(root->data);
}

//Helper functions
template <typename ElementType>
void AVLSet<ElementType>::copyNode(Node*& current, Node*& insertPos) {

    insertPos->data = current->data;
    insertPos->left = nullptr;
    insertPos->right = nullptr;
    insertPos->distanceFromLeaf = current->distanceFromLeaf;

    if(current->left != nullptr) {
        Node* leftBelow = new Node;
        insertPos->left = leftBelow;
        treeSize++;
        copyNode(current->left, insertPos->left);
    }
    
    if(current->right != nullptr) {
        Node* rightBelow = new Node;
        insertPos->right = rightBelow;
        treeSize++;
        copyNode(current->right, insertPos->right);
    }
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::insertNode(Node*& current, Node*& node)
{
    if (current == nullptr) {
        return node;
    }
    else if(current->data == node->data) {
        return current;
    }
    else if(node->data < current->data) {
        //go to the left
        current->left = insertNode(current->left, node);
    }
    else if(node->data > current->data) {
        //go to the right
        current->right = insertNode(current->right, node);
    }

    //Modify distance from leaf as we back up
    updateDistanceFromLeaf(current);
    
    if(haveToBalance) {
        int diffBtwRL = getRightLeftDifference(current);

        //check for any rotation
        if(diffBtwRL > 1) {
            //RR or RL
            if(getRightLeftDifference(current->right) > 0) {
                //RR
                return leftRotation(current);
            }
            else { //RL
                current->right = rightRotation(current->right);
                return leftRotation(current);
            }
        }
        else if(diffBtwRL < -1) {
            //LL or LR
            if(getRightLeftDifference(current->left) > 0) {
                //LR
                current->left = leftRotation(current->left);
                return rightRotation(current);
            }
            else {
                //LL
                return rightRotation(current);
            }
        }
    }
    return current;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::rightRotation(Node*& rotationPoint)
{
    Node* nextRoot = rotationPoint->left;
    Node* keeper = nextRoot->right;

    nextRoot->right = rotationPoint;
    rotationPoint->left = keeper;

    //update distanceFromLeaf
    updateDistanceFromLeaf(rotationPoint);
    updateDistanceFromLeaf(nextRoot);

    return nextRoot;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::leftRotation(Node*& rotationPoint)
{
    Node* nextRoot = rotationPoint->right;
    Node* keeper = nextRoot->left;
    
    nextRoot->left = rotationPoint;
    rotationPoint->right = keeper;

    //update distanceFromLeaf
    updateDistanceFromLeaf(rotationPoint);
    updateDistanceFromLeaf(nextRoot);

    return nextRoot;
}

template <typename ElementType>
void AVLSet<ElementType>::updateDistanceFromLeaf(Node*& node) 
{
    if(node->left == nullptr && node->right == nullptr) {
        //if the node is leaf
        node->distanceFromLeaf = 0;
    }
    else {
        if(node->left == nullptr) {
            node->distanceFromLeaf = node->right->distanceFromLeaf + 1;
        }
        else if(node->right == nullptr) {
            node->distanceFromLeaf = node->left->distanceFromLeaf + 1;
        }
        else if(node->left->distanceFromLeaf > node->right->distanceFromLeaf) {
            node->distanceFromLeaf = node->left->distanceFromLeaf + 1;
        }
        else {
            node->distanceFromLeaf = node->right->distanceFromLeaf + 1;
        }
    }
}

template <typename ElementType>
int AVLSet<ElementType>::getRightLeftDifference(Node*& node) {
    //if the right subtree has more children, than this function will return
    //a postive value.
    if(node->left == nullptr && node->right == nullptr) {
        return 0;
    }
    else if(node->left == nullptr) {
        return node->right->distanceFromLeaf + 1;
    }
    else if(node->right == nullptr) {
        return  0 - node->left->distanceFromLeaf - 1;
    }
    else {
        return node->right->distanceFromLeaf - node->left->distanceFromLeaf;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::preorderHelp(VisitFunction visit, Node*& current) const {
    visit(current->data);

    if(current->left != nullptr) {
        preorderHelp(visit, current->left);
    }

    if(current->right != nullptr) {
        preorderHelp(visit, current->right);
    }
}

template <typename ElementType>
void  AVLSet<ElementType>::inorderHelp(VisitFunction visit, Node*& current) const {

    if(current->left != nullptr) {
        inorderHelp(visit, current->left);
    }
    
    visit(current->data);

    if(current->right != nullptr) {
        inorderHelp(visit, current->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorderHelp(VisitFunction visit, Node*& current) const {

    if(current->left != nullptr) {
        postorderHelp(visit, current->left);
    }

    if(current->right != nullptr) {
        postorderHelp(visit, current->right);
    }

    visit(current->data);
}

template <typename ElementType>
void AVLSet<ElementType>::cleaner(Node*& current) {

    if(current->left != nullptr) {
        cleaner(current->left);
    }

    if(current->right != nullptr) {
        cleaner(current->right);
    }

    delete current;
}

#endif