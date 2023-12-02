#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

class RBT {
private:
    struct Node {
        string comment = "";
        string color = "red";
        int replies = 0;
        int likes = 0;

        Node* left;
        Node* right;
        Node* parent = nullptr;
        Node(string x, int y, int z) : comment(x), replies(y), likes(z), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;
    void fixViolations(Node* node);
    void leftRotation(Node* node);
    void rightRotation(Node* node);
public:
    void insert(string comment, int replies, int likes);
    RBT::Node* getRoot();
    vector<string> mostLiked(Node* root);
    void readFile(RBT& myRBT);
};

void RBT::insert(string comment, int replies, int likes)
{
    Node* newNode = new Node(comment, replies, likes);
    Node* parent = nullptr;
    Node* current = root;
    while (current != nullptr)
    {
        parent = current;
        if (newNode->likes < current->likes)
        {
            current = current->left;
        }
        else if (newNode->likes > current->likes)
        {
            current = current->right;
        }
        else {
            current = current->right; // comment like amount is equal, hardcoded to go right for now
        }
    }
    newNode->parent = parent;
    if (parent == nullptr)
    {
        root = newNode; // This is the first node added to the tree
    }
    else if (newNode->likes < parent->likes)
    {
        parent->left = newNode;
    }
    else if (newNode->likes > parent->likes)
    {
        parent->right = newNode;
    }
    else
    {
        parent->right = newNode; // comment like amount is equal, hardcoded to go right for now
    }
    fixViolations(newNode);
}

void RBT::fixViolations(Node* node)
{
    while ((node != root) and (node->parent->color == "red"))
    {
        if ((node->parent == node->parent->parent->left))
        {
            Node* uncle = node->parent->parent->right;
            if ((uncle != nullptr) and (uncle->color == "red"))
            {
                node->parent->color = "black";
                uncle->color = "black";
                node->parent->parent->color = "red"; //may not be needed
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    leftRotation(node);
                }
                node->parent->color = "black";
                node->parent->parent->color = "red";
                rightRotation(node->parent->parent);
            }
        }
        else
        {
            Node* uncle = node->parent->parent->left;
            if ((uncle != nullptr) and (uncle->color == "red"))
            {
                node->parent->color = "black";
                uncle->color = "black";
                node->parent->parent->color = "red"; //may not be needed
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rightRotation(node);
                }
                node->parent->color = "black";
                node->parent->parent->color = "red";
                leftRotation(node->parent->parent);
            }
        }
    }
    root->color = "black";
}

void RBT::leftRotation(Node* node)
{
    Node* rChild = node->right;
    node->right = rChild->left;
    if (rChild->left != nullptr)
    {
        rChild->left->parent = node;
    }
    rChild->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = rChild;
    }
    else if (node == node->parent->left)
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
    Node* lChild = node->left;
    node->left = lChild->right;
    if (lChild->right != nullptr)
    {
        lChild->right->parent = node;
    }
    lChild->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = lChild;
    }
    else if (node == node->parent->left)
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

RBT::Node* RBT::getRoot()
{
    return root;
}

vector<string> RBT::mostLiked(Node* root)
{
    vector<string> comment_attributes;
    Node* current = root;
    while (current->right != nullptr)
    {
        current = current->right;
    }
    comment_attributes.push_back(current->comment);
    comment_attributes.push_back(to_string(current->likes));
    comment_attributes.push_back(to_string(current->replies));
    return comment_attributes;
    //cout << "The most liked comment has " << current->likes << " likes." << endl << "The comment is: " << current->comment << endl;
}