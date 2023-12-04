#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

class RBT { //RedBlack Tree structure with left and right node attributes and variables for comment, likes, and replies
private:
    struct Node {
        //default value initialization
        string comment = "";
        string color = "red";
        int replies = 0;
        int likes = 0;

        Node* left;
        Node* right;
        Node* parent = nullptr;
        Node(string x, int y, int z) : comment(x), replies(y), likes(z), left(nullptr), right(nullptr) {} //constructor
    };

    Node* root = nullptr; //root node instantiation
    void fixViolations(Node* node); //private function declarations
    void leftRotation(Node* node);
    void rightRotation(Node* node);
public:
    void insert(string comment, int replies, int likes); //public function declarations
    RBT::Node* getRoot();
    vector<string> mostLiked(Node* root);
    void readFile(RBT& myRBT);
};

void RBT::insert(string comment, int replies, int likes)
{
    Node* newNode = new Node(comment, replies, likes); //defining node values for insert
    Node* parent = nullptr;
    Node* current = root;
    while (current != nullptr)
    {
        parent = current;
        if (newNode->likes < current->likes) //comment like is less than previous node, go left
        {
            current = current->left;
        }
        else if (newNode->likes > current->likes) //comment like is greater than previous node, go right
        {
            current = current->right;
        }
        else {
            current = current->right; //comment like amount is equal, go right
        }
    }
    newNode->parent = parent;
    if (parent == nullptr)
    {
        root = newNode; // This is the first node added to the tree
    }
    else if (newNode->likes < parent->likes) //comment like less, go left
    {
        parent->left = newNode;
    }
    else if (newNode->likes > parent->likes) //comment like greater, go right
    {
        parent->right = newNode;
    }
    else
    {
        parent->right = newNode; //comment like amount is equal, hardcoded to go right
    }
    fixViolations(newNode); //call function to fix RedBlack tree violations in the tree
}

void RBT::fixViolations(Node* node)
{
    while ((node != root) and (node->parent->color == "red"))
    {
        if ((node->parent == node->parent->parent->left))
        {
            Node* uncle = node->parent->parent->right; //assign node to preform operation
            if ((uncle != nullptr) and (uncle->color == "red"))
            {
                node->parent->color = "black"; //set colors of nodes and assign node swaps
                uncle->color = "black";
                node->parent->parent->color = "red";
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right) //other case, preform left rotation on parent
                {
                    node = node->parent;
                    leftRotation(node);
                }
                node->parent->color = "black"; //assign colors and right rotate grandparent
                node->parent->parent->color = "red";
                rightRotation(node->parent->parent);
            }
        }
        else
        {
            Node* uncle = node->parent->parent->left; //assign node to preform operation
            if ((uncle != nullptr) and (uncle->color == "red"))
            {
                node->parent->color = "black"; //set colors of nodes and assign node swaps
                uncle->color = "black";
                node->parent->parent->color = "red";
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left) //other case, preform right rotation on parent
                {
                    node = node->parent;
                    rightRotation(node);
                }
                node->parent->color = "black"; //assign colors and left rotate grandparent
                node->parent->parent->color = "red";
                leftRotation(node->parent->parent);
            }
        }
    }
    root->color = "black";
}

void RBT::leftRotation(Node* node)
{
    Node* rChild = node->right; //assign nodes
    node->right = rChild->left;
    if (rChild->left != nullptr) //check nodes exist
    {
        rChild->left->parent = node;
    }
    rChild->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = rChild;
    }
    else if (node == node->parent->left) //assign node children respectively
    {
        node->parent->left = rChild;
    }
    else
    {
        node->parent->right = rChild;
    }
    rChild->left = node;
    node->parent = rChild;
}

void RBT::rightRotation(Node* node)
{
    Node* lChild = node->left; //assign nodes
    node->left = lChild->right;
    if (lChild->right != nullptr) //check nodes exist
    {
        lChild->right->parent = node;
    }
    lChild->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = lChild;
    }
    else if (node == node->parent->left) //assign node children respectively
    {
        node->parent->left = lChild;
    }
    else
    {
        node->parent->right = lChild;
    }
    lChild->right = node;
    node->parent = lChild;
}

RBT::Node* RBT::getRoot() //return root for most_liked function
{
    return root;
}

vector<string> RBT::mostLiked(Node* root) //return right most node of tree which has most likes
{
    vector<string> comment_attributes; //declare vector to hold comment attributes
    Node* current = root;
    while (current->right != nullptr)
    {
        current = current->right;
    }
    comment_attributes.push_back(current->comment); //pushback values to vector
    comment_attributes.push_back(to_string(current->likes));
    comment_attributes.push_back(to_string(current->replies));
    return comment_attributes; //return vector for display in GUI
}