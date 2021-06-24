#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>//exit()
#include <unistd.h> //close()
#include <netinet/in.h>//sockaddr_in и <sys/socket.h>
#include <arpa/inet.h> //inet_aton()
#include <cryptopp/sha.h>//sha256
#include <cryptopp/hex.h>
using namespace std;

class CheckClient
{
private:
	string address = "127.0.0.1";
	int port = 44177;
public:
	string get_sha256(string mess);
	bool check_license(string id, string pass);
};


class CCError: public invalid_argument
{
public:
	explicit CCError (const string& what_arg):
		invalid_argument(what_arg) {}
	explicit CCError (const char* what_arg):
		invalid_argument(what_arg) {}
};