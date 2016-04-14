#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 100
#define EXTRA 10
#define MAXINPUT MAXLEN+EXTRA
#define USERFILE "../data/users.txt"
#define VARLEN 8
#include <string.h>

char file_username[MAXLEN];
char file_password[MAXLEN];
char file_fullname[MAXLEN];
char file_jobthing[MAXLEN];

void unencode(char *src, char *last, char *dest)
{
 for(; src != last; src++, dest++)
    if(*src == '+') *dest = ' ';
    else if(*src == '%') {
	int code;
	if(sscanf(src+1, "%2x", &code) != 1) code = '?';
		*dest = code;
		src +=2;}
	     
	else *dest = *src;
	*dest = '\n';
	*++dest = '\0';
 }

/* 	scan users.txt to search for username and password	*/
int validate_credentials(char*username, char*password){
	FILE *fpointer;
	fpointer = fopen(USERFILE, "r");
	if(fpointer == NULL){
		printf("<p>ERROR: users.txt failed to open</p>");
	}

	int counter = 0;
	do{
		fgets(file_username, MAXLEN, fpointer);
		printf("<p>%s %s</p>", username, file_username);
		fgets(file_password, MAXLEN, fpointer);
		printf("<p>%s %s</p>", password, file_password);
		printf("<p>Loops: %d</p>", counter);	

		if(strcmp(username, file_username) == 0 && 
		   strcmp(password, file_password) == 0){
			fclose(fpointer);
			return 1;
		}
		fgets(file_fullname, MAXLEN, fpointer);
		fgets(file_jobthing, MAXLEN, fpointer);
	
/*			fgets(file_username, MAXLEN, fpointer);
			fgets(file_password, MAXLEN, fpointer);
			fgets(file_fullname, MAXLEN, fpointer);
			fgets(file_jobthing, MAXLEN, fpointer);
*/		counter++;	
	}while(feof(fpointer) == 0);
	
	fclose(fpointer);
	return 0;

}


/*	unencode the username and password
	check if username and password exist in users.txt
	if success: load dashboard
	else return an error page (link back to home and login)
*/
int main(){
	
	int n = atoi(getenv("CONTENT_LENGTH"));
	char input[MAXINPUT], data[MAXINPUT];
	char username[MAXLEN], password[MAXLEN];
	fgets(input, n+1, stdin);
	sscanf(input, "username=%[^&]&password=%s", username, password);
	char *password_var = strstr(username, "&password=");
	unencode(password, password + strlen(password), password);
	unencode(username, username + strlen(username), username);
/*	Here we have the username and password variables	*/
	
	printf("Content-type: text/html\n\n");
	printf("<html>\n<body>");
/*	Check the users.txt for the username and password	*/	
	 
	if(validate_credentials(username, password) == 0){
		printf("<title>ERROR</title>");
		printf("<p>Username or password is wrong/doesn't exist</p>");
	}
	else{
		printf("<title>Response</title>");
		printf("<p>Login Successful</p>");
		printf("</body>\n</html>");
	}
}
