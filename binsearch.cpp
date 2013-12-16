#include <iostream>
#include <cstdio>
using namespace std;

#include "binsearch.h"


Tree::Tree() 
{
  root = 0;
}
void Tree::inorder() 
{
  inorder(root);
}

void Tree::insert(int num)
{
   insert(root, num);
}

Node* Tree::search(int num)
{
   if(root == NULL)
     return NULL;
   return search(root, num);
}

Tree::~Tree()
{
  if(root != NULL)
    postorder(root);
}
void Tree::postorder(Node *q)
{
 if(q!=0) {
    postorder(q->left);
    postorder(q->right);
    delete q;
  }
}
void Tree::inorder(Node *q) const
{
  if(q!=0) {
    inorder(q->left);
    printf("%d ", q->item);
    inorder(q->right);
  }
}

void Tree::insert(Node *&q, int num) {
  Node *temp = q, *prev = q;
  while(temp != 0) {
    prev = temp;
    if(num < temp->item)
      temp = temp->left;
    else
      temp = temp->right;
  }
  
  if(!q) {
    q = new Node(num);
  }
  else{
    if(num < prev->item)
      prev->left = new Node(num);
    else
      prev->right = new Node(num);
  }
}


Node* Tree::search(Node *q, int num)
{
  while(q!=0 && q->item != num) {
    if(num < q->item)
      q = q->left;
    else
      q = q->right;
  }
  return q;
}


