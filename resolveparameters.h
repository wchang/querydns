#ifndef QUERYDNS_H_
#define QUERYDNS_H_
#include <ares.h>
#include <arpa/nameser.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <map>
#include <string.h>
#include <iostream>
using namespace std;
bool QueryDns(string, int, const char*,const uint16_t, string*);
#endif
