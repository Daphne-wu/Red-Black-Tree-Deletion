//Daphne wu
//Red-black tree insertion
#include <iostream>
#include <cstring>


using namespace std;

// Red-black trees have a certain set of properties:

//  A node is either red or black.
//  The root is black.
//  All leaves (NULL) are black.
//  Both children of every red node are black.
//  Every simple path from a given node to any of its descendant leaves contains the same number of black nodes.

// Add:  Enter a single number which gets inserted into the tree.  Update the tree. (40 points)

// Read:  Read in a series of numbers from a file.  The file should have numbers separated by spaces in a single line.  Add each number sequentially, updating the tree properly after each one. (10 points)

// Print:  Print out a representation of the tree.  It should have the number, red or black value, and parent as part of the print-out.  I would recommend creating a visual representation of the tree, as this will help with debugging. (10 points)


// a struct for node more accessible
struct Node {
	bool isRed;
    int data;
    Node* parent;
    Node* right;
    Node* left;
};

//function prototype for getting relationships of nodes
Node* getParent(Node* n);
Node* getUncle(Node* n);
Node* getGrandParent(Node* n);
bool isonLeft(Node* n);
Node* succesor(Node* n);
Node* BSTreplace(Node* n);
void swapValues(Node *u, Node *v);
//function prototype for insertion/rotation
void rotateLeft(Node* n);
void rotateRight(Node* n);
Node* insertNode(Node* root, Node* n);
void insertRepairTree(Node* n);
void insertRecurse(Node* root, Node* n);

//function prototypes for cases:
void insertCase1(Node* &n);
void insertCase2(Node* &n);
void insertCase3(Node* &n);
void insertCase4(Node* &n);
void insertCase5(Node* &n);

//read, print, and search function prototypes:
void visual(Node* root, int spacing);
void readFile(char file[], Node* &root, int inputNumbers);
Node* search(Node* root, int value);

//deletion ideas from geeksforgeeks
void deleteNode(Node* n, Node* root);
void fixTwoBlack(Node* n, Node* root);
void replaceNode(Node* n, Node* child);




int main() {

	char* input = new char();
	bool running = true;
	Node* root = new Node();

	cout << "The Red-Black Tree program." << endl;

  	while (running) {
		cout << "What would you like to do? Enter: 'ADD', 'READ', 'PRINT', 'SEARCH', 'REMOVE' 'QUIT'. " << endl;
		cin.getline(input, 80, '\n');
		//if they want to add
		if (strcmp(input, "ADD") == 0) {
			int number;
			cout << "Enter the number you would like to add: ";
			cin >> number;
			cin.get();

			//assigns first input number as the root node
			if (root->data == 0) {
				root->data = number;
			}
			else {
				Node* n = new Node();
				n->data = number;
				root = insertNode(root, n);
			}
		}
		// if they would like to remove a node
	  	else if (strcmp(input, "READ") == 0) {
	      char file[20];
	      int inputNumbers;
	      cout << "How many numbers would you like to read from the file into the tree?" << endl;
	      cin >> inputNumbers;
	      //get the null char
	      cin.get();
	      cout << "Enter the file name. Currently available: 'inputNumbers.txt'" << endl;
		  //read until null char
	      cin.getline(file, 80, '\n');
	      //read in and add sequentially the numbers into the tree
	      readFile(file, root, inputNumbers);
	 	}
	 	else if (strcmp(input, "PRINT") == 0) {
	 		//print tree
	 		visual(root, 0);
	 	}
	 	//if the user wants to search a value in tree
	 	else if (strcmp(input, "SEARCH") == 0) {
	    	int number;
			cout << "Enter the number you would like to search: ";
			cin >> number;
			cin.get();

			if (search(root, number) != NULL) {
				cout << number << " is in the tree!" << endl;
			}
			else {
				cout << number << " is not in the tree."  << endl;
			}
	 	}
	 	//if the user wants to remove a value in tree
	 	else if (strcmp(input, "REMOVE") == 0) {
	    	int number;
			cout << "Enter the number you would like to remove: ";
			cin >> number;
			cin.get();
			if (search(root, number) == NULL) {
				cout << number << " is not in the tree" << endl;
			}
			else {
				deleteNode(search(root, number), root);
			}
	 	}
	 	 // else they want to quit the program
	    else if (strcmp(input, "QUIT") == 0) {
	       running = false;
	    	}
		}
		cout << "Thanks!";

  return 0;
}

//Read in a series of numbers from a file.  The file should have numbers separated by spaces in a single line.  Add each number sequentially, updating the tree properly after each one. (10 points)
void readFile(char file[], Node* &root, int inputNumbers) {
	FILE* pfile = NULL;
	char line[1000];
	bool read = true;
	int index = 0;
	//open file with given name and read
	pfile = fopen(file, "r");
	//exit if file is not readable
	if (pfile == NULL) {
		cout << "Oops! There was an error in opening the file." << endl;
		return;
	}
    while(fgets(line, 1000, pfile) && read == true) { //read the line of the file
    	char* buffer = new char[20];
    	memset(buffer, '\0', 20); //set the buffer to the null character
    	int position = 0;
    	while (buffer != NULL && read == true) {
    		buffer = strchr(line + position, ' '); //finds first occurance of the space ands stores it in buffer
    		if (buffer != NULL) { //when there is a returned value to the buffer
        		char lineBuffer[20]; 
        		memset(lineBuffer, '\0', 20); //set the linebuffer to the null character
        		strncpy(lineBuffer, line + position, sizeof(line - buffer)); //copy the string of whats before the occurance of the space into the lineBuffer, (line - buffer is where the information is stored (occurance))
				if (root->data == 0){
					root->data = stoi(lineBuffer);
				}
				else {
					Node* n = new Node();
					n->data = stoi(lineBuffer);
					root = insertNode(root, n);
				}
				index++;
				//if there are as many numbers as inputnumbers then stops
				if (index == inputNumbers) { 
					read = false;
					return;
				}
				position = (buffer - line) + 1;
			}
		}
	}
}

//prints 2d binary tree using spaces to sepreate in between (sideways)
void visual(Node* root, int spacing) {
	if (root != NULL) {
        spacing = spacing + 10;
        visual(root->right, spacing);
        cout << endl;
        for (int i = 10; i < spacing; i++) {
            cout << " ";
        }    
		//if root to be inserted at is red then print the text as red
		if (root->isRed == true) {
			cout << "\033[1m\033[31m" << root->data << endl << "\033[0m";
        }
        else {
            cout << "\033[1m\033[37m" << root->data << endl << "\033[0m";
		}
        visual(root->left, spacing);
    }
}

//return the parent to the node n, parent is set to null for the root node. idea from wikipedia 
Node* getParent(Node* n) {
	if (n == NULL) {
		return NULL;
	}
	else {
		return n->parent;
	}
}

//returns the grandParent of node n, will return null if node is root or child of root
Node* getGrandParent(Node* n) {
	return getParent(getParent(n));
}

Node* getSibling(Node* n) {
	Node* parent = getParent(n);
	//if no parent, there is no sibling
	if (parent == NULL) {
		return NULL;
	}
	if (n == parent->left) {
		return parent->right;
	} 
	else {
		return parent->left;
	}
}
//returns uncle of node n
Node* getUncle(Node* n) {
	Node* parent = getParent(n);
	//if no parent there is also no uncle
	return getSibling(parent);
}

// returns true if node is a left child
bool isonLeft(Node* n) {
    Node* parent = getParent(n);

    if (n == parent->left) {
        return true;
    }
    else {
        return false;
    }
}
// find node that doesn't have a left child in the subtree of the given node 
Node* succesor(Node* n) {
    Node* temp = n;
    while (temp->left != NULL) {
        temp = temp->left;
    }
    return temp;
}

//for left rotation from wikipedia
void rotateLeft(Node* n) {
	Node* newNode = n->right;
	Node* parent = getParent(n);
	if(newNode != NULL ){
		//if the leaves of a red-black tree are emtpy, they can't become internal nodes, so cannot rotate
		//perfom the rotation
		n->right = newNode->left;
		newNode->left = n;
		n->parent = newNode;
		//handle other related pointers
		if (n->right != NULL) {
			n->right->parent = n;
		}
	 	
	 	//if n is the root initially (resets the relationship)
	 	if (parent != NULL) {
	 		if (n == parent->left) {
	 			parent->left = newNode;
	 		}
	 		else if (n == parent->right) {
	 			parent->right = newNode;
	 		}
	 	}
	 	newNode->parent = parent;
	 }
}

//for right rotation (using similar logic as left rotation)
void rotateRight(Node* n) {
	Node* newNode = n->left;
    Node* parent = getParent(n);
    //not using assert()
    if (newNode != NULL) {
        n->left = newNode->right;
        newNode->right = n;
        n->parent = newNode;

        if (n->left != NULL) {
            n->left->parent = n;
        }

        if (parent != NULL) {
            if (n == parent->left) {
                parent->left = newNode;
            }
            else if (n == parent->right) {
                parent->right = newNode;
            }
        }
        newNode->parent = parent;
    }
}

//The below is for insertion ideas from wikipedia

//function to insert a given node n
Node* insertNode(Node* root, Node* n) {
	//insert new node into the current tree
	insertRecurse(root, n);

	//repair tree incase any conditions have been violated
	insertRepairTree(n);

	//find the new root 
	root = n;
	while (getParent(root) != NULL) {
		root = getParent(root);
	}
	return root;
}

// recursively decend the tree until a leaf is found then insert
void insertRecurse(Node* root, Node* n) {
	if (root != NULL) {
		//if the node data is smaller than the root data then it should go down the left side of tree
		if (n->data < root->data) {
			//if there is already a left child
			if (root->left != NULL) {
				insertRecurse(root->left, n);
				return;
			}
			//otherwise if there is no left child, then there is only one node in the tree thus the left child should be the n node
			else {
				root->left = n;
			}
		}
		//otherwise the n->data is >= root->data
		else {  
			if (root->right != NULL) {
				insertRecurse(root->right, n);
				return;
			}
			else {
				root->right = n;
			}
		}
	}
	//now to insert the new node n into the tree
	n->parent = root; //parent would be where we ended off
	n->left = NULL;
	n->right = NULL;
	n->isRed = true;
}


//to repair tree if any part of it is invalid, and makes sure it fits all conditions
void insertRepairTree(Node* n) {
	if (getParent(n) == NULL) {
		insertCase1(n);
	}
	else if (getParent(n)->isRed == false) {
		insertCase2(n);
	}
	else if (getUncle(n) != NULL && getUncle(n)->isRed == true) {
		insertCase3(n);
	}
	else {
		insertCase4(n);
	}
}


//The following are case functions (ideas from wikipedia)

//case 1: we are inserting a node as the root fo the tree. It needs to be painted black to satisfy the properties
void insertCase1(Node* &n) {
	n->isRed = false;
}

//case 2: nothing needs to be done, the tree satisfies all properties
void insertCase2(Node* &n) {
	return;
}

//case 3: the parent and uncle are red and both need to be repainted to black. The grandparent is then repainted red. Then rerun on G to repair because if G is root then must be black
void insertCase3(Node* &n) {
	getParent(n)->isRed = false;
	getUncle(n)->isRed = false;
	getGrandParent(n)->isRed = true;
	insertRepairTree(getGrandParent(n));
}

//case 4: parent is red but uncle is black (either parent's left or right child must be black). Rotate (left or right) new node to parent position. 
void insertCase4(Node* &n) {
	Node* parent = getParent(n);
	Node* grandParent = getGrandParent(n);

	if (n == parent->right && parent == grandParent->left) {
		rotateLeft(parent);
		n = n->left;
	}
	else if (n == parent->left && parent == grandParent->right) {
		rotateRight(parent);
		n = n->right;
	}
	insertCase5(n);
}

//case 5: the parent is red and so is the child, do a right/left rotation to ensure the tree is valid (rotating parent to grandparent position) 
void insertCase5(Node* &n) {
	Node* parent = getParent(n);
	Node* grandParent = getGrandParent(n);

	if (n == parent->left) {
		rotateRight(grandParent);
	}
	else {
		rotateLeft(grandParent);
	}
	parent->isRed = false;
	grandParent->isRed = true;
}


//the following is for deletion:

 // find node that replaces a deleted node in BST 
  Node* BSTreplace(Node* n) { 
    // when node have 2 children 
    if (n->left != NULL && n->right != NULL) 
      return succesor(n->right); 
  
    // when leaf 
    if (n->left == NULL && n->right == NULL) 
      return NULL; 
  
    // when single child 
    if (n->left != NULL) 
      return n->left; 
    else
      return n->right; 
  } 

//swaps values of given nodes
 void swapValues(Node *u, Node *v) { 
    int temp; 
    temp = u->data; 
    u->data = v->data; 
    v->data = temp; 
  } 
  
// fixes tree if there are two black nodes 
void fixTwoBlack(Node* n, Node* root) {
    if (n == root) {
        // you are  at the root
        return;
    }

    Node* sibling = getSibling(n);
    Node* parent = getParent(n);

    if (sibling == NULL) {
        // if  no sibling, two black gets pushed up
        fixTwoBlack(parent, root);
    }
    else {
        if (sibling->isRed == true) {
            // if sibling is red
            parent->isRed = true;
            sibling->isRed = false;
            if (isonLeft(sibling) == true) {
                // left sibling case
                rotateRight(parent);
            }
            else {
                // right sibling case
                rotateLeft(parent);
            }
            fixTwoBlack(n, root);
        }
        else {
            // if sibling black
            if ((sibling->right != NULL && sibling->right->isRed == true) || (sibling->left != NULL && sibling->left->isRed == true)) {// might put null ptr exception
                // if has at least 1 red child
                if (sibling->left != NULL && sibling->left->isRed == true) {
                    if (isonLeft(sibling)) {
                        // left left
                        sibling->left->isRed = sibling->isRed;
                        sibling->isRed = parent->isRed;
                        rotateRight(parent);
                    }
                    else {
                        // right left
                        sibling->left->isRed = parent->isRed;
                        rotateRight(sibling);
                        rotateLeft(parent);
                    }
                }
                else {
                    // left right
                    if (isonLeft(sibling) == true) {
                        sibling->right->isRed = parent->isRed;
                        rotateLeft(sibling);
                        rotateRight(parent);
                    }
                    else {
                        // right left
                        sibling->right->isRed = sibling->isRed;
                        sibling->isRed = parent->isRed;
                        rotateLeft(parent);
                    }
                }
                parent->isRed = false;
            }
            else {
                // 2 black children
                sibling->isRed = true;
                if (parent->isRed == false) {
                    fixTwoBlack(parent, root);
                }
                else {
                    parent->isRed = false;
                }
            }
        }
    }
}


// function to delete a given node
void deleteNode(Node* n, Node* root) {
    Node* u = BSTreplace(n);
    // True when n and u are both black 
    bool nuBlack;
    if (((u == NULL || u->isRed == false) && n->isRed == false)) {
        nuBlack = true;
    }
    else {
        nuBlack = false;
    }
    Node* parent = n->parent;

	// u is NULL this n is leaf
    if (u == NULL) {
    	// n is root, making root null
        if (n == root) {
            root = NULL;
        }
        else {
        	// u and n both black 
         	// n is leaf, fix two black at n 
            if (nuBlack == true) {
                fixTwoBlack(n, root);
            }
            // u or n is red 
            else {           	
                Node* sibling = getSibling(n);
                // sibling is not null, thus make it red
                if (sibling != NULL) {
                    sibling->isRed = true;
                }
            }

            //delete n from the tree
            if (isonLeft(n) == true) {
                parent->left = NULL;
            }
            else {
                parent->right = NULL;
            }
        }
        delete n;
        return;
    }

    // if n has 1 child
    if (n->left == NULL || n->right == NULL) {
    	 // v is root, assign the value of u to n, and delete u 
        if (n == root) {
            n->data = u->data;
            n->left = NULL;
            n->right = NULL;
            delete u;
        }
        // Detach n from tree and move u up 
        else {
            if (isonLeft(n) == true) {
                parent->left = u;
            }
            else {
                parent->right = u;
            }
            delete n;
            u->parent = parent;
             // u and n both black, fix two black at u
            if (nuBlack == true) {
                fixTwoBlack(u, root);
            }
             // u or n red, color u black
            else {
                u->isRed = false;
            }
        }
        return;
    }
    // n has 2 children, swap values with successor and recurse
    swapValues(u, n);
    deleteNode(u, root);
}


//searches and returns the node if found
Node* search(Node* root, int value) {
	Node* current = root;
	//if value is larger than the root then goes to the right, otherwise goes to the left
	if (current != NULL) {
		if (current->data == value) {
			return current;
		}
		else if (current->data < value) {
			current = search(current->right, value);
		}
		else {
			current = search(current->left, value);
		}
	}
	if (current == root) {
		return NULL;
	}
	else {
		return current;
	}
}
