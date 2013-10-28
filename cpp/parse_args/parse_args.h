/***************************************************************************************************
 * Description: Facility that parse and validate the input arguments received throung main argc,
 * argv.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 28/10/2013
 **************************************************************************************************/
#include <vector>
#include <string>

//! Represents the option as a pair of key/value.
typedef struct st_param {
	char option;
	std::string value;
} st_param;

/**
 * \brief Check if there are no missing parameters (for each option must be a value).
 * \parameter argc The arguments counter.
 * \return true if the argumentes are consistent; false if something is missing.
 */
inline bool parse_args_counter_is_valid(const int& argc)
{
	return static_cast<bool>((argc-1)%2 == 0);
}

/**
 * \brief Create a options list (like a dictionary).
 * \parameter options The element keys os the list.
 * \return A vector with st_param as pairs of key/value.
 */
std::vector<st_param> parse_args_create_list(const std::string& options);

/**
 * \brief Fill the parameters list with the parsed arguments.
 * \parameter options_list List of options to fill.
 * \parameter argc Args counter.
 * \parameter argv Arfs values.
 * \return 0 if succesful parsed the input args; -1 if found an invalid option or parameters are
 * missing.
 */
int parse_args(std::vector<st_param>& options_list, const int& argc, char *argv[], const char *flags);

/**
 * \brief Diplay an options list.
 * \parameter options_list The list to be displayed.
 */
void parse_args_print_list(std::ostream& os, std::vector<st_param>& options_list);

/**
 * \brief Checks if the option value was received.
 * \parameter options_list List of options.
 * \paramter opt The option value to be checked.
 * \return true if the value was filled; false if not.
 */
bool parse_args_check_option(const std::vector<st_param>& options_list, const char& opt);

/**
 * \brief Check if all required flags received a value.
 * \parameter options_list List to check.
 * \parameter flags Options to validate.
 * \return true if all required options have its values; false if not.
 */
bool parse_args_check_flags(std::vector<st_param>& options_list, const std::string& flags);

/**
 * \brief Get the value from the specified option.
 * \parameter options_list List to check.
 * \parameter opt The option that will have its value returned.
 * \return A string with the value of the option if exist; Or an empty string if the option was not
 * found.
 */
std::string parse_args_get_value(const std::vector<st_param>& options_list, const char& opt);

/**
 * \brief Get the value from the specified option as an integer.
 * \parameter options_list List to check.
 * \parameter opt The option that will have its value returned.
 * \return A string with the value of the option if exist; Or an empty string if the option was not
 * found.
 */
bool parse_args_get_value(const std::vector<st_param>& options_list, const char& opt, int &value);

