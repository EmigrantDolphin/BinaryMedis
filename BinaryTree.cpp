#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

#define null 0

struct Node{
	Node *Parent = null;
	Node *LeftChild = null;
	Node *RightChild = null;
	int Value = 0;
	int Level = 0;
	int PosX = -1;
	int PosY = -1;
	Node(int value){
		this->Value = value;
	}
};

void AddValue(Node *&node, int value);
void RemoveValue(Node *&node, int value);  //Without & pointer is passed by value. I need reference because I set it to null if root is deleted
Node *FindValue(Node *node, int value);
int GetHeight(Node *node);
void PrintTree(Node *node);
void SetPositions(Node *node, vector<int> distFromParent, vector<vector<string>> &drawnTree);
void CalculateLevels(Node *node);

void LRN(Node *node, vector<int> &value);
Node *GetRightMostNode(Node *node);
Node *GetLeftMostNode(Node *node);

int main(){

	bool closing = false;
	Node *root = null;
	
	while(!closing){
		
		int choice = -1;
		
		printf("0. Pateikti skaiciu aibe\n");
		printf("1. Prideti elementa\n");
		printf("2. Pasalinti elementa\n");
		printf("3. Surasti elementa\n");
		printf("4. Nustatyti auksti\n");
		printf("5. Nupiesti medi\n");
		printf("6. Baigti darba\n");
		
		cin >> choice;
		system("cls");
		
		switch (choice){
			case 0:
				{
					choice = -1;
					string input;
					string num = "";			
					printf("1.Ivesti skaiciu aibe klaviatura \n");
					printf("2.Nuskaityti skaiciu aibe is failo 'duom.txt' \n");
					cin >> choice;
					system("cls");
					if (choice == 1) {
						printf("Iveskite skaiciu aibe atskirta tarpais\n");
						cin.ignore();
						getline(cin, input);
					}else {
						fstream fs("duom.txt", ios::in);
						fs >> noskipws;
						char c;
						while (fs >> c) 
							input.push_back(c);
						fs.close();
					}
					for (int i = 0; i < input.size(); i++) {
						if (input[i] != ' ')
							num += input[i];
						else if (num.size() != 0) {
							AddValue(root, atoi(num.c_str()));
							num = "";
						}
					}
					if (num.size() != 0)
						AddValue(root, atoi(num.c_str()));
					system("cls");
					PrintTree(root);
				}
				break;
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
					PrintTree(root);
				}
				break;
			case 2:
				{
					printf("Iveskite elemento reiksme istrinimui\n");
					int value;
					cin >> value;
					system("cls");
					RemoveValue(root, value);
					PrintTree(root);
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
				PrintTree(root);
				cout << "Aukstis: " << GetHeight(root) << endl << endl;
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

void AddValue(Node *&node, int value) {
	if (node == null) {
		*&node = new Node(value);
		return;
	}
	if (node->Value == value) {
		printf("Verte %i jau egzistuoja\n\n", value);
		return;
	}
	if (value < node->Value) {
		if (node->LeftChild == null) {
			node->LeftChild = new Node(value);
			node->LeftChild->Parent = node;
		}
		else
			AddValue(node->LeftChild, value);
	}
	if (value > node->Value) {
		if (node->RightChild == null) {
			node->RightChild = new Node(value);
			node->RightChild->Parent = node;
		}
		else
			AddValue(node->RightChild, value);
	}
}

void RemoveValue(Node *&node, int value){
	if (node == null)
		return;
	if (value > node->Value){
		if (node->RightChild != null)
			RemoveValue(node->RightChild, value);
		return;
	}
	if (value < node->Value){
		if (node->LeftChild != null)
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
				node->LeftChild->Parent = node->Parent;
				
				if (node->Parent->LeftChild != null && node->Value == node->Parent->LeftChild->Value)
					node->Parent->LeftChild = node->LeftChild;			
				else if (node->Parent->RightChild != null && node->Value == node->Parent->RightChild->Value)
					node->Parent->RightChild = node->LeftChild;

			}else{
				node = node->LeftChild;
				node->Parent = null;
			}		
			return;		
		}
		
		if (node->RightChild != null && node->LeftChild == null){ // turi desni pomedi
			if (node->Parent != null){
				node->RightChild->Parent = node->Parent;
				
				if (node->Parent->LeftChild != null && node->Value == node->Parent->LeftChild->Value)
					node->Parent->LeftChild = node->RightChild;							
				else if (node->Parent->RightChild != null && node->Value == node->Parent->RightChild->Value)
					node->Parent->RightChild = node->RightChild;

			}else{
				node = node->RightChild;
				node->Parent = null;
			}
			return;
		}
		
		if (node->LeftChild != null && node->RightChild != null){                                        // turi abu pomedzius
			if (node->Parent != null){ // virsune nera saknis
				if (node->Parent->RightChild != null && node->Parent->RightChild->Value == node->Value){ // virsune desiniajame pomedyje
					Node *lowestValueNode = GetLeftMostNode(node->RightChild);                           // gaunama desniojo pomedzio maziausia virsune
					node->Value = lowestValueNode->Value;
					if (node->RightChild->Value == lowestValueNode->Value)
						node->RightChild = null;
					else
						lowestValueNode->Parent->LeftChild = null;
					
					delete lowestValueNode;

				}else if (node->Parent->LeftChild != null && node->Parent->LeftChild->Value == node->Value){ // virsune kairiajame pomedyje
					Node *highestValueNode = GetRightMostNode(node->LeftChild);                              // gaunama kairiojo pomedzio didziausioji virsune
					node->Value = highestValueNode->Value;
					
					if (node->LeftChild->Value == highestValueNode->Value)
						node->LeftChild = null;
					else
						highestValueNode->Parent->RightChild = null;
	
					delete highestValueNode;
				}				
			}else{ // jei saknis turi du vaikus
				node->Value = node->RightChild->Value;
				delete node->RightChild;
				node->RightChild = null;
			}
		}
		
	}
}

Node *FindValue(Node *node, int value){
	if (node == null)
		return null;
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
	if (node == null)
		return 0;
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

void PrintTree(Node *root){
	if (root == null)
		return;
	int maxNumLength = 3;
	int treeHeight = GetHeight(root);
	int treeWidth = ((pow(2, treeHeight-1) * 2 )-1 ) * maxNumLength; // poziciju kiekis x asyje
	root->PosX = (pow(2, treeHeight-1) -1 ) * maxNumLength;
	root->PosY = 0;
	vector<int> distanceFromParent;
	distanceFromParent.push_back(root->PosX);
	for (int i = 1; i < treeHeight; i++) 
		distanceFromParent.push_back(distanceFromParent[i-1] / 2); // atstumas tarp virsuniu ant skirtingu lygiu 
	for (int i = 1; i < treeHeight; i++)
		distanceFromParent[i] += 1;                                // atstumas nuo tevo 
	
	vector<vector<string>> drawnTree(treeHeight, vector<string> (treeWidth)); //drawnTree[y][x] , kur y dideja zemyn
	for (int i = 0; i < treeHeight; i++)
		for (int j = 0; j < treeWidth; j++)
			drawnTree[i][j] = ".";
	drawnTree[root->PosY][root->PosX] = to_string(root->Value);
	
	
	root->Level = 0;
	CalculateLevels(root);
	if (root->LeftChild != null)
		SetPositions(root->LeftChild, distanceFromParent, drawnTree);
	if (root->RightChild != null)
		SetPositions(root->RightChild, distanceFromParent, drawnTree);
	
	for (int i = 0; i < treeHeight; i++){
		for (int j = 0; j < treeWidth; j++)
			cout << drawnTree[i][j];
		cout << endl;
	}
}

void SetPositions(Node *node, vector<int> distFromParent, vector<vector<string>> &drawnTree){
	if (node->Parent->LeftChild != null && node->Parent->LeftChild->Value == node->Value) // kairysis
		node->PosX = node->Parent->PosX - distFromParent[node->Level];
	else if (node->Parent->RightChild != null && node->Parent->RightChild->Value == node->Value) //desinysis
		node->PosX = node->Parent->PosX + distFromParent[node->Level];
		
	node->PosY = node->Level;
	
	drawnTree[node->PosY][node->PosX] = to_string(node->Value);
	
	if (node->LeftChild != null)
		SetPositions(node->LeftChild, distFromParent, drawnTree);
	if (node->RightChild != null)
		SetPositions(node->RightChild, distFromParent, drawnTree);
	
}

void CalculateLevels(Node* node){
	if (node->Parent != null)
		node->Level = node->Parent->Level + 1;
	if (node->RightChild != null)
		CalculateLevels(node->RightChild);
	if (node->LeftChild != null)
		CalculateLevels(node->LeftChild);
}

void LRN(Node *node, vector<int> &values){ // KDV masyvas
	if (node->LeftChild != null)
		LRN(node->LeftChild, values); 
	if (node->RightChild != null)
		LRN(node->RightChild, values);
	
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
	if (node->LeftChild != null)
		leftMost = GetRightMostNode(node->LeftChild);
	else
		return node;
	
	return leftMost;
}











