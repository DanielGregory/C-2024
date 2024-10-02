Purpose
This project implements a Binary Search Tree (BST) and a Red-Black Tree (RBT) in C++. It demonstrates basic operations such as insertion, deletion, traversal, and calculating the maximum depth of both tree types.

Files Included
main.cpp: The main source file containing the implementation of the BST and RBT, along with the logic for reading data from CSV files and performing various operations.
testRandom.csv: A sample CSV file used to test the insertion of random values into the trees.
deleteNodes.csv: A CSV file containing values to be deleted from the trees.

Features

Binary Search Tree (BST)
- Insertion of nodes
- Deletion of nodes
- Inorder traversal
- Calculation of maximum depth

Red-Black Tree (RBT)
- Insertion of nodes with balancing
- Deletion of nodes with rebalancing
- Inorder traversal
- Calculation of maximum depth
  
Usage
1. Compile the program using a C++ compiler:
    g++ -o bst_rbt main.cpp
2. Run the executable:
    ./bst_rbt
3. The program will read from testRandom.csv to insert values into both the BST and RBT, then it will read from deleteNodes.csv to remove specified values from both trees. The program outputs the inorder traversal and the maximum depth after each set of operations.
