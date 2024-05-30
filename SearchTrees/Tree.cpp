/*
Author: Daniel Gregory
Class: CS 315
Project:Assignment 2
Purpose: Use BST and RBST 
Date: 3/15/24
*/
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Structure for Binary Search Tree (BST) node
struct node {
    int key;
    struct node* left, * right;
};

// Function to create a new BST node
struct node* newNode(int item) {
    struct node* temp = new node;
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// Function to insert a new node with given key in BST
struct node* insert(struct node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

// Function to do inorder traversal of BST and print the nodes
void inorder(struct node* root) {
    if (root != NULL) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

// Function to calculate the maximum depth of the BST
int maxDepth(struct node* node) {
    if (node == NULL)
        return 0;
    else {
        int leftDepth = maxDepth(node->left);
        int rightDepth = maxDepth(node->right);

        return max(leftDepth, rightDepth) + 1;
    }
}
// Find the inorder successor
struct node* minValueNode(struct node* node) {
    struct node* current = node;

    // Find the leftmost leaf
    while (current && current->left != NULL)
        current = current->left;

    return current;
}


// Deleting a node
struct node* deleteNode(struct node* root, int key) {
    // Return if the tree is empty
    if (root == NULL) return root;

    // Find the node to be deleted
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // If the node is with only one child or no child
        if (root->left == NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct node* temp = root->left;
            free(root);
            return temp;
        }

        // If the node has two children
        struct node* temp = minValueNode(root->right);

        // Place the inorder successor in position of the node to be deleted
        root->key = temp->key;

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

// Structure for Red-Black Tree (RBT) node
struct Node {
    int data;
    Node* parent;
    Node* left;
    Node* right;
    int color; // 0 for Black, 1 for Red
};

typedef Node* NodePtr;

// Class for Red-Black Tree (RBT)
class RedBlackTree {
private:
    NodePtr root;
    NodePtr TNULL; // Null leaf node

    // Function to initialize NULL node
    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->data = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0; // Black by default
    }

    // Helper function for preorder traversal
    void preOrderHelper(NodePtr node) {
        if (node != TNULL) {
            cout << node->data << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    // Helper function for inorder traversal
    void inOrderHelper(NodePtr node) {
        if (node != TNULL) {
            inOrderHelper(node->left);
            cout << node->data << " ";
            inOrderHelper(node->right);
        }
    }

    // Helper function for postorder traversal
    void postOrderHelper(NodePtr node) {
        if (node != TNULL) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout << node->data << " ";
        }
    }

    // Helper function to search for a key in the tree
    NodePtr searchTreeHelper(NodePtr node, int key) {
        if (node == TNULL || key == node->data) {
            return node;
        }

        if (key < node->data) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // Helper function to fix the Red-Black Tree after deletion
    void deleteFix(NodePtr x) {
        NodePtr s;
        while (x != root && x->color == 0) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->right->color == 0) {
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else {
                s = x->parent->left;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->left->color == 0) {
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    // Helper function for transplanting nodes
    void rbTransplant(NodePtr u, NodePtr v) {
        if (u->parent == nullptr) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    // Helper function for deleting a node from the Red-Black Tree
    void deleteNodeHelper(NodePtr node, int key) {
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL) {
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }

        if (z == TNULL) {
            cout << "Key not found in the tree" << endl;
            return;
        }

        y = z;
        int yOriginalColor = y->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            }
            else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == 0) {
            deleteFix(x);
        }
    }
    // Helper function to balance the Red-Black Tree after insertion
    void insertFix(NodePtr k) {
        NodePtr u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            }
            else {
                u = k->parent->parent->right;

                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

    // Helper function to print the Red-Black Tree
    void printHelper(NodePtr root, string indent, bool last) {
        if (root != TNULL) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            }
            else {
                cout << "L----";
                indent += "|  ";
            }

            string sColor = root->color ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")" << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }
    public:
        // Constructor
        RedBlackTree() {
            TNULL = new Node;
            TNULL->color = 0;
            TNULL->left = nullptr;
            TNULL->right = nullptr;
            root = TNULL;
        }

        // Function to perform preorder traversal
        void preorder() {
            preOrderHelper(this->root);
        }

        // Function to perform inorder traversal
        void inorder() {
            inOrderHelper(this->root);
        }

        // Function to perform postorder traversal
        void postorder() {
            postOrderHelper(this->root);
        }

        // Function to search for a key in the Red-Black Tree
        NodePtr searchTree(int k) {
            return searchTreeHelper(this->root, k);
        }

        // Function to find the minimum value in the Red-Black Tree
        NodePtr minimum(NodePtr node) {
            while (node->left != TNULL) {
                node = node->left;
            }
            return node;
        }

        // Function to find the maximum value in the Red-Black Tree
        NodePtr maximum(NodePtr node) {
            while (node->right != TNULL) {
                node = node->right;
            }
            return node;
        }

        // Function to find the successor of a given node
        NodePtr successor(NodePtr x) {
            if (x->right != TNULL) {
                return minimum(x->right);
            }

            NodePtr y = x->parent;
            while (y != TNULL && x == y->right) {
                x = y;
                y = y->parent;
            }
            return y;
        }

        // Function to find the predecessor of a given node
        NodePtr predecessor(NodePtr x) {
            if (x->left != TNULL) {
                return maximum(x->left);
            }

            NodePtr y = x->parent;
            while (y != TNULL && x == y->left) {
                x = y;
                y = y->parent;
            }

            return y;
        }

        // Function to perform left rotation
        void leftRotate(NodePtr x) {
            NodePtr y = x->right;
            x->right = y->left;
            if (y->left != TNULL) {
                y->left->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == nullptr) {
                this->root = y;
            }
            else if (x == x->parent->left) {
                x->parent->left = y;
            }
            else {
                x->parent->right = y;
            }
            y->left = x;
            x->parent = y;
        }

        // Function to perform right rotation
        void rightRotate(NodePtr x) {
            NodePtr y = x->left;
            x->left = y->right;
            if (y->right != TNULL) {
                y->right->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == nullptr) {
                this->root = y;
            }
            else if (x == x->parent->right) {
                x->parent->right = y;
            }
            else {
                x->parent->left = y;
            }
            y->right = x;
            x->parent = y;
        }
        // Function to insert a node into the Red-Black Tree
        void insert(int key) {
            NodePtr node = new Node;
            node->parent = nullptr;
            node->data = key;
            node->left = TNULL;
            node->right = TNULL;
            node->color = 1; // New nodes are always RED

            NodePtr y = nullptr;
            NodePtr x = this->root;

            while (x != TNULL) {
                y = x;
                if (node->data < x->data) {
                    x = x->left;
                }
                else {
                    x = x->right;
                }
            }

            node->parent = y;
            if (y == nullptr) {
                root = node;
            }
            else if (node->data < y->data) {
                y->left = node;
            }
            else {
                y->right = node;
            }

            if (node->parent == nullptr) {
                node->color = 0; // If the node is the root, color it black
                return;
            }

            if (node->parent->parent == nullptr) {
                return;
            }

            insertFix(node);
        }

        // Function to get the root of the Red-Black Tree
        NodePtr getRoot() {
            return this->root;
        }

        // Function to delete a node with a given data value from the Red-Black Tree
        void deleteNode(int data) {
            deleteNodeHelper(this->root, data);
        }

        // Function to print the Red-Black Tree
        void printTree() {
            if (root) {
                printHelper(this->root, "", true);
            }
        }
        // Function to calculate the maximum depth of the Red-Black Tree
        int maxDepthRBT(NodePtr node) {
            if (node == nullptr)
                return 0;
            else {
                int leftDepth = maxDepthRBT(node->left);
                int rightDepth = maxDepthRBT(node->right);

                return max(leftDepth, rightDepth) + (node->color == 0 ? 1 : 0);
            }
        }
};

// Main function
int main() {
    RedBlackTree rbTree;
    struct node* bstRoot = nullptr; // Binary Search Tree root

    // Read data from testRandom.csv and insert into Red-Black Tree and Binary Search Tree
    ifstream randomFile("testBad.csv");
    if (!randomFile) {
        cerr << "Error opening testRandom.csv" << endl;
        return 1;
    }

    string line2;
    while (getline(randomFile, line2)) {
        stringstream ss(line2);
        string value;
        while (getline(ss, value, ',')) {
            int key = stoi(value);
            rbTree.insert(key);
            bstRoot = insert(bstRoot, key); // Insert into Binary Search Tree
        }
    }
    randomFile.close();

    // Print the inorder traversal after inserting testRandom.csv into BST
    cout << "Inorder traversal after inserting testRandom.csv into BST:" << endl;
    inorder(bstRoot);
    cout << endl;

    // Calculate and print the max depth of BST after inserting testRandom.csv
    int maxDepthAfterInsertBST = maxDepth(bstRoot);
    cout << "Max depth of BST after inserting testRandom.csv: " << maxDepthAfterInsertBST << endl;

    // Read data from deleteNodes.csv and delete nodes from the BST
    ifstream deleteFile("deleteNodes.csv");
    if (!deleteFile) {
        cerr << "Error opening deleteNodes.csv" << endl;
        return 1;
    }

    while (getline(deleteFile, line2)) {
        int key = stoi(line2);
        bstRoot = deleteNode(bstRoot, key); // Delete from Binary Search Tree
    }
    deleteFile.close();

    // Print the inorder traversal after deleting deleteNodes.csv from BST
    cout << "Inorder traversal after deleting deleteNodes.csv from BST:" << endl;
    inorder(bstRoot);
    cout << endl;

    // Calculate and print the max depth of BST after deleting deleteNodes.csv
    int maxDepthAfterDeleteBST = maxDepth(bstRoot);
    cout << "Max depth of BST after deleting deleteNodes.csv: " << maxDepthAfterDeleteBST << endl;

    // Read data from testRandom.csv again and insert into Red-Black Tree
    ifstream randomFile2("testBad.csv");
    if (!randomFile2) {
        cerr << "Error opening testRandom.csv" << endl;
        return 1;
    }
    string line;
    while (getline(randomFile, line)) {
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            int key = stoi(value);
            rbTree.insert(key);
            bstRoot = insert(bstRoot, key); // Insert into RB Search Tree
        }
    }
    randomFile.close();

    // Print the inorder traversal after inserting testRandom.csv into RB-Tree
    cout << "Inorder traversal after inserting testRandom.csv into RB-Tree:" << endl;
    rbTree.inorder();
    cout << endl;

    // Calculate and print the max depth of RB-Tree after inserting testRandom.csv
    int maxDepthAfterInsertRBT = rbTree.maxDepthRBT(rbTree.getRoot());
    cout << "Max depth of RB-Tree after inserting testRandom.csv: " << maxDepthAfterInsertRBT << endl;

    // Read data from deleteNodes.csv and delete nodes from the RB-Tree
    ifstream deleteFile2("deleteNodes.csv");
    if (!deleteFile2) {
        cerr << "Error opening deleteNodes.csv" << endl;
        return 1;
    }

    while (getline(deleteFile2, line)) {
        int key = stoi(line);
        rbTree.deleteNode(key);
    }
    deleteFile2.close();

    // Print the inorder traversal after deleting deleteNodes.csv from RB-Tree
    cout << "Inorder traversal after deleting deleteNodes.csv from RB-Tree:" << endl;
    rbTree.inorder();
    cout << endl;

    // Calculate and print the max depth of RB-Tree after deleting deleteNodes.csv
    int maxDepthAfterDeleteRBT = rbTree.maxDepthRBT(rbTree.getRoot());
    cout << "Max depth of RB-Tree after deleting deleteNodes.csv: " << maxDepthAfterDeleteRBT << endl;

    return 0;
}