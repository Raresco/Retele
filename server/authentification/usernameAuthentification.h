#ifndef _usernameAuthentification_h
#define _usernameAuthentification_h

#include <fcntl.h>
#include <string.h>
#include <errno.h>

extern char loggedIn;

char usernameAuthentification(const char* username);
const char* logIn(const char* username);

#endif
