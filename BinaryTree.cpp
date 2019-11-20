#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

#define null 0

struct Node{
	Node *Parent = null;
	Node *LeftChild = null;
	Node *RightChild = null;
	int Value = 0;
	int Level = 0;
	Node(int value){
		this->Value = value;
	}
};

void AddValue(Node *root, int value);
void RemoveValue(Node *root, int value);
Node *FindValue(Node *root, int value);
int GetHeight(Node *root);
void PrintTree(Node *root);

void LNR(Node *root, vector<int> &value);
Node *GetRightMostNode(Node *root);
Node *GetLeftMostNode(Node *root);

int main(){

	bool closing = false;
	Node *root = null;
	
	while(!closing){
		
		int choice = 0;
		
		printf("1. Prideti elementa\n");
		printf("2. Pasalinti elementa\n");
		printf("3. Surasti elementa\n");
		printf("4. Nustatyti auksti\n");
		printf("5. Nupiesti medi\n");
		printf("6. Baigti darba\n");
		
		cin >> choice;
		system("cls");
		
		switch (choice){
			case 1:
				{
					printf("Iveskite elemento reiksme (-99 iki 999)\n");
					int value;
					cin >> value;
					system("cls");
					if (root == null)
						root = new Node(value);
					else
						AddValue(root, value);
				}
				break;
			case 2:
				{
					printf("Iveskite elemento reiksme istrinimui");
					int value;
					cin >> value;
					system("cls");
					RemoveValue(root, value);
				}
				break;
			case 3:
				{
					printf("Iveskite elemento reiksme suradimui\n");
					int value;
					cin >> value;
					system("cls");
					Node *foundNode = FindValue(root, value);
					if (foundNode != null){
						printf("Reiksme rasta. [ko dar prirasyti?]\n\n");
					}else
						printf("Reiksme nerasta\n\n");
				}
				break;
			case 4:
				cout << GetHeight(root) << endl << endl;
				break;
			case 5:
				PrintTree(root);
				break;
			case 6:
				closing = true;
				break;
		}
		
		
	}


	return 0;
}

void AddValue(Node *root, int value){
	if (root->Value == value){
		printf("Verte %i jau egzistuoja\n\n", value);
		return;
	}
	if (value < root->Value){
		if (root->LeftChild == null){
			root->LeftChild = new Node(value);
			root->LeftChild->Parent = root;
		}
		else
			AddValue(root->LeftChild, value);
	}
	if (value > root->Value){
		if (root->RightChild == null){
			root->RightChild = new Node(value);
			root->RightChild->Parent = root;
		}
		else
			AddValue(root->RightChild, value);
	}
}
void RemoveValue(Node *root, int value){
	if (value > root->Value){
		printf("AA");
		RemoveValue(root->RightChild, value);
		return;
	}
	if (value < root->Value){
		printf("BB");
		RemoveValue(root->LeftChild, value);
		return;
	}

	if (value == root->Value){
		if (root->LeftChild == null && root->RightChild == null){ // lapas
			if (root->Parent != null){
				if (root->Parent->LeftChild != null && root->Value == root->Parent->LeftChild->Value)
					root->Parent->LeftChild = null;			
				else if (root->Parent->RightChild != null && root->Value == root->Parent->RightChild->Value)
					root->Parent->RightChild = null;
			}
			delete root;
			return;
		}
		
		if (root->LeftChild != null && root->RightChild == null){ // turi kairi pomedi
			if (root->Parent != null){
				if (root->Parent->LeftChild != null && root->Value == root->Parent->LeftChild->Value){
					root->Parent->LeftChild = root->LeftChild;			
					root->LeftChild->Parent = root->Parent;
				}
				else if (root->Parent->RightChild != null && root->Value == root->Parent->RightChild->Value){
					root->Parent->RightChild = root->LeftChild;
					root->LeftChild->Parent = root->Parent;
				}
				delete root;
			}else{
				root = root->LeftChild;
			}		
			return;		
		}
		
		if (root->LeftChild == null && root->RightChild != null){ // turi desni pomedi
			if (root->Parent != null){
				if (root->Parent->LeftChild != null && root->Value == root->Parent->LeftChild->Value){
					root->Parent->LeftChild = root->RightChild;			
					root->RightChild->Parent = root->Parent;
				}
				else if (root->Parent->RightChild != null && root->Value == root->Parent->RightChild->Value){
					root->Parent->RightChild = root->RightChild;
					root->RightChild->Parent = root->Parent;
				}
				delete root;
			}else{
				root = root->RightChild;
			}
			return;
		}
		
		if (root->LeftChild != null && root->RightChild != null){ // turi abu pomedzius
			if (root->Parent != null){
				if (root->Parent->RightChild != null && root->Parent->RightChild->Value == root->Value){ // jei desinysis pomedis
					Node *leftMostNode = GetLeftMostNode(root->RightChild);
					leftMostNode->Parent->LeftChild = null;
					leftMostNode->Parent = root->Parent;
					leftMostNode->LeftChild = root->LeftChild;
					leftMostNode->RightChild = root->RightChild;
					root->Parent->RightChild = leftMostNode;
					if (root->LeftChild != null)
						root->LeftChild->Parent = leftMostNode;
					if (root->RightChild != null)
						root->RightChild->Parent = leftMostNode;
					delete root;
				}
				
				if (root->Parent->LeftChild != null && root->Parent->LeftChild->Value == root->Value){ // jei kairysis pomedis
					Node *rightMostNode = GetRightMostNode(root->LeftChild);
					rightMostNode->Parent->RightChild = null;
					rightMostNode->Parent = root->Parent;
					rightMostNode->LeftChild = root->LeftChild;
					rightMostNode->RightChild = root->RightChild;
					root->Parent->LeftChild = rightMostNode;
					if (root->LeftChild != null)
						root->LeftChild->Parent = rightMostNode;
					if (root->RightChild != null)
						root->RightChild->Parent = rightMostNode;
					delete root;
				}
			}
		}
		
	}
}
Node *FindValue(Node *root, int value){
	Node *foundNode = null;
	if (root->LeftChild != null && value < root->Value)
		foundNode = FindValue(root->LeftChild, value);
	else if (root->RightChild != null && value > root->Value)
		foundNode = FindValue(root->RightChild, value);
	else if (value == root->Value)
		return root;
	
	return foundNode;
}
int GetHeight(Node *root){
	int leftHeight = 0;
	int rightHeight = 0;
	
	if (root->LeftChild != null)
		leftHeight = GetHeight(root->LeftChild) + 1;
	else
		leftHeight = 1;
	
	if (root->RightChild != null)
		rightHeight = GetHeight(root->RightChild) + 1;
	else
		rightHeight = 1;
	
	return leftHeight > rightHeight ? leftHeight : rightHeight;
}
void PrintTree(Node *root){
	vector<int> values;
	LNR(root, values);
	printf("KVD {");
	for (int i = 0; i < values.size()-1; i++)
		printf(" %i,",values[i]);
	printf(" %i }\n\n", values[values.size()-1]);
}

void LNR(Node *root, vector<int> &values){ // KVD masyvas
	if (root->LeftChild != null)
		LNR(root->LeftChild, values);
	if (root->RightChild != null)
		LNR(root->RightChild, values);
	
	values.push_back(root->Value);
	return;	
}

Node *GetRightMostNode(Node *root){
	Node *rightMost;
	if (root->RightChild != null)
		rightMost = GetRightMostNode(root->RightChild);
	else
		return root;
	
	return rightMost;
}

Node *GetLeftMostNode(Node *root){
	Node *leftMost;
	if (root->RightChild != null)
		leftMost = GetRightMostNode(root->LeftChild);
	else
		return root;
	
	return leftMost;
}











