// Testfgets.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h> /* _getcwd */
#endif

struct config_rcd {
	char userid[30];
	char password[30];
	char host[30];
	char output_file[512];
	char count_file[512];
	char model[30];
};


static struct config_rcd CONFIG_RCD;

static void getCurrentWorkingDirectory(void);
static void getConfig(struct config_rcd* CONFIG_RCD);
static char cwd[256];

int main()
{
	char output_filename[256];
	char count_filename[256];
	char model[256];

    std::cout << "Hello World!\n";
	getCurrentWorkingDirectory();
	getConfig(&CONFIG_RCD);
	if (strlen(CONFIG_RCD.output_file) > 0) {
		strcpy(output_filename, CONFIG_RCD.output_file);
	}
	if (strlen(CONFIG_RCD.count_file) > 0) {
		strcpy(count_filename, CONFIG_RCD.count_file);
	}
	sprintf(model, "%-10s", CONFIG_RCD.model);

}

static void getCurrentWorkingDirectory(void) {
#ifdef __linux__
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		perror("Unable to get current working directory\n");
		exit(4);
	}
#endif
#ifdef _WIN32
	{
		char* buffer = NULL;

		if ((buffer = _getcwd(NULL, 0)) == NULL) {
			perror("Unable to get the current working directory\n");
			exit(4);
		}
		else {
			strcpy(cwd, buffer);
			free(buffer);
		}
	}
#endif
#ifdef DEBUG
	printf("cwd: %s", cwd);
#endif

}
static void getConfig(struct config_rcd* CONFIG_RCD) {
	FILE* fp;
#define MAXCHAR 512
#define MAXSTR 256
	char config_filename[MAXCHAR];
	char name[MAXSTR];
	char value[MAXSTR];

	char line[MAXSTR];
	char msg[MAXSTR];
	int i = 0;
	printf("cwd: %s\n", cwd);
	strcpy(config_filename, cwd);
#ifdef __linux__
	strcat(config_filename, "/.config");
#endif
#ifdef _WIN32
	strcat(config_filename, "\\.config");
#endif

	strcpy(CONFIG_RCD->userid, "ops$provftp");
	strcpy(CONFIG_RCD->password, "mvWQjcTPiuEO9e");
	strcpy(CONFIG_RCD->host, "stl_prodsup01");
	strcpy(CONFIG_RCD->output_file, "");
	strcpy(CONFIG_RCD->count_file, "");
	strcpy(CONFIG_RCD->count_file, "F18slicprf_cnt.dat");
	strcpy(CONFIG_RCD->model, "F18");


	fp = fopen(config_filename, "r");
	if (fp == NULL) {
		sprintf_s(msg,sizeof(msg) - 1, "Could not open file %s using defaults", config_filename);
		perror(msg);
		return;
	}

	while (fgets(line, MAXSTR, fp) != NULL) {
		for (i = 0; i < MAXSTR; i++) {
			if (line[i] == '=') {
				strncpy_s(name,sizeof(name) - 1, line, i);
				name[i] = '\0';
				strcpy_s(value,sizeof(value) - 1, &line[i + 1]);
				value[strlen(value) - 1] = '\0';
				break;
			}
		}
		if (i > MAXSTR) {
			sprintf(msg, "bad config string %s", line);
			perror(msg);
			exit(4);
		}
		if (strcmp(name, "userid") == 0) {
			strcpy_s(CONFIG_RCD->userid,sizeof(CONFIG_RCD->userid) - 1, value);
		}
		else if (strcmp(name, "password") == 0) {
			strcpy_s(CONFIG_RCD->password,sizeof(CONFIG_RCD->password) - 1, value);
		}
		else if (strcmp(name, "host") == 0) {
			strcpy_s(CONFIG_RCD->host,sizeof(CONFIG_RCD->host) - 1, value);
		}
		else if (strcmp(name, "outfile") == 0) {
			strcpy_s(CONFIG_RCD->output_file,sizeof(CONFIG_RCD->output_file) - 1, value);
		}
		else if (strcmp(name, "countfile") == 0) {
			strcpy_s(CONFIG_RCD->count_file,sizeof(CONFIG_RCD->count_file) - 1,
				value);
		}
		else if (strcmp(name, "model") == 0) {
			strcpy(CONFIG_RCD->model, value);
		}
		else {
			sprintf_s(msg,sizeof(msg) - 1, "Unknown name/value pair: %s=%s\n", name, value);
			perror(msg);
		}
	}
	fclose(fp);
}
static void showConfig(void) {
	printf("   userid: (%s)\n", CONFIG_RCD.userid);
	printf("     host: (%s)\n", CONFIG_RCD.host);
	printf("  outfile: (%s)\n", CONFIG_RCD.output_file);
	printf("countfile: (%s)\n", CONFIG_RCD.count_file);
	printf("    model: (%s)\n", CONFIG_RCD.model);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
