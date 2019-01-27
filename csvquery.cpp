#include "CSVRow.hpp"

#include <iostream>
#include <unordered_map>
#include <utility>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>


static const char DEFAULT_DELIMITER = '|';
//KeyValue accumulator
// template <class K, class V>
// class KeyValueAccumulator {
//   private:
//     std::unordered_map<K, V> accumulator;
//   public:
//     KeyValueAccumulator();
//     void add_item(const std::pair<K, V> &);
//     V retrieve_element(const &K);
//     std::vector<std::pair<K, V> >& retrieve_elements();
// };
namespace po = boost::program_options;

bool verbose = false;

//delimiter fetcher
const char fetch_delimiter(const po::variables_map &vm) {
  if (vm.count("delimiter")) {
    try {
      return vm["delimiter"].as<char>();
    } catch (...) {
      return DEFAULT_DELIMITER;
    }
  }
  return DEFAULT_DELIMITER;
}

//keys fetcher
std::vector<std::string> fetch_keys(const po::variables_map &vm, const std::string &variable_name) {
  auto key_num = vm.count(variable_name);
  std::vector<std::string> keys;
  if (key_num)
  {
      //each key_group is a comma-separated list
      for (auto key_group : vm[variable_name].as<std::vector<std::string>>()) {
        std::vector<std::string> untrimmed_keys;
        //splitting keys on commas
        boost::split(untrimmed_keys, key_group, [](char c){return (c == ',' || c == ' ');});
        //trim keys and fill the resulting keys
        for (auto key : untrimmed_keys) {
          boost::trim(key);
          if (!key.empty()) keys.push_back(std::move(key));
        }
      }
  }

  return keys;
}

//print string vector
void log_vector(const std::vector<std::string> &vec, const std::string &vec_name) {
  if (!verbose) return;
  std::cout << vec_name << ": " << "[" << vec.size() << "]{";
  for (auto el : vec) {
    std::cout << el <<", ";
  }
  std::cout << "}"  << "\n";
}

//log string
template<typename T>
void log_str(const T &str, const std::string &str_name) {
  if (!verbose) return;
  std::cout << str_name << ": " << str << '\n';
}

int main(int ac, char *av[]) {

  // Declare the supported options.
  po::options_description desc("Allowed options");

  //todo enhance with examples
  desc.add_options()
      ("help,h", "produce help message")
      ("verbose,v", po::bool_switch(&verbose), "verbosity") //todo implement verbosity
      ("delimiter,d", po::value<char>(), "delimiter")
      ("keys,k", po::value<std::vector<std::string>>() -> multitoken(), "aggregation keys")
      ("metrics,m", po::value<std::vector<std::string>>() -> multitoken(), "metrics")
      ("addition_files,a", po::value<std::vector<std::string>>() -> multitoken(), "addition file mask")
      ("subtraction_files,s", po::value<std::vector<std::string>>() -> multitoken(), "subtraction file mask")
      ("key_filters,kf", po::value<std::string>(), "key filters")
      ("metric_filters,mf", po::value<double>(), "metric filters")
      ("aliases,al", po::value<std::string>(), "alias for keys called in different forms on k")
  ;


  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
  }

  //fetch delimiter
  char delimiter = fetch_delimiter(vm);

  log_str(delimiter, "delimiter");

  // fetch keys
  std::vector<std::string> keys = fetch_keys(vm, "keys");
  log_vector(keys, "keys");

  //fetch metrics
  std::vector<std::string> metrics = fetch_keys(vm, "metrics");
  log_vector(metrics, "metrics");

  //fetch file masks
  std::vector<std::string> addition_files = fetch_keys(vm, "addition_files");
  log_vector(addition_files, "addition_files");

  std::vector<std::string> subtraction_files = fetch_keys(vm, "subtraction_files");
  log_vector(subtraction_files, "subtraction_files");

  // std::ifstream       file("plop.csv");
  //
  // for(CSVIterator loop(file); loop != CSVIterator(); ++loop)
  // {
  //     std::cout << "4th Element(" << (*loop)[3] << ")\n";
  // }
  //fetch key filters
  //fetch metric filters

}
