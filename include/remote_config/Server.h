//
// Created by slovak on 1/3/21.
//

#ifndef REMOTE_CONFIG_INCLUDE_SERVER_H_
#define REMOTE_CONFIG_INCLUDE_SERVER_H_

#include <iostream>
#include <array>
#include <thread>
#include <vector>
#include <fstream>
#include <memory>
#include <type_traits>

//#define USE_ZMQ

#ifdef USE_ZMQ
#include <azmq/socket.hpp>
#endif // USE_ZMQ

#include <boost/asio.hpp>

#include <Eigen/Core>
#include <Eigen/Dense>

#include <nlohmann/json.hpp>
#include "yaml-cpp/yaml.h"

namespace asio = boost::asio;

namespace remote_config {

static constexpr const char *json_type_names[] = {
    "null",             ///< null value
    "object",           ///< object (unordered set of name/value pairs)
    "array",            ///< array (ordered collection of values)
    "string",           ///< string value
    "boolean",          ///< boolean value
    "number_integer",   ///< number value (signed integer)
    "number_unsigned",  ///< number value (unsigned integer)
    "number_float",     ///< number value (floating-point)
    "binary",           ///< binary array (ordered collection of bytes)
    "discarded"         ///< discarded by the parser callback function
};

template<nlohmann::json::value_t T>
struct json_enum_to_type { using type = nullptr_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::object> { using type = nlohmann::json::object_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::array> { using type = nlohmann::json::array_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::string> { using type = nlohmann::json::string_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::boolean> { using type = nlohmann::json::boolean_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::number_integer> { using type = nlohmann::json::number_integer_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::number_unsigned> { using type = nlohmann::json::number_unsigned_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::number_float> { using type = nlohmann::json::number_float_t; };
template<>
struct json_enum_to_type<nlohmann::json::value_t::binary> { using type = nlohmann::json::binary_t; };

template<nlohmann::json::value_t T>
using json_enum_to_type_t = typename json_enum_to_type<T>::type;

template<typename T> constexpr nlohmann::json::value_t json_type_to_enum_v = nlohmann::json::value_t::null;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::object_t> = nlohmann::json::value_t::object;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::array_t> = nlohmann::json::value_t::array;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::string_t> = nlohmann::json::value_t::string;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::boolean_t> = nlohmann::json::value_t::boolean;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::number_integer_t> = nlohmann::json::value_t::number_integer;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::number_unsigned_t> = nlohmann::json::value_t::number_unsigned;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::number_float_t> = nlohmann::json::value_t::number_float;
template<> constexpr nlohmann::json::value_t json_type_to_enum_v<nlohmann::json::binary_t> = nlohmann::json::value_t::binary;


template<typename T, nlohmann::json::value_t V>
bool json_is_same_type_enum_v = std::is_same_v<T, json_enum_to_type_t<V>>;


template<typename T, int R, int C>
using MapType = Eigen::Map<
    Eigen::Matrix<T, R, C, Eigen::RowMajor>,
    Eigen::Unaligned,
    Eigen::InnerStride<sizeof(nlohmann::json) / sizeof(T)>
>;


template<typename T, int R, int C>
using ConstMapType = Eigen::Map<
    const Eigen::Matrix<T, R, C, Eigen::RowMajor>,
    Eigen::Unaligned,
    Eigen::InnerStride<sizeof(nlohmann::json) / sizeof(T)>
>;


inline nlohmann::json yaml_to_json(const YAML::Node &node) {
  nlohmann::json json;

  if (node.IsScalar()) {
    try {
      return nlohmann::json::parse(node.as<std::string>());
    } catch (const nlohmann::detail::parse_error &e) {}

    try {
      return nlohmann::json::parse("[\"" + node.as<std::string>() + "\"]")[0];
    } catch (const nlohmann::detail::parse_error &e) {}

    return node.as<std::string>();

  } else if (node.IsSequence()) {
    for (const auto& v : node) {
      json.push_back(yaml_to_json(v));
    }
  } else if (node.IsMap()) {
    for (const auto& v : node) {
      json[v.first.as<std::string>()] = yaml_to_json(v.second);
    }
  }
  return json;
}


template<typename T>
void check_array(const nlohmann::json &json, int rows, int cols) {

  if (!(json.is_array() && (json[0].is_number() || json[0].is_boolean()))) {
    throw std::runtime_error("input json object must be all numbers or all booleans array");
  }

  if (json_type_to_enum_v<T> != json[0].type()) {
    throw std::runtime_error(
        std::string("Map to wrong type: ")
            + json_type_names[static_cast<int>(json_type_to_enum_v<T>)]
            + " != "
            + json_type_names[static_cast<int>(json[0].type())]);
  }

}

template<typename T, int rows = 1, int cols = Eigen::Dynamic, typename J>
auto MapMatrixXT(J &json) {
  check_array<T>(json, rows, cols);

  if constexpr (std::is_const_v<J>) {
    if constexpr (cols == Eigen::Dynamic) {
      return ConstMapType<T, rows, cols>(json[0].template get_ptr<const T *>(), json.size());
    } else {
      if ((rows * cols) != json.size()) {
        throw std::runtime_error("rows*cols != vector.size()");
      }
      return ConstMapType<T, rows, cols>(json[0].template get_ptr<const T *>());
    };
  } else {
    if constexpr (cols == Eigen::Dynamic) {
      return MapType<T, rows, cols>(json[0].template get_ptr<T *>(), json.size());
    } else {
      if ((rows * cols) != json.size()) {
        throw std::runtime_error("rows*cols != vector.size()");
      }
      return MapType<T, rows, cols>(json[0].template get_ptr<T *>());
    };
  }
}

inline nlohmann::json get_types(const nlohmann::json &j) {
  nlohmann::json types;
  auto jf = j.flatten();
  for (auto&[k, v] : jf.items()) {
    types[k] = static_cast<std::uint64_t>(v.type());
  }
  return types.unflatten();
}

inline nlohmann::json get_type_names(const nlohmann::json &j) {
  nlohmann::json types;
  auto jf = j.flatten();
  for (auto&[k, v] : jf.items()) {
    types[k] = json_type_names[v.get<int>()];
  }
  return types.unflatten();
}

inline nlohmann::json pprint(const nlohmann::json &j) {
  nlohmann::json types;
  auto jf = j.flatten();
  for (auto&[k, v] : jf.items()) {
    auto type = static_cast<int>(v.type());
    types[k] = v.dump() + ": " + json_type_names[type];
  }
  return types.unflatten();
}

inline void check_numerical_homogenous_arrays(const nlohmann::json &j) {

  if (j.is_array() && std::all_of(j.begin(), j.end(), [](const auto x) { return x.is_number() || x.is_boolean(); })) {
    if (std::any_of(j.begin(), j.end(), [zero_type = j[0].type()](const auto x) { return x.type() != zero_type; })) {
      throw std::runtime_error("Elements in all numbers array have different types: j = " + j.dump() + ", types: " + get_types(j).dump());
    }
  } else if (!j.is_primitive()) {
    for (const auto &[k, v] : j.items()) {
      check_numerical_homogenous_arrays(v);
    }
  }
}

template<typename I, typename F>
inline void fix_arrays(nlohmann::json &j) {

  static_assert(std::is_integral_v<I>, "I must be integral type");
  static_assert(std::is_floating_point_v<F>, "F must be floating point type");

  if (j.is_array() && std::all_of(j.begin(), j.end(), [](const auto x) { return x.is_number(); })) {

    if (std::all_of(j.begin(), j.end(), [](const auto x) { return x.is_number_integer() || x.is_number_unsigned(); })) {
      for (auto &el : j) { el = static_cast<I>(el.get<I>()); }
    } else {
      for (auto &el : j) { el = static_cast<F>(el.get<F>()); }
    }

  } else if (!j.is_primitive()) {
    for (auto &v : j) {
      fix_arrays<I, F>(v);
    }
  }
}

inline void fix_unsigned(nlohmann::json &j) {
  if (j.is_primitive() && j.is_number_unsigned()) {
    j = static_cast<nlohmann::json::number_integer_t>(j.get<nlohmann::json::number_unsigned_t>());
  } else if (!j.is_primitive()) {
    for (auto &v : j) {
      fix_unsigned(v);
    }
  }
}

inline nlohmann::json apply_types(const nlohmann::json &j, const nlohmann::json &types) {
  // try cas to old types
  auto new_values_flat = j.flatten();
  auto old_types_flat = types.flatten();

  for (auto[k, v] : old_types_flat.items()) {
    switch (static_cast<nlohmann::json::value_t>(v.get<std::uint8_t>())) {
      case nlohmann::json::value_t::number_unsigned: {
        new_values_flat[k] = static_cast<nlohmann::json::number_unsigned_t>(new_values_flat[k]);
        break;
      }
      case nlohmann::json::value_t::number_integer: {
        new_values_flat[k] = static_cast<nlohmann::json::number_integer_t>(new_values_flat[k]);
        break;
      }
      case nlohmann::json::value_t::number_float: {
        new_values_flat[k] = static_cast<nlohmann::json::number_float_t>(new_values_flat[k]);
        break;
      }
      case nlohmann::json::value_t::boolean: {
        new_values_flat[k] = static_cast<nlohmann::json::boolean_t>(new_values_flat[k]);
        break;
      }

      default:break;
    }
  }

  auto new_values = new_values_flat.unflatten();

  return new_values;
}

class Server {

 public:

  std::string m_config_path;
  bool m_verbose = true;

 private:

  std::shared_ptr<nlohmann::json> m_storage;
  std::shared_ptr<nlohmann::json> m_types;
#ifdef USE_ZMQ
  std::vector<std::uint8_t> m_buf;
  azmq::rep_socket m_responder;
#endif

 public:

  explicit Server(
      asio::io_service &ios,
      const std::string &config_path = std::string(PROJECT_SOURCE_DIR) + "/config/conf.yaml",
      const std::string &addr = "tcp://127.0.0.1:5555", bool verbose = true
          )
      :
#ifdef USE_ZMQ
      m_responder(ios),
#endif
      m_verbose(verbose) {

    m_storage = std::make_shared<nlohmann::json>();
    m_types = std::make_shared<nlohmann::json>();

    Load(config_path);

#ifdef USE_ZMQ
    m_responder.bind(addr);
    m_buf.reserve(256);
    Receive();
#endif

  }

  nlohmann::json &operator[](const std::string &key) {
    return Get(key);
  }

  const nlohmann::json &operator()(const std::string &key) {
    return GetConst(key);
  }

  template<typename T>
  T get(const std::string &key) {
    return GetConst(key).get<T>();
  }

  void Load(const std::string &config_path = "") {

    if (!config_path.empty()) {
      this->m_config_path = config_path;
    }

    nlohmann::json tmp;

    if(this->m_config_path.substr(this->m_config_path.find_last_of('.') + 1) == "json") {
      std::ifstream i(this->m_config_path);
      tmp = nlohmann::json::parse(i);
    } else if(this->m_config_path.substr(this->m_config_path.find_last_of('.') + 1) == "yaml") {
      tmp = yaml_to_json(YAML::LoadFile(this->m_config_path));
    } else if(this->m_config_path.substr(this->m_config_path.find_last_of('.') + 1) == "yml") {
      tmp = yaml_to_json(YAML::LoadFile(this->m_config_path));
    } else {
      throw std::runtime_error("Wrong file format");
    }

    fix_unsigned(tmp);
    fix_arrays<std::int64_t, double>(tmp);
    check_numerical_homogenous_arrays(tmp);

    this->Set(tmp);

    if (this->m_verbose) { std::cout << pprint(Get("")).dump(1) << std::endl; }

  }

 private:

#ifdef USE_ZMQ
  void Receive() {

    this->m_buf.resize(1024);

    this->m_responder.async_receive(
        asio::buffer(this->m_buf),
        [this](auto ...vn) { this->OnReceive(vn...); });
  }

  void OnReceive(const boost::system::error_code &error, size_t bytes_transferred) {

    nlohmann::json repl;

    if (error) {
      repl["error"] = error.message();
    } else {
      this->m_buf.resize(bytes_transferred);

      try {
        nlohmann::json req = nlohmann::json::from_msgpack(this->m_buf);
        if (this->m_verbose) { std::cout << req.dump() << std::endl; }

        if (req["cmd"].get<std::string>() == "get") {
          repl = this->Get(req["key"].get<std::string>());
        } else if (req["cmd"].get<std::string>() == "set") {
          this->Set(req["key"].get<std::string>(), req["value"]);
          repl = this->Get(req["key"].get<std::string>());
        } else if (req["cmd"].get<std::string>() == "load") {
          this->Load(req["value"].get<std::string>());
          repl = this->Get("");
        }
      } catch (const std::exception &e) {
        repl["error"] = std::string(e.what());
      }
    }

    this->m_responder.async_send(
        azmq::message(asio::buffer(nlohmann::json::to_msgpack(repl))),
        [this](auto ...vn) {});

    Receive();
  }

#endif // USE_ZMQ

  nlohmann::json &Get(const std::string &key) {
    return (*this->m_storage).at(nlohmann::json::json_pointer(key));
  }

  const nlohmann::json &GetConst(const std::string &key) {
    return (*this->m_storage).at(nlohmann::json::json_pointer(key));
  }

  const nlohmann::json &GetTypes(const std::string &key) {
    return (*this->m_types).at(nlohmann::json::json_pointer(key));
  }

  void Set(const nlohmann::json &value) {
    Set("", value, true);
  }

  void Set(const std::string &key, const nlohmann::json &value, bool unsafe = false) {
    auto new_types = get_types(value);
    auto old_types = GetTypes(key);

    if (unsafe) {
      (*this->m_storage).at(nlohmann::json::json_pointer(key)) = value;
      (*this->m_types).at(nlohmann::json::json_pointer(key)) = new_types;
    } else {

      auto diff = nlohmann::json::diff(old_types, new_types);
      if (diff.empty()) {
        (*this->m_storage).at(nlohmann::json::json_pointer(key)) = value;
      } else {

        auto updated_value = apply_types(value, old_types);
        new_types = get_types(updated_value);

        diff = nlohmann::json::diff(old_types, new_types);
        if (diff.empty()) {
          (*this->m_storage).at(nlohmann::json::json_pointer(key)) = updated_value;
        } else {
          throw std::runtime_error(
              std::string("Cannot cast new types to original types")
                  + "old: " + get_type_names(old_types).dump(1)
                  + ", new: " + get_type_names(new_types).dump(1)
          );
        }
      }
    }
  }

};

}

#endif //REMOTE_CONFIG_INCLUDE_SERVER_H_
