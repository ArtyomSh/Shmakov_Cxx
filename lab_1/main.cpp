#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct Config {
  int count;
  string result_type;
  std::vector<std::string> types;
  std::vector<std::string> operations;
  std::vector<int> order;
  std::vector<json> values;

  int convert_to_string = -1;

  void display() {
    cout << "*************************" << endl;

    cout << "Your config description: " << endl;

    cout << "Count: " << count << std::endl;

    cout << "Result Type: " << result_type << std::endl;

    cout << "Types: ";
    for (const auto &type : types) {
      cout << type << ", ";
    }
    cout << endl;

    cout << "Operations: ";
    for (const auto &operation : operations) {
      cout << operation << " ";
    }
    cout << endl;

    cout << "Order: ";
    for (const auto &ord : order) {
      cout << ord << " ";
    }
    cout << endl;

    cout << "Values: ";
    for (const auto &val : values) {
      cout << val.dump(4) << " ";
    }
    cout << endl;

    cout << "*************************" << endl;
  }

  void parse(const string &filename) {
    ifstream file(filename);
    if (file.is_open()) {
      json j;
      file >> j;

      count = j.value("count", 0);
      result_type = j.value("result_type", "void");
      types = j.value("types", std::vector<std::string>{});
      operations = j.value("operations", std::vector<std::string>{});
      order = j.value("order", std::vector<int>{});
      values = j["values"].get<std::vector<json>>();
      file.close();
    } else {
      cerr << "Не удалось открыть файл: " << filename << endl;
    }
  }

  tuple<bool, string> validate() {
    if (types.size() != count || operations.size() != count - 1 ||
        order.size() != count || values.size() != count) {
      return make_tuple(false, "wrong input data format");
    }

    string types_checking_error = types_validation();
    if (types_checking_error != "") {
      return make_tuple(false, types_checking_error);
    }

    bool convert = should_convert_to_string();

    if (convert) {
      for (size_t i = 0; i < count - 1; i++) {
        if (operations[i] != "+") {
          return make_tuple(false, "strings has only `+` operations");
        }
      }
    }

    return make_tuple(true, "");
  }

  string types_validation() {
    for (int i = 0; i < count; i++) {
      if (types[i] == "int" || types[i] == "long int" ||
          types[i] == "long long int" || types[i] == "short int" ||
          types[i] == "char") {
        if (!values[i].is_number_integer()) {
          return "wrong value type. It should be " + types[i];
        }
      } else if (types[i] == "float" || types[i] == "double" ||
                 types[i] == "long double") {
        if (!values[i].is_number_float()) {
          return "wrong value type. It should be " + types[i];
        }
      } else if (types[i] == "string") {
        if (!values[i].is_string()) {
          return "wrong value type. It should be " + types[i];
        }
      } else if (types[i] == "bool") {
        if (values[i] != true && values[i] != false) {
          return "wrong value type. It should be " + types[i];
        }
      } else {
        cout << types[i] << " int" << endl;
        return "Unknown value type: " + types[i];
      }
    }

    return "";
  }

  bool should_convert_to_string() {
    switch (convert_to_string) {
    case 1:
      return true;
    case 0:
      return false;
    default:
      for (size_t i = 0; i < count; i++) {
        if (types[i] == "string") {
          convert_to_string = 1;
          return true;
        }
      }

      convert_to_string = 0;
      return false;
    }
  }
};

int main(int argc, char *argv[]) {
  Config config;

  if (argc == 2) {
    config.parse(argv[1]);
    config.display();

    tuple<bool, string> validation_result = config.validate();
    bool ok = get<0>(validation_result);
    string error_string = get<1>(validation_result);
    if (!ok) {
      cout << error_string << endl;
      return 1;
    }

    ofstream fout("func_1.cpp");
    fout << "#include <iostream>" << endl;
    fout << "#include <string>" << endl;
    fout << "using namespace std;" << endl;
    fout << endl;

    fout << config.result_type << " f(void) {" << endl;
    for (size_t i = 0; i < config.count; i++) {
      fout << "   " << config.types[i] << " var" << (i + 1) << " = "
           << config.values[i] << ";" << endl;
    }

    string result = "   return ";
    if (config.should_convert_to_string() &&
        config.types[config.order[0] - 1] != "string") {
      result += "to_string(var" + to_string(config.order[0]) + ")";
    } else {
      result += "var" + to_string(config.order[0]);
    }

    for (size_t i = 0; i < config.operations.size(); i++) {
      string var = "var" + to_string(config.order[i + 1]);
      if (config.should_convert_to_string() &&
          config.types[config.order[i + 1] - 1] != "string") {
        var = "to_string(" + var + ")";
      }
      result += " " + config.operations[i] + " " + var;
    }
    fout << endl;
    fout << result << ";" << endl;
    fout << "}" << endl;
    fout << endl;
  } else {
    cerr << "Для запуска используйте: " << argv[0] << " <путь к config.json>"
         << endl;
  }

  return 0;
}
