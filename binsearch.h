#ifndef BINSEARCH_H
#define BINSEARCH_H
#include <iostream>
#include <cstdio>
using namespace std;

class Node{

public:
  int item;
  Node *left;
  Node *right;
  Node(int num, Node *l = 0, Node *r = 0) {
    item = num; left= l; right = r;
  }
};


class Tree{
  Node *root;
  // private methods
  Node* search(Node *m, int num);
  void insert(Node *&, int num); // CALL BY REFERENCE.
  void inorder(Node *) const;
  void postorder(Node *);
// public methods
public:
  Tree();
  ~Tree();
  void inorder() ;
  void insert(int num);
  
  Node* search(int num);

};

#endif
