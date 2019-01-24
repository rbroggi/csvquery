#include <iostream>
#include <unordered_map>
#include <utility>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

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

int main(int ac, char *av[]) {

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "produce help message")
      ("keys,k", po::value<std::vector<std::string>>(), "aggregation keys")
      ("delimiter,d", po::value<std::string>(), "delimiter")
      ("metrics,m", po::value<std::string>(), "metrics")
      ("addition_files,a", po::value<std::string>(), "addition file mask")
      ("subtraction_files,s", po::value<std::string>(), "subtraction file mask")
      ("filters,f", po::value<std::string>(), "filters")
      ("threshold,t", po::value<double>(), "threshold")
      ("aliases,l", po::value<std::string>(), "alias for keys called in different forms on k")
  ;

  po::positional_options_description p;
  p.add("a", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
  }

  if (vm.count("keys"))
  {
      std::cout << "keys: ";
      for (auto key_group : vm["keys"].as<std::vector<std::string>>()) {
        std::vector<std::string> keys;
        boost::split(keys, key_group, [](char c){return (c == ',');});
        for (auto key : keys) {
          std::cout << key  << " ";
        }
      }
      std::cout << "\n";
  }


}
