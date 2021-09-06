#include <unordered_map>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <queue>
#include <iterator>
#include <utility>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <winsock2.h>
#include <Ws2tcpip.h>
// #pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

typedef struct Node {
	char c;
	int freq;
	Node* right;
	Node* left;
};

Node* getNode(char ch, int f, Node* left, Node* right) {
	Node* n = new Node();
	n->c = ch;
	n->freq = f;
	n->left = left;
	n->right = right;
	return n;
}

struct comp{
	bool operator()(Node* l, Node* r) {
		return l->freq > r->freq;
	}
};

void encode(Node* root, std::string s , std::unordered_map<char, std::string> hoffmanEncoding)
{
	if (root == nullptr)
		return;
	if (!root->left && !root->right)
		hoffmanEncoding[root->c] = s;
	encode(root->left, s + "0", hoffmanEncoding);
	encode(root->right, s + "1", hoffmanEncoding);
}

void decode(Node* root, int top_index, std::string str) {
	if (root == nullptr) {
		return;
	}
	if (!root->left && !root->right) {
		std::cout << root->c;
		return;
	}
	top_index++;
	if (str[top_index] == '0')
		decode(root->left, top_index, str);
	else
		decode(root->right, top_index, str);
}

Node* buildHuffmanTree(std::string text) {
	std::unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	std::priority_queue<Node*, std::vector<Node*>,comp> pq;
	for (auto pair : freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}
	while (pq.size() != 1) {
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top(); pq.pop();
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}
	Node* root = pq.top();
	return root;
}


int main()
{
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");
	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	// service.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, (PCSTR)"127.0.0.1", &service.sin_addr.s_addr);
	service.sin_port = htons(27015);
	if (bind(mainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(mainSocket);
		return 1;
	}
	if (listen(mainSocket, 1) == SOCKET_ERROR)
		printf("Error listening on socket.\n");
	SOCKET acceptSocket = SOCKET_ERROR;
	printf("Waiting for a client to connect...\n");

	while (acceptSocket == SOCKET_ERROR)
	{
		acceptSocket = accept(mainSocket, NULL, NULL);
	}

	printf("Client connected.\n");
	mainSocket = acceptSocket;
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[32] = "Server says hello!";
	char recvbuf[32] = "";

	bytesRecv = recv(mainSocket, recvbuf, 32, 0);
	printf("Bytes received: %ld\n", bytesRecv);
	printf("Received text: %s\n", recvbuf);

	bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
	printf("Bytes sent: %ld\n", bytesSent);

	system("pause");
	/*
	std::vector<int> my_digits(10);
	std::iota(std::begin(my_digits), std::end(my_digits), 0);
	auto print = [](auto x) { std::cout << x << " "; };
	for (auto a : my_digits) {
		print(a);
	}
	std::vector<int> odd_digits;
	auto is_odd = [](auto x) {return (x % 2) == 0; };
	std::copy_if(std::begin(my_digits), std::end(my_digits), std::back_inserter(odd_digits),
		[](auto x) {return (x % 2) != 0; });
	std::cout << std::endl << "Odd number in my_digits: ";
	for (auto a : odd_digits) {
		print(a);
	}
	std::stringstream string_odd_digits;
	for (auto tmp : odd_digits) {
		string_odd_digits << tmp;
	}
	Node* root = buildHuffmanTree(string_odd_digits.str());
	std::unordered_map<char, std::string> huffman_code;
	encode(root, "", huffman_code);
	*/
}

