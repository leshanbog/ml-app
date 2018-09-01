#include <map>
#include <string>


const std::string APP_NAME = "proga";

namespace core
{


const std::map<std::string, std::string> Information = {

	{ "GetHelp", "Type \"" + APP_NAME + " help\" for help\n" },

	{ "NoSuchAlg" , "There is no such algorithm!\n"},
	{ "WrongCommand" , "Wrong command!\n" },
	{ "WrongArgs", "Wrong arguments!\n"},

	{ "AlgsCommand", "These are basic algorithms:\n\t1 - Const prediction\n\t2 - DecisionStump\n\t3 - K nearest neighbours\n\nTo build bagged algorithm type 2i, where i - number of basic algorithm\nTo build boosted algorithm type 3i, where i - number of basic algorithm\n\nTo get information about algorithm parameters and default values use command 'info <i>', where i - number of algorithm\n" },
	{ "HelpCommand", "\nUsage format:\n\t" + APP_NAME + " <command> <arguments> \n\nCommands:\n\thelp <command name> - get help about command\n\talgs - get list of available algorithms\n\tinfo <number of algorithm> - get information about arguments and default values of algorithm\n\tml - get estimation of algorithm on dataset, see \"ml\" command format using info\n" },
	{ "HelpCommandalgs", "Command 'algs' don't has any arguments. It shows list of available algorithms\n "},
	{ "HelpCommandinfo", "Command 'info' has 1 argument - number of algorithm\n"},
	{ "HelpCommandml", "Command 'ml' has 2 necessary arguments - name of data file and number of algorithm\nThey are followed by optional arguments:\n\t-fn   folds number, default = 5\n\t-sep  separator of data, default = ','\n\t-a   arguments for algorithm, define all of them, separe by comma. If you don't define all arguments, default will be used!\n\t-f   feature names provided, default = 1\n\t-n   doing normalization before learning, default = 0\n"},

	{ "InfoCommand1", "Const prediction has no parameters\n"},
	{ "InfoCommand2", "DecisionStump has no parameters\n"},
	{ "InfoCommand3", "K nearest neighbours has 1 parameter K, default value is 5\n"},
	{ "InfoCommand4", "Linear regression has 2 parameters: learning rate (default = 0.00001), number of iterations (default = 300000)\n"}, 

	{ "InfoCommand2i" , "First parameter - number of models to be bagged, default value is 25\nSecond parameter - size of training set, default = 0.8 * <size of dataset>\n"},
	{ "InfoCommand3i" , "First parameter - number of models to be boosted, default value is 50\n"}


};


} // core

