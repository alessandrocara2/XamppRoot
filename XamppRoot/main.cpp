#include <fstream>
#include <string>
#include <iostream>
#include <regex>
#include <windows.h>
#include "getopt.h"
#include <filesystem>
namespace fs = std::filesystem;

#define MAX_PATH 250
#define HELP_MSG "\
Easy-to-use tool for quickly modifying xampp root directory\n\
For more information visit https://github.com/alessandrocara2/XamppRoot \n\n\
>>USAGE: xampproot [options]\n\
\n\
>>OPTIONS:\n\
  -h --help\t\tView this help page\n\
  -l --local\t\tSet current directory as root directory\n\
  -p --path\t\Path to set as root directory (either absolute or relative)\n\
  -x --xamppfolder\t\tPath to Xampp main directory (if different from C:\\xampp)\
\n\
>>RULES:\n\
1 - Make sure Xampp Apache server is stopped\n\
2 - Run xampproot from command line prompt\n\
3 - Start Xampp Apache server\n\
4 - Enjoy :-)\n\
\n\
>>EXAMPLES:\n\
	xampproot -l\n\
Result: Xampp localhost points to current directory\n\n\
	xampproot -p C:\\Users\\localuser\\Desktop\n\
Result: Xampp localhost points to localuser's Desktop folder\n\n\
	xampproot -r ..\\rootdir\n\
Result: Xampp localhost points to rootdir in father's directory\n\
\n\
"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		//Variable for storing xampp root path
		fs::path selectedPath;

		//Variable for xampp root folder
		fs::path xamppFolder = "C:\\xampp";

		//curPath stores the current executing path
		fs::path curPath = fs::current_path();

		//Variable for the command line options management
		char opt = 0;

		/* getopt_long stores the option index here. */
		static struct option long_options[] =
		{
			{ "help",			no_argument,		0,	'h' },
			{ "path",			required_argument,	0,	'p' },
			{ "local",			no_argument,		0,	'l' },
			{ "xamppfolder",	required_argument,	0,	'x' },
			{ 0, 0, 0, 0 }
		};

		while (opt != -1)
		{
			int option_index = 0;

			opt = getopt_long(argc, argv, "hlp:x:", long_options, &option_index);

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
				selectedPath = curPath;
				break;
			}
			case 'p':
			{
				//Removal of last "\"
				string path = optarg;
				if (path[path.length() - 1] == '\\' || path[path.length() - 1] == '\"')
					path[path.length() - 1] = '\0';
				fs::path relpath = path;
				selectedPath = fs::absolute(relpath);
				break;
			}
			case 'x':
			{
				string path = optarg;
				if (path[path.length() - 1] == '\\' || path[path.length() - 1] == '\"')
					path[path.length() - 1] = '\0';
				fs::path folderpath = path;
				xamppFolder = fs::absolute(folderpath);
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

		//Check if path variable represents a valid path
		if (!fs::is_directory(selectedPath)) {
			cout << "\nError, " << selectedPath << " is not a valid path, exiting\n\n";
			return 0;
		}

		cout << "Selected path: " << selectedPath << "\n\n";

		//Read of the whole content of httpd configuration file
		ifstream httpd_in(xamppFolder.string() + "\\apache\\conf\\httpd.conf");
		string httpd_content;
		httpd_content.assign(istreambuf_iterator<char>(httpd_in), istreambuf_iterator<char>());
		httpd_in.close();

		//Match of the path to be substituted and substitution
		regex httpd_regex("(DocumentRoot \")(.*)(\"\n<Directory \")(.*)(\">)");
		string httpd_replaced = regex_replace(httpd_content, httpd_regex, "$1" + selectedPath.string() + "$3" + selectedPath.string() + "$5");

		//Modification of httpd file
		ofstream httpd_out(xamppFolder.string() + "\\apache\\conf\\httpd.conf");
		httpd_out << httpd_replaced;
		httpd_out.close();

		//Configuration for working SSL
		//Read of the whole content of httpd-ssl configuration file
		ifstream httpdssl_in(xamppFolder.string() + "\\apache\\conf\\extra\\httpd-ssl.conf");
		string httpdssl_content;
		httpdssl_content.assign(istreambuf_iterator<char>(httpdssl_in), istreambuf_iterator<char>());
		httpdssl_in.close();

		//Match of the path to be substituted and substitution
		regex httpdssl_regex("(DocumentRoot \")(.*)(\")");
		string httpdssl_replaced = regex_replace(httpdssl_content, httpdssl_regex, "$1" + selectedPath.string() + "$3");

		//Modification of httpd-ssl file
		ofstream httpdssl_out(xamppFolder.string() + "\\apache\\conf\\extra\\httpd-ssl.conf");
		httpdssl_out << httpdssl_replaced;
		httpdssl_out.close();
		
	}
	else
		cout << "Missing arguments\n";

	return 0;
}