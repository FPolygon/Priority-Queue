/* This file contains the implementation of a priority queue class template. The priority queue is implemented using a custom binary search tree (BST). Each node in the BST contains a priority value and a corresponding value. The priority queue supports insertion, removal, and retrieval of elements based on their priority. Duplicate priorities are also supported, and the priority queue maintains the order of elements with the same priority using a linked list. The class provides various member functions such as enqueue, dequeue, clear, begin, next, size, toString, and peek. The priority queue is implemented as a template, allowing it to store elements of any type. This file also includes necessary header files, such as iostream, sstream, and set, and provides a default constructor, destructor, and assignment operator for the priority queue class. The code is a part of the CS 251 Spring 2023 course at the University of Illinois Chicago. */

#pragma once

#include <iostream>
#include <sstream>
#include <set>

using namespace std;

template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority;  // used to build BST
        T value;  // stored data for the p-queue
        bool dup;  // marked true when there are duplicate priorities
        NODE* parent;  // links back to parent
        NODE* link;  // links to linked list of NODES with duplicate priorities
        NODE* left;  // links to left child
        NODE* right;  // links to right child
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of elements in the pqueue
    NODE* curr;  // pointer to next item in pqueue (see begin and next)
    
public:
    //
    // default constructor:
    //
    // Creates an empty priority queue.
    // O(1)
    //
    priorityqueue() {
        root = nullptr;
        size = 0;
        curr = nullptr;
    }
    
    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    // Sets all member variables appropriately.
    // O(n), where n is total number of nodes in custom BST
    //
    priorityqueue& operator=(const priorityqueue& other) {
        if (this != &other) {
            // Clear the current tree
            clear();

            // Copy the other tree
            copyTree(other.root);

            // Copy the size and curr pointers
            size = other.size;
            if (other.curr != nullptr) {
                curr = findNode(root, other.curr->priority);
            } else {
                curr = nullptr;
            }
        }
        return *this;
    }

    // Helper function to copy a tree from another tree
    void copyTree(NODE* otherNode) {
        if (otherNode != nullptr) {
            // Enqueue the node in the current tree
            enqueue(otherNode->value, otherNode->priority);

            // If the node has duplicates, enqueue them as well
            if (otherNode->dup) {
                NODE* otherLink = otherNode->link;
                while (otherLink != nullptr) {
                    enqueue(otherLink->value, otherLink->priority);
                    otherLink = otherLink->link;
                }
            }

            // Recursively copy the left and right subtrees
            copyTree(otherNode->left);
            copyTree(otherNode->right);
        }
    }
    
    // This function takes a pointer to a node in the tree and a priority value to search for.
    // If the node is null or the priority of the node matches the search priority, the node is returned.
    // Otherwise, the function recursively searches either the left or right subtree depending on the priority value.
    NODE* findNode(NODE* node, int priority) {
        if (node == nullptr || node->priority == priority) {
            return node;
        } else if (priority < node->priority) {
            return findNode(node->left, priority);
        } else {
            return findNode(node->right, priority);
        }
    }
    
    
    
    //
    // clear:
    //
    // Frees the memory associated with the priority queue but is public.
    // O(n), where n is total number of nodes in custom BST
    //
    void clear() {
        clearHelper(root);

        // Reset member variables
        root = nullptr;
        size = 0;
        curr = nullptr;
    }

    void clearHelper(NODE* node) {
        if (node != nullptr) {
            // Recursively clear the left and right subtrees
            clearHelper(node->left);
            clearHelper(node->right);

            // Delete the current node and any linked list nodes
            if (node->link != nullptr) {
                NODE* linkNode = node->link;
                while (linkNode != nullptr) {
                    NODE* nextNode = linkNode->link;
                    delete linkNode;
                    linkNode = nextNode;
                }
            }
            delete node;
        }
    }
    
    //
    // destructor:
    //
    // Frees the memory associated with the priority queue.
    // O(n), where n is total number of nodes in custom BST
    //
    ~priorityqueue() {
        clear();
    }
    
    //
    // enqueue:
    //
    // Inserts the value into the custom BST in the correct location based on
    // priority.
    // O(logn + m), where n is number of unique nodes in tree and m is number
    // of duplicate priorities
    //
    void enqueue(T value, int priority) {
        // Create a new node with the given value and priority
        NODE* newNode = new NODE;
        newNode->priority = priority;
        newNode->value = value;
        newNode->dup = false;
        newNode->parent = nullptr;
        newNode->link = nullptr;
        newNode->left = nullptr;
        newNode->right = nullptr;

        // If the tree is empty, set newNode as root
        if (root == nullptr) {
            root = newNode;
            size++;
            return;
        }

        NODE* currNode = root;
        NODE* prevNode = nullptr;

        // Traverse the tree to find the correct location for newNode
        while (currNode != nullptr) {
            if (priority == currNode->priority) {
                // If the priority already exists in the tree, add newNode to its linked list
                newNode->dup = true;
                while (currNode->link != nullptr) {
                    currNode = currNode->link;
                }
                currNode->link = newNode;

                size++;
                return;
            }
            prevNode = currNode;
            if (priority < currNode->priority) {
                currNode = currNode->left;
            } else {
                currNode = currNode->right;
            }
        }

        // Attach newNode to the correct leaf node
        newNode->parent = prevNode;
        if (priority < prevNode->priority) {
            prevNode->left = newNode;
        } else {
            prevNode->right = newNode;
        }
        size++;
    }

    
    //
    // dequeue:
    //
    // returns the value of the next element in the priority queue and removes
    // the element from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number
    // of duplicate priorities
    //
    T dequeue() {
        if (root == nullptr) {
            throw logic_error("Cannot dequeue from an empty priority queue");
        }

        // Traverse the tree to find the node with the highest priority
        NODE* node = root;
        while (node->left != nullptr) {
            node = node->left;
        }
        if (node->link != nullptr) {
            // If there are duplicates with the same priority, remove the first one
            if (node == root) {
                root = node->link;
                T value = node->value;
                delete node;
                size--;
                return value;
            }
            if (node->parent->left == node) {
                node->parent->left = node->link;
            } else {
                node->parent->right = node->link;
            }
            node->link->parent = node->parent;
            T value = node->value;
            delete node;
            size--;
            return value;
        } else {
            // If there are no duplicates, remove the node
            T value = node->value;
            removeNode(node);
            size--;
            return value;
        }
    }

    void removeNode(NODE* node) {
        if (node == root) {
            if (node->right != nullptr) {
                root = node->right;
                return;
            } else {
                return;
            }
        }
        if (node->left == nullptr && node->right == nullptr) {
            // If the node has no children, just remove it
            if (node->parent == nullptr) {
                // If the node is the root, set root to nullptr
                root = nullptr;
            } else {
                // Otherwise, remove the link to this node from its parent
                if (node->parent->left == node) {
                    node->parent->left = nullptr;
                } else {
                    node->parent->right = nullptr;
                }
            }
            delete node;
        } else if (node->left == nullptr || node->right == nullptr) {
            // If the node has only one child, replace it with that child
            NODE* childNode = (node->left == nullptr ? node->right : node->left);
            childNode->parent = node->parent;
            if (node->parent == nullptr) {
                // If the node is the root, set root to the child
                root = childNode;
            } else {
                // Otherwise, replace the link to this node with a link to its child
                if (node->parent->left == node) {
                    node->parent->left = childNode;
                } else {
                    node->parent->right = childNode;
                }
            }
            delete node;
        } else {
            // If the node has two children, replace it with the successor
            NODE* successorNode = node->left;
            while (successorNode->right != nullptr) {
                successorNode = successorNode->right;
            }
            node->priority = successorNode->priority;
            node->value = successorNode->value;
            removeNode(successorNode);
        }
    }
    
    
    
    //
    // Size:
    //
    // Returns the # of elements in the priority queue, 0 if empty.
    // O(1)
    //
    int Size() {
        return size;
    }
    
    
    //
    // begin
    //
    // Resets internal state for an inorder traversal.  After the
    // call to begin(), the internal state denotes the first inorder
    // node; this ensure that first call to next() function returns
    // the first inorder node value.
    //
    // O(logn), where n is number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    void begin() {
        if (root == nullptr) {
            curr = nullptr;
            return;
        }
        curr = root;
        while (curr->left != nullptr) {
            curr = curr->left;
        }
    }

    
    
    //
    // next
    //
    // Uses the internal state to return the next inorder priority, and
    // then advances the internal state in anticipation of future
    // calls.  If a value/priority are in fact returned (via the reference
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more values/priorities are available.  This is the end of the
    // inorder traversal.
    //
    // O(logn), where n is the number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (pq.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    //
    bool next(T& value, int& priority) {
            if (root == nullptr) {
                return false;
            }
            if (curr == nullptr) {
                // If curr is null, start from the root
                curr = root;
                while (curr->left != nullptr) {
                    curr = curr->left;
                }
                value = curr->value;
                priority = curr->priority;
                return true;
            }
            NODE* nextNode = nullptr;
            if (curr->right != nullptr) {
                // If curr has a right subtree, go to the leftmost node in the right subtree
                nextNode = curr->right;
                while (nextNode->left != nullptr) {
                    nextNode = nextNode->left;
                }
            } else {
                // Otherwise, go up the tree until we find a node that is a left child
                NODE* parentNode = curr->parent;
                while (parentNode != nullptr && curr == parentNode->right) {
                    curr = parentNode;
                    parentNode = parentNode->parent;
                }
                nextNode = parentNode;
            }
            if (nextNode == nullptr) {
                return false;
            }
            curr = nextNode;
            value = curr->value;
            priority = curr->priority;
            return true;
        }




    
    
    //
    // toString:
    //
    // Returns a string of the entire priority queue, in order.  Format:
    // "1 value: Ben
    //  2 value: Jen
    //  2 value: Sven
    //  3 value: Gwen"
    //
    string toString() {
        stringstream ss;
        inorderPrint(root, ss);
        return ss.str();
    }
    
    void inorderPrint(NODE* node, stringstream& ss) {
        if (node != nullptr) {
            inorderPrint(node->left, ss);
            ss << node->priority << " value: " << node->value << endl;
            if (node->link != nullptr) {
                NODE* curr = node->link;
                while (curr != nullptr) {
                    ss << node->priority << " value: " << curr->value << endl;
                    curr = curr->link;
                }
            }
            inorderPrint(node->right, ss);
        }
    }
    
    
    //
    // peek:
    //
    // returns the value of the next element in the priority queue but does not
    // remove the item from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number
    // of duplicate priorities
    //
    T peek() {
        NODE* currNode = root;
        while (currNode->left != nullptr) {
            currNode = currNode->left;
        }
        return currNode->value;
    }
    
    //
    // ==operator
    //
    // Returns true if this priority queue as the priority queue passed in as
    // other.  Otherwise returns false.
    // O(n), where n is total number of nodes in custom BST
    //
    bool operator==(const priorityqueue<T>& other) const {
        // If the sizes of the two priority queues are different, they are not equal
        if (size != other.size) {
            return false;
        }

        // Create two pointers to traverse the two priority queues
        NODE* thisPtr = root;
        NODE* otherPtr = other.root;

        // Traverse the two priority queues using in-order traversal until we have
        // compared all the elements in both priority queues or we find a pair of
        // elements that are not equal
        while (thisPtr != nullptr && otherPtr != nullptr) {
            // If the priorities of the two nodes are different, the priority queues
            // are not equal
            if (thisPtr->priority != otherPtr->priority) {
                return false;
            }

            // If the values of the two nodes are different, the priority queues are
            // not equal
            if (thisPtr->value != otherPtr->value) {
                return false;
            }

            // If the current nodes have duplicates, compare the linked lists
            if (thisPtr->link != nullptr && otherPtr->link != nullptr) {
                NODE* thisLink = thisPtr->link;
                NODE* otherLink = otherPtr->link;
                while (thisLink != nullptr && otherLink != nullptr) {
                    if (thisLink->priority != otherLink->priority) {
                        return false;
                    }
                    if (thisLink->value != otherLink->value) {
                        return false;
                    }
                    thisLink = thisLink->link;
                    otherLink = otherLink->link;
                }
                if (thisLink != nullptr || otherLink != nullptr) {
                    return false;
                }
            } else if (thisPtr->dup != otherPtr->dup) {
                // If one of the nodes has duplicates and the other does not, the
                // priority queues are not equal
                return false;
            }

            // Move to the next node in the two priority queues using in-order
            // traversal
            if (thisPtr->right != nullptr && otherPtr->right != nullptr) {
                // If both nodes have a right child, move to the leftmost node in
                // the right subtree
                thisPtr = leftmostNode(thisPtr->right);
                otherPtr = leftmostNode(otherPtr->right);
            } else if (thisPtr->right != nullptr || otherPtr->right != nullptr) {
                // If one node has a right child and the other does not, the
                // priority queues are not equal
                return false;
            } else {
                // If neither node has a right child, move up the tree until we
                // find a node that has not been visited yet
                while (thisPtr->parent != nullptr && thisPtr == thisPtr->parent->right) {
                    thisPtr = thisPtr->parent;
                    otherPtr = otherPtr->parent;
                }
                thisPtr = thisPtr->parent;
                otherPtr = otherPtr->parent;
            }
        }

        // If we have reached the end of one priority queue but not the other,
        // the priority queues are not equal
        if (thisPtr != nullptr || otherPtr != nullptr) {
            return false;
        }

        // If we have not found any differences between the two priority queues,
        // they are equal
        return true;
    }

    NODE* leftmostNode(NODE* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    
    //
    // getRoot - Do not edit/change!
    //
    // Used for testing the BST.
    // return the root node for testing.
    //
    void* getRoot() {
        return root;
    }
};
