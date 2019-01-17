#include "usernameAuthentification.h"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#define userList "authentification/userList.txt"

char loggedIn = 0;

char usernameAuthentification(const char* username){

	  char buffer[1024];      
          int desc = open(userList, O_RDONLY);
          if( desc == -1){
                  perror("Fisierul de configuratie nu a fost gasit.");
                  exit(1);
          }
          int i = 0;
          while(read(desc, &buffer[i], 1) == 1){
                  i++;
                  if(buffer[i-1] == '\n' || buffer[i-1] == 0){
                          buffer[i-1] = 0;
                          if(!strcmp(username,buffer))
                                  return 1;
                          for(unsigned j = 0; j < i; ++j)
                                  buffer[j] = 0;
                          i = 0;
                  }
          }
          close(desc);
          return 0;
}

const char* logIn(const char* username){
	
	char *returnValue = "V-ati autentificat cu succes cu numele de utilizator ";	
	char *rV = (char*)malloc(strlen(returnValue) + strlen(username) + 2);
	strcpy(rV, returnValue);
	if(usernameAuthentification(username)){
		strcpy(rV + strlen(returnValue), username);
		strcpy(rV + strlen(rV), "\n");
		loggedIn = 1;
		return rV;
	}
	else
		return "Numele de utilizator introdus nu este valabil.\n";
	


}
