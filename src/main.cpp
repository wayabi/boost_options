#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace boost;
using namespace boost::program_options;

struct args_struct {
	int number_;
	string log_level_;
};

bool check_args(args_struct* args);
bool parse_args(int argc, char** argv, args_struct* args)
{
  options_description options("ops");
  options.add_options()
    ("number,n", value<int>()->default_value(0), "integer")
    ("log,l", value<string>(), "log level")
    ("help,h", "show help message")
  ;
  variables_map vm;
  try{
    //parse args
		if(argc == 2 && strcmp(*(argv+1), "-h") && strcmp(*(argv+1), "--help")){
			ifstream config_file(*(argv+1));
	    store(parse_config_file(config_file, options), vm);
		}else{
	    store(parse_command_line(argc, argv, options), vm);
		}
		if(vm.count("help") != 0){
			BOOST_LOG_TRIVIAL(info) << options;
			return false;
		}
    notify(vm);

		for (const auto& it : vm) {
			stringstream ss;
			ss << it.first.c_str() << " : ";
			auto& value = it.second.value();
			if (auto v = boost::any_cast<uint32_t>(&value))
				ss << *v;
			else if (auto v = boost::any_cast<std::string>(&value))
				ss << *v;
			else if (auto v = boost::any_cast<int>(&value))
				ss << *v;
			else
				ss << "parse error";
			BOOST_LOG_TRIVIAL(info) << ss.str();
		}

    if(vm.count("number")){
      args->number_ = vm["number"].as<int>();
    }
		if(vm.count("log")){
      args->log_level_ = vm["log"].as<string>();
    }

  }
  catch(std::exception& e){
    BOOST_LOG_TRIVIAL(error) << "error in parsing args:" << e.what();
		BOOST_LOG_TRIVIAL(info) << options;
    return false;
  }
  return check_args(args);
}

bool check_args(args_struct* args)
{
	if(args->log_level_.empty()){
		BOOST_LOG_TRIVIAL(error) << "log_level must be set";
		return false;
	}
	return true;
}


int main(int argc, char** argv)
{
	args_struct args;
	bool ret = parse_args(argc, argv, &args);
}
