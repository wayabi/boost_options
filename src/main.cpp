#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace boost;
using namespace boost::program_options;

int arg_int;
string arg_log_level;

int parse_args(int argc, char** argv)
{
  options_description options("ops");
  options.add_options()
    ("int,i", value<int>()->default_value(0), "integer")
    ("log,l", value<string>(), "log level")
  ;
  variables_map vm;
  try{
    //parse args
		if(argc == 2){
			ifstream config_file(*(argv+1));
	    store(parse_config_file(config_file, options), vm);
		}else{
	    store(parse_command_line(argc, argv, options), vm);
		}
    notify(vm);

    if(vm.count("int")){
      arg_int = vm["int"].as<int>();
      BOOST_LOG_TRIVIAL(info) << "int:" << arg_int;
    }
    if(vm.count("log")){
      arg_log_level = vm["log"].as<string>();
      BOOST_LOG_TRIVIAL(info) << "log_level:" << arg_log_level;
    }

    if(arg_log_level.empty()){
      BOOST_LOG_TRIVIAL(info) << "Need --log for log_level";
      BOOST_LOG_TRIVIAL(info) << options;
      return 1;
    }
  }
  catch(std::exception& e){
    BOOST_LOG_TRIVIAL(error) << "error in parsing args:" << e.what();
    return 1;
  }
  return 0;
}

int main(int argc, char** argv)
{
	parse_args(argc, argv);
}
