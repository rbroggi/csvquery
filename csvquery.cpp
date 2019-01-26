#include <iostream>
#include <unordered_map>
#include <utility>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

static const std::string DEFAULT_DELIMITER = "|";
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

//delimiter fetcher
const std::string& fetch_delimiter(const po::variables_map &vm) {
  if (vm.count("delimiter")) {
    try {
      return vm["delimiter"].as<std::string>();
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

int main(int ac, char *av[]) {

  // Declare the supported options.
  po::options_description desc("Allowed options");

  //todo enhance with examples
  desc.add_options()
      ("help,h", "produce help message")
      ("verbose,v", "verbosity") //todo implement verbosity
      ("delimiter,d", po::value<std::string>(), "delimiter")
      ("keys,k", po::value<std::vector<std::string>>(), "aggregation keys")
      ("metrics,m", po::value<std::vector<std::string>>(), "metrics")
      ("addition_files,a", po::value<std::string>(), "addition file mask")
      ("subtraction_files,s", po::value<std::string>(), "subtraction file mask")
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
  std::string delimiter = fetch_delimiter(vm);

  std::cout << "Delimiter: " << delimiter << "\n";

  // fetch keys
  std::vector<std::string> keys = fetch_keys(vm, "keys");

  std::cout << "size: " << '\n';
  std::cout << keys.size() << '\n';
  for (auto el : keys) {
    std::cout << "el: " << el << "\n";
  }

  //fetch metrics
  std::vector<std::string> metrics = fetch_keys(vm, "metrics");

  std::cout << "size: " << '\n';
  std::cout << metrics.size() << '\n';
  for (auto el : metrics) {
    std::cout << "el: " << el << "\n";
  }

  //fetch file masks

  //fetch key filters

  //fetch metric filters

}
