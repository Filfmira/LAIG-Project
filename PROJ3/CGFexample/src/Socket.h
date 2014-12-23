#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <iostream>

#define IPADDRESS "127.0.0.1"
#define PORT 60070

using namespace std;


bool socketConnect();
void envia(char *s, int len);
void recebe(char *ans);
void quit();

/*
int main() {
	socketConnect();
	char *s = "comando(1, 2).\n";
	envia(s, strlen(s));
	char ans[128];
	recebe(ans);
	quit();
	getchar();
	return 0;
}
*/


#endif