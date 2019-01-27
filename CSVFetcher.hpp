#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <boost/algorithm/string/predicate.hpp>


class CSVFetcher
{
    public:
      CSVFetcher(std::istream &input,
          const std::vector<std::string> &keys,
          const std::vector<std::string> &metrics,
          const std::unordered_map<std::string, std::string> &aliases,
          char delimiter);

      std::string compose_key(const CSVRow &row,
          const std::unordered_map<std::string, std::size_t> key_to_index,
          const std::vector<std::string> &keys,
          const std::unordered_map<std::string,  std::string> &aliases);

      double retrieve_metric(const CSVRow &row, const std::unordered_map<std::string, std::size_t> &key_to_index, const std::string &metric_name);

      //generates a map which returns the index position of each of the keys in the parsed row vector
      std::unordered_map<std::string, std::size_t> generate_key_to_index(const CSVRow &header,
          const std::vector<std::string> &keys,
          const std::vector<std::string> &metrics,
          const std::unordered_map<std::string,  std::string> &aliases);

      std::unordered_map<std::string,std::unordered_map<std::string,double>> retrieve_aggregation_results();

    private:
      char                                                                    _delimiter;
      std::vector<std::string>                                                _keys;
      std::vector<std::string>                                                _metrics;
      std::unordered_map<std::string,std::string>                             _aliases;
      //metric_name->map<key,metric>
      std::unordered_map<std::string,std::unordered_map<std::string,double>>  _aggregated_metrics;

};
//
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

