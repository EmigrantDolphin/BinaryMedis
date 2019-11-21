#include <iostream>
#include <vector>

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

void AddValue(Node *node, int value);
void RemoveValue(Node *&node, int value);  //Without & pointer is passed by value. I need reference because I set it to null if root is deleted
Node *FindValue(Node *node, int value);
int GetHeight(Node *node);
void PrintTree(Node *node);

void LNR(Node *node, vector<int> &value);
Node *GetRightMostNode(Node *node);
Node *GetLeftMostNode(Node *node);

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
					printf("Iveskite elemento reiksme istrinimui\n");
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
				if (root != null){
					PrintTree(root);
				}
				break;
			case 6:
				closing = true;
				break;
		}
		
		
	}


	return 0;
}

void AddValue(Node *node, int value){
	if (node->Value == value){
		printf("Verte %i jau egzistuoja\n\n", value);
		return;
	}
	if (value < node->Value){
		if (node->LeftChild == null){
			node->LeftChild = new Node(value);
			node->LeftChild->Parent = node;
		}
		else
			AddValue(node->LeftChild, value);
	}
	if (value > node->Value){
		if (node->RightChild == null){
			node->RightChild = new Node(value);
			node->RightChild->Parent = node;
		}
		else
			AddValue(node->RightChild, value);
	}
}
void RemoveValue(Node *&node, int value){
	if (value > node->Value){
		RemoveValue(node->RightChild, value);
		return;
	}
	if (value < node->Value){
		RemoveValue(node->LeftChild, value);
		return;
	}

	if (value == node->Value){
		if (node->LeftChild == null && node->RightChild == null){ // lapas
			if (node->Parent != null){
				if (node->Parent->LeftChild != null && node->Value == node->Parent->LeftChild->Value)
					node->Parent->LeftChild = null;			
				else if (node->Parent->RightChild != null && node->Value == node->Parent->RightChild->Value)
					node->Parent->RightChild = null;
				delete node;
			}else{
				node = null;
			}
			return;
		}
		
		if (node->LeftChild != null && node->RightChild == null){ // turi kairi pomedi
			if (node->Parent != null){
				if (node->Parent->LeftChild != null && node->Value == node->Parent->LeftChild->Value){
					node->Parent->LeftChild = node->LeftChild;			
					node->LeftChild->Parent = node->Parent;
				}
				else if (node->Parent->RightChild != null && node->Value == node->Parent->RightChild->Value){
					node->Parent->RightChild = node->LeftChild;
					node->LeftChild->Parent = node->Parent;
				}
				delete node;
			}else{
				node = node->LeftChild;
				node->Parent = null;
			}		
			return;		
		}
		
		if (node->LeftChild == null && node->RightChild != null){ // turi desni pomedi
			if (node->Parent != null){
				if (node->Parent->LeftChild != null && node->Value == node->Parent->LeftChild->Value){
					node->Parent->LeftChild = node->RightChild;			
					node->RightChild->Parent = node->Parent;
				}
				else if (node->Parent->RightChild != null && node->Value == node->Parent->RightChild->Value){
					node->Parent->RightChild = node->RightChild;
					node->RightChild->Parent = node->Parent;
				}
				delete node;
			}else{
				node = node->RightChild;
				node->Parent = null;
			}
			return;
		}
		
		if (node->LeftChild != null && node->RightChild != null){ // turi abu pomedzius
			if (node->Parent != null){
				
			}
		}
		
	}
}
Node *FindValue(Node *node, int value){
	Node *foundNode = null;
	if (node->LeftChild != null && value < node->Value)
		foundNode = FindValue(node->LeftChild, value);
	else if (node->RightChild != null && value > node->Value)
		foundNode = FindValue(node->RightChild, value);
	else if (value == node->Value)
		return node;
	
	return foundNode;
}
int GetHeight(Node *node){
	int leftHeight = 0;
	int rightHeight = 0;
	
	if (node->LeftChild != null)
		leftHeight = GetHeight(node->LeftChild) + 1;
	else
		leftHeight = 1;
	
	if (node->RightChild != null)
		rightHeight = GetHeight(node->RightChild) + 1;
	else
		rightHeight = 1;
	
	return leftHeight > rightHeight ? leftHeight : rightHeight;
}
void PrintTree(Node *node){
	vector<int> values;
	LNR(node, values);
	printf("KVD {");
	for (int i = 0; i < values.size()-1; i++)
		printf(" %i,",values[i]);
	printf(" %i }\n\n", values[values.size()-1]);
}

void LNR(Node *node, vector<int> &values){ // KVD masyvas
	if (node->LeftChild != null)
		LNR(node->LeftChild, values);
	if (node->RightChild != null)
		LNR(node->RightChild, values);
	
	values.push_back(node->Value);
	return;	
}

Node *GetRightMostNode(Node *node){
	Node *rightMost;
	if (node->RightChild != null)
		rightMost = GetRightMostNode(node->RightChild);
	else
		return node;
	
	return rightMost;
}

Node *GetLeftMostNode(Node *node){
	Node *leftMost;
	if (node->RightChild != null)
		leftMost = GetRightMostNode(node->LeftChild);
	else
		return node;
	
	return leftMost;
}











