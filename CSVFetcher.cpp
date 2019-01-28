
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <boost/algorithm/string/predicate.hpp>

#include "CSVRow.hpp"

class CSVFetcher
{
    public:
      CSVFetcher(std::istream &input,
          const std::vector<std::string> &keys,
          const std::vector<std::string> &metrics,
          const std::unordered_map<std::string, std::string> &aliases,
          char delimiter = '|') : _delimiter(delimiter), _keys(keys), _metrics(metrics), _aliases(aliases) {


        std::unordered_map<std::string, std::size_t> key_to_index;
        int i = 0;
        for(CSVIterator row_ptr(input); row_ptr != CSVIterator(); ++row_ptr, i++)
        {
          //manage header
          if (i == 0) {

            key_to_index = generate_key_to_index(*row_ptr, _keys, _metrics, _aliases);

          } else { //manage rows

            //todo handle key filters while fetching key from row to avoid processing unwanted data
            std::string key = compose_key(*row_ptr, key_to_index, _keys, _aliases);

            //if empty key ignore row
            if (key == "") continue;

            //generate all metrics maps
            //for each row there will be always "number of metrics" aggregations to be performed
            for (auto metric_name : _metrics) {

              //ptr to (first -> metric_name, second -> map<key, metric>)
              auto m_map_ptr = _aggregated_metrics.find(metric_name);

              //create first entry on metric map
              if (m_map_ptr == _aggregated_metrics.end()) {

                std::unordered_map<std::string, double> key_metric;
                key_metric[key] = retrieve_metric(*row_ptr, key_to_index, metric_name);
                _aggregated_metrics[metric_name] = key_metric;

              } else { //adding elements to existing metric map

                //retrieve pointer to (first -> key, second -> metric)
                auto k_map_ptr = (m_map_ptr -> second).find(key);
                //if key does not exist create it with the related metric key of the present row
                if (k_map_ptr == (m_map_ptr -> second).end()) {
                  (m_map_ptr -> second)[key] = retrieve_metric(*row_ptr, key_to_index, metric_name);
                } else { //if key exists add to the present entry
                  (k_map_ptr -> second) += retrieve_metric(*row_ptr, key_to_index, metric_name);
                }

              }

            }
          }
        }
      }

      std::string compose_key(const CSVRow &row,
          const std::unordered_map<std::string, std::size_t> key_to_index,
          const std::vector<std::string> &keys,
          const std::unordered_map<std::string,  std::string> &aliases) {
        //todo implement aliases
        std::string final_key = "";
        for (auto key : keys) {
          auto row_index = key_to_index.at(key);
          auto row_element = row[row_index];
          final_key+=_delimiter;
          final_key.append(row_element);
        }
        return final_key;

      }

      double retrieve_metric(const CSVRow &row, const std::unordered_map<std::string, std::size_t> &key_to_index, const std::string &metric_name) {
        auto row_index = key_to_index.at(metric_name);
        return std::stod(row[row_index]);
      }

      //generates a map which returns the index position of each of the keys in the parsed row vector
      std::unordered_map<std::string, std::size_t> generate_key_to_index(const CSVRow &header,
          const std::vector<std::string> &keys,
          const std::vector<std::string> &metrics,
          const std::unordered_map<std::string,  std::string> &aliases) {

            std::unordered_map<std::string, std::size_t> key_to_index;
            std::unordered_map<std::string, std::size_t> header_map = header.retrieveHeaderIndexes();

            //fill key_to_index map
            for (auto key : keys) {
                auto got = header_map.find(key);
                if ( got == header_map.end() ) {
                  std::cerr << "No column found with name: " << key << '\n';
                  abort();
                } else { //todo manage aliases
                  key_to_index[key] = got->second;
                }
            }

            //fill metric_to_index map
            for (auto metric_name : metrics) {
                auto got = header_map.find(metric_name);
                //todo better handle of "count" metric
                if ( got == header_map.end() && !boost::iequals(metric_name, "count") ) {
                  //todo manage aliases
                  std::cerr << "No column found with name: " << metric_name << '\n';
                  abort();
                } else {
                  key_to_index[metric_name] = got->second;
                }
            }

            return key_to_index;

      }

      std::unordered_map<std::string,std::unordered_map<std::string,double>> retrieve_aggregation_results() {
        return _aggregated_metrics;
      }


    private:
      char                             _delimiter;
      std::vector<std::string>         _keys;
      std::vector<std::string>         _metrics;
      std::unordered_map<std::string,  std::string>                     _aliases;
      //metric_name -> map<key,metric>
      std::unordered_map<std::string,  std::unordered_map<std::string,  double>>   _aggregated_metrics;

};
