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

//function prototype for getting parent uncle and grandparent.
Node* getParent(Node* n);
Node* getUncle(Node* n);
Node* getGrandParent(Node* n);

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

//deletion ideas from wikipedia
void deleteCase1(Node* n);
void deleteCase2(Node* n);
void deleteCase3(Node* n);
void deleteCase4(Node* n);
void deleteCase5(Node* n);
void deleteCase6(Node* n);
void replaceNode(Node* n, Node* child);
void deleteOneChild(Node* n);



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
				deleteOneChild(search(root, number));
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

//substitutes child into n's place in the tree
void replaceNode(Node* n, Node* child) {
	child->parent = n->parent;
	if (n == n->parent->left) {
		n->parent->left = child;
	}
	else {
		n->parent->right = child;
	}
}

void deleteOneChild(Node* n) {
	//n must have at most one non-leaf child (meaning it must have a non0null node)
	Node* child = (n->right == NULL) ? n->left : n->right; //if the right child is null, then the child is the left child of node n, otheriwse it remains the right child
	if (child) {

		replaceNode(n, child);
		if (n->isRed == false) {
			if (child->isRed == true) {
				child->isRed = false;
			}
			else {
				deleteCase1(child);
			}
		}
		delete(n);
	}
}

//N and it's origional parent are black. n is new root, removed one black node from every path
void deleteCase1(Node* n) {
	if (n->parent != NULL) {
		deleteCase2(n);
	}
}

//reverse the colors of the parent and sibling and rotate left/right at parent turning sibling into nodes grandparent
void deleteCase2(Node* n) {
	Node* sibling = getSibling(n);

	if (sibling->isRed == true) {
		n->parent->isRed = true;
		sibling->isRed = false;
		if (n == n->parent->left) {
			rotateLeft(n->parent);
		}
		else {
			rotateRight(n->parent);
		}
	}
	deleteCase3(n);
}	

//the parent sibling and siblings children are all black. Thus, need to repaing sibling to red to be maintain properties. However must consider other properties/pathways not through parent
void deleteCase3(Node* n) {
	Node* sibling = getSibling(n);

	if ((n->parent->isRed == false) && (sibling->isRed == false) && (sibling->left->isRed == false) && (sibling->right->isRed == false)) {
		sibling->isRed = true;
		deleteCase1(n->parent);
	} 
	else {
		deleteCase4(n);
	}
}

//the sibling and the sibling's children are black but the parent is read. Exchange sibling and parent colors. However must take into account extra black node down path through N
void deleteCase4(Node* n) {
	Node* sibling = getSibling(n);

	if ((n->parent->isRed == true) && (sibling->isRed == false) && (sibling->left->isRed == false) && (sibling->right->isRed == false)) {
		sibling->isRed = true;
		n->parent->isRed = false;
	}
	else {
		deleteCase5(n);
	}
}


//sibling is black, and siblings left child is red, while right child is black, n is left child of its parent. Rotate right/left at s so that siblings child becomes siblings parent and n's new sibling.
//exchange colors of s and new parent.
void deleteCase5(Node* n) {
	Node* sibling = getSibling(n);

	if (sibling->isRed == false) {
		//forces the red to be on left of left or right of right of parents so rotates properlu
		if ((n == n->parent->left) && (sibling->right->isRed == false) && (sibling->left->isRed = true)) {
			sibling->isRed = true;
			sibling->left->isRed = false;
			rotateRight(sibling);
		}
		else if ((n == n->parent->right) && (sibling->left->isRed == false) && (sibling->right->isRed == true)) {
			sibling->isRed = true;
			sibling->right->isRed = false;
			rotateLeft(sibling);
		}
	}
	deleteCase6(n);
}

// Sibling (s) is black, S's right child is red, and N is the left child of its parent P. Rotate left at P, so that S becomes the parent of P and S's right child. Exchange the colors of P and S, and make S's right child black. 
void deleteCase6(Node* n) {
	Node* sibling = getSibling(n);

	sibling->isRed = n->parent->isRed;
	n->parent->isRed = false;

	if (n == n->parent->left) {
		sibling->right->isRed = false;
		rotateLeft(n->parent);
	} 
	else {
		sibling->left->isRed = false;
		rotateRight(n->parent);
	}
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
























