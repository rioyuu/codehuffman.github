#include <iostream>
#include <cstdlib>
#include <stdio.h>

using namespace std;

#define MAX_TREE 100

// Khai báo node
struct NodeTree 
{
	char data;
	unsigned tanXuat;
	struct NodeTree* left, * right;
};

// Khai báo 1 nhánh ( cây nhỏ )
struct MinTree 
{
	unsigned size;
	unsigned value;
	struct NodeTree** array;
};

// Tạo node
struct NodeTree* newNode(char data, unsigned tanXuat)
{
	struct NodeTree* temp = (struct NodeTree*)malloc(sizeof(struct NodeTree));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->tanXuat = tanXuat;

	return temp;
}

// Tạo 1 nhánh
struct MinTree* createMinTree(unsigned value)

{

	struct MinTree* minTree = (struct MinTree*)malloc(sizeof(struct MinTree));

	// Số lượng node trong nhánh là 0
	minTree->size = 0;
	minTree->value = value;
	minTree->array = (struct NodeTree**)malloc(minTree->value * sizeof(struct NodeTree*));
	return minTree;
}

// Hoán vị 2 node trong 1 nhánh
void swapNodeTree(struct NodeTree** a, struct NodeTree** b)
{
	struct NodeTree* t = *a;
	*a = *b;
	*b = t;
}

// The standard minHeapify function.
void minTreeify(struct MinTree* minTree, int a)

{

	int temp = a;
	int left = 2 * a + 1;
	int right = 2 * a + 2;

	if (left < minTree->size && minTree->array[left]->tanXuat < minTree->array[temp]->tanXuat)
		temp = left;

	if (right < minTree->size && minTree->array[right]->tanXuat < minTree->array[temp]->tanXuat)
		temp = right;

	if (temp != a) 
	{
		swapNodeTree(&minTree->array[temp], &minTree->array[a]);
		minTreeify(minTree, temp);
	}
}

// Kiểm tra giá trị của nhánh có bằng 1 hay không?
int testSize(struct MinTree* minTree)
{

	return (minTree->size == 1);
}

// Lấy giá trị nhỏ nhất trong 1 nhánh
struct NodeTree* minSize(struct MinTree* minTree)
{

	struct NodeTree* temp = minTree->array[0];
	minTree->array[0] = minTree->array[minTree->size - 1];

	--minTree->size;
	minTreeify(minTree, 0);

	return temp;
}

// Thêm một node mới vào nhánh
void addNode(struct MinTree* minTree, struct NodeTree* nodeTree)

{

	++minTree->size;
	int i = minTree->size - 1;

	while (i && nodeTree->tanXuat < minTree->array[(i - 1) / 2]->tanXuat) {

		minTree->array[i] = minTree->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minTree->array[i] = nodeTree;
}

// Build một nhánh
void buildMinTree(struct MinTree* minTree)

{

	int n = minTree->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minTreeify(minTree, i);
}

// Xuất mảng 
void outputArr(int arr[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
		wcout << arr[i];

	wcout << "\n";
}

// Kiểm tra node có phải là node lá không?
int testLeaf(struct NodeTree* root) 
{
	return !(root->left) && !(root->right);
}

//	Tạo các nhánh dựa trên các node đầu vào
struct MinTree* createAndBuildMinTree(char data[], int tanXuat[], int size)
{
	struct MinTree* minTree = createMinTree(size);

	for (int i = 0; i < size; ++i)
		minTree->array[i] = newNode(data[i], tanXuat[i]);

	minTree->size = size;
	buildMinTree(minTree);

	return minTree;
}

// Xây dựng cây huffman
struct NodeTree* buildHuffmanTree(char data[], int tanXuat[], int size)

{
	struct NodeTree* left, * right, * top;

	// Bước 1: Tạo nhánh dựa trên các data nhập vào node.
	struct MinTree* minTree = createAndBuildMinTree(data, tanXuat, size);

	// Tạo vòng lặp cho tới khi size bằng 1
	while (!testSize(minTree)) {

		// Bước 2: Lấy 2 node nhỏ nhất trong các nhánh nhỏ

		left = minSize(minTree);
		right = minSize(minTree);

		// Bước 3: Tạo node mới bằng tổng 2 node nhỏ
		// Thêm 2 node đã chọn làm node bên trái và bên phải của node tổng
		// Thêm node này vào nhánh, giá trị $ là giá trị tượng trưng cho các node tổng

		top = newNode('$', left->tanXuat + right->tanXuat);
		top->left = left;
		top->right = right;
		addNode(minTree, top);
	}

	// Bước 4: Node cuối cừng là root
	return minSize(minTree);
}

// Xuất mã hoá Huffman từ các root của cây Huffman. Mã code được lưu ở trong arr[]
void outputCode(struct NodeTree* root, int arr[], int top)
{
	
	// Gán giá trị 0 khi xét về bên trái
	if (root->left) 
	{
		arr[top] = 0;
		outputCode(root->left, arr, top + 1);
	}

	// Gán giá trị 1 khi xét về bên phải
	if (root->right) 
	{
		arr[top] = 1;
		outputCode(root->right, arr, top + 1);
	}

	// Nếu là node lá thì in ký tự và mã code
	if (testLeaf(root)) 
	{
		wcout << root->data << ": ";
		outputArr(arr, top);
	}
}

// Xây dựng cây Huffman và in mã code bằng cách duyệt cây
void HuffmanCode(char data[], int tanXuat[], int size)
{
	// Tạo cây Huffman
	struct NodeTree* root = buildHuffmanTree(data, tanXuat, size);

	//	In mã code bằng cách duyệt cây 
	int arr[MAX_TREE], top = 0;
	outputCode(root, arr, top);
}

int main()
{
	char arr[] = { 'a', 'b', 'c', 'd', 'e'};
	int tanXuat[] = { 5,15,20,25,35 };
	int size = sizeof(arr) / sizeof(arr[0]);
	HuffmanCode(arr, tanXuat, size);
	system("pause");
	return 0;
}
