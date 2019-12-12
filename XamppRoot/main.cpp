#include <fstream>
#include <string>
#include <iostream>
#include <regex>
#include <windows.h>
#include "getopt.h"

#define MAX_PATH 250
#define HELP_MSG "\
Easy-to-use tool for quickly modifying xampp root directory\n\
For more information visit https://github.com/alessandrocara2/XamppRoot \n\n\
>>USAGE: xampproot [options]\n\
\n\
>>OPTIONS:\n\
  -h --help\t\tView this help page\n\
  -l --local\t\tSet current directory as root directory\n\
  -p --path\t\tAbsolute path to set as root directory\n\
  -r --relative\t\Realtive path to set as root directory\n\
\n\
>>RULES:\n\
\n\
\n\
"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		//Variable for storing xampp root path
		string path;

		//curPath stores the current executing path 
		char buf[256];
		GetCurrentDirectoryA(256, buf);
		string curPath = string(buf);

		//Variable for the command line options management
		char opt = 0;

		/* getopt_long stores the option index here. */
		static struct option long_options[] =
		{
			{ "help",			no_argument,		0,	'h' },
			{ "path",			required_argument,	0,	'p' },
			{ "local",			no_argument,		0,	'l' },
			{ "relative",		required_argument,	0,	'r' },
			{ 0, 0, 0, 0 }
		};

		while (opt != -1)
		{
			int option_index = 0;

			opt = getopt_long(argc, argv, "hlp:r:", long_options, &option_index);

			/* Detect the end of the options. */
			if (opt == -1)
				break;

			switch (opt)
			{
			case 'h':
			{
				cout << HELP_MSG << "\n";
				return 0;
				break;
			}
			case 'l':
			{
				path = curPath;
				break;
			}
			case 'p':
			{
				path = optarg;
				break;
			}
			case 'r':
			{
				string relpath = optarg;

				//Removal of last "\"
				if (relpath[relpath.length() - 1] == '\\' || relpath[relpath.length() - 1] == '\"')
					relpath[relpath.length() - 1] = '\0';

				int rel_len = relpath.length();
				path = curPath;

				int dotnum = 0;
				for (int i = 0; i < rel_len; i++) {
					if (relpath[i] == '.')
						dotnum++;
					else
						dotnum = 0;

					if (dotnum > 2) {
						cout << "Invalid relative path";
						return 0;
					}
				}

				if (rel_len > 1) {
					if (relpath[0] != '.') {
						if (relpath[0] == '\\')
							relpath = relpath.erase(0, 1);
						path = path + '\\' + relpath;
					}
					else {
						if (relpath[1] != '.') {
							relpath = relpath.erase(0, 1);
							path = path + relpath;
						}
						else {
							dotnum = 0;
							int i, lastdot;
							for (i = 0; i < rel_len; i++) {
								if (relpath[i] == '.') {
									dotnum++;
									lastdot = i;
								}
							}


							int count = dotnum / 2;
							for (i = path.length() - 1; i >= 0 && count > 0; i--) {
								if (path[i] == '\\')
									count--;
							}

							i++;

							for (int j = lastdot + 1; j < rel_len; i++, j++) {
								path[i] = relpath[j];
							}
							path = path.erase(i, path.length() - i);
						}
					}
				}

				break;
			}
			case ':':
			{
				cout << "Missing value for option -" << (char)optopt << "\n";
				return 0;
				break;
			}
			case '?':
			default:
				return 1;
			}
		}
		if (optind < argc)
		{
			cout << "Illegal non-option arguments: ";
			while (optind < argc)
				cout << argv[optind++] << " ";
			cout << "\n\n" << HELP_MSG << "\n";
			return 0;
		}

		//Removal of last "\"
		if (path[path.length() - 1] == '\\' || path[path.length() - 1] == '\"')
			path[path.length() - 1] = '\0';

		//Check if path variable represents a valid path
		DWORD ftyp = GetFileAttributesA(path.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY)) {
			cout << "\nError, \"" << path << "\" is not a valid path, exiting\n\n";
			return 0;
		}

		cout << "Selected path: " << path << "\n\n";
		return 0;


		//Read of the whole content of httpd configuration file
		ifstream httpd_in("C:\\xampp\\apache\\conf\\httpd.conf");
		string httpd_content;
		httpd_content.assign(istreambuf_iterator<char>(httpd_in), istreambuf_iterator<char>());
		httpd_in.close();

		//Match of the path to be substituted and substitution
		regex httpd_regex("(DocumentRoot \")(.*)(\"\n<Directory \")(.*)(\">)");
		string httpd_replaced = regex_replace(httpd_content, httpd_regex, "$1" + string(path) + "$3" + string(path) + "$5");

		//Modification of httpd file
		ofstream httpd_out("C:\\xampp\\apache\\conf\\httpd.conf");
		httpd_out << httpd_replaced;
		httpd_out.close();

		//Configuration for working SSL
		//Read of the whole content of httpd-ssl configuration file
		ifstream httpdssl_in("C:\\xampp\\apache\\conf\\extra\\httpd-ssl.conf");
		string httpdssl_content;
		httpdssl_content.assign(istreambuf_iterator<char>(httpdssl_in), istreambuf_iterator<char>());
		httpdssl_in.close();

		//Match of the path to be substituted and substitution
		regex httpdssl_regex("(DocumentRoot \")(.*)(\")");
		string httpdssl_replaced = regex_replace(httpdssl_content, httpdssl_regex, "$1" + string(path) + "$3");

		//Modification of httpd-ssl file
		ofstream httpdssl_out("C:\\xampp\\apache\\conf\\extra\\httpd-ssl.conf");
		httpdssl_out << httpdssl_replaced;
		httpdssl_out.close();
		
	}
	else
		cout << "No path or flag provided\n";

	return 0;
}