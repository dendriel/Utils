#include "parse_args.h"
#include <iostream>
#include <sstream>

using namespace std;

// Static functions.
static int add_option(vector<st_param>& options_list, const char& opt, const char *param);


// Public functions.
vector<st_param> parse_args_create_list(const string& options)
{
	vector<st_param> options_list;

	for (size_t i = 0; i < options.size(); ++i) {
		/* Avoid special character. */
		if (options[i] == ':') {
			continue;
		}

		st_param item = {options[i], ""};
		options_list.push_back(item);
	}

	return options_list;
}

int parse_args(vector<st_param>& options_list, const int& argc, char *argv[], const char *flags)
{
	char opt;
	unsigned int i = 1;

	// Parse options into a vector.
	opt = getopt(argc, argv, flags);
	while (opt != '?' && static_cast<int>(opt) != -1) {
		if (add_option(options_list, opt, argv[i*2]) != 0) {
			cout << "Invalid option \"" << opt << " " << argv[i*2] << "\"" << endl;
			return -1;
		}
		opt = getopt(argc, argv, flags);
		i++;
	}

	// Found invalid option?
	if (opt == '?') {
			cout << "Invalid option \"" << argv[(i*2-1)] << " " << argv[(i*2)] << "\"." << endl;
			return -1;
	}

	return 0;
}

void parse_args_print_list(std::ostream& os, vector<st_param>& options_list)
{
	for (vector<st_param>::iterator iter = options_list.begin(); iter != options_list.end(); ++iter) {
		os << "-" << iter->option << " \"" << iter->value << "\""<< endl;
	}
}

bool parse_args_check_flags(vector<st_param>& options_list, const string& flags)
{
	// For each required flag. Assume that all required flags are on the options list.
	for (size_t i = 0; i < flags.size(); ++i) {
		if (parse_args_check_option(options_list, flags[i]) != true) {
			return false;
		}
	}

	return true;
}

bool parse_args_check_option(const vector<st_param>& options_list, const char& opt)
{
	for (size_t i = 0; i < options_list.size(); ++i) {
		if (options_list[i].option == opt) {
			return (options_list[i].value.size() > 0);
		}
	}
	return false;
}

bool parse_args_get_value(const vector<st_param>& options_list, const char& opt, int &value)
{
	string str_value = parse_args_get_value(options_list, opt);

	if (str_value == "") {
		return false;
	}

	std::stringstream conversor(str_value);
	conversor >> value;

	return true;
}

string parse_args_get_value(const vector<st_param>& options_list, const char& opt)
{
	for (size_t i = 0; i < options_list.size(); ++i) {
		if (options_list[i].option == opt) {
			return options_list[i].value;
		}
	}
	return "";
}

/**
 * \brief fill the list position pointed by opt, with the corresponding parameter.
 */
static int add_option(vector<st_param>& options_list, const char& opt, const char *param)
{
	for (vector<st_param>::iterator iter = options_list.begin(); iter != options_list.end(); ++iter) {
		if (iter->option == opt) {
			iter->value = static_cast<string>(param);
			return 0;
		}
	}
	return -1;
}

