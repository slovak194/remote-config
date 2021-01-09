#pragma once

#include <type_traits>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <nlohmann/json.hpp>

struct type {
  template<typename T>
  using base_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
  template<typename T>
  static constexpr const char *dtype() {
    if constexpr(std::is_same<base_type<T>, char>::value) { return "u1"; }
    if constexpr(std::is_same<base_type<T>, std::uint8_t>::value) { return "u1"; }
    else if constexpr(std::is_same<base_type<T>, std::uint16_t>::value) { return "u2"; }
    else if constexpr(std::is_same<base_type<T>, std::uint32_t>::value) { return "u4"; }
    else if constexpr(std::is_same<base_type<T>, std::uint64_t>::value) { return "u8"; }
    else if constexpr(std::is_same<base_type<T>, std::int8_t>::value) { return "i1"; }
    else if constexpr(std::is_same<base_type<T>, std::int16_t>::value) { return "i2"; }
    else if constexpr(std::is_same<base_type<T>, std::int32_t>::value) { return "i4"; }
    else if constexpr(std::is_same<base_type<T>, std::int64_t>::value) { return "i8"; }
    else if constexpr(std::is_same<base_type<T>, float>::value) { return "f4"; }
    else if constexpr(std::is_same<base_type<T>, double>::value) { return "f8"; }
    else if constexpr(std::is_same<base_type<T>, long double>::value) { return "f8"; }
    else { return "unknown type"; }
  }
};

namespace Eigen {
template<typename T>
void to_json(nlohmann::json &j, const T &matrix) {
  typename T::Index rows = matrix.rows(), cols = matrix.cols();
  auto data_begin = const_cast<typename T::Scalar *>(matrix.data());
  std::vector<typename T::Scalar> v(data_begin, data_begin + rows * cols);

  bool is_row_major = matrix.IsRowMajor;

  j = {
      {"dtype", type::dtype<typename T::Scalar>()},
      {"shape", {rows, cols}},
      {"data", v},
      {"order", is_row_major ? "C" : "F"}
  };
}
}

static constexpr const char * json_type_names[] = {
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


template <typename T>
struct json_type_to_enum {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::null;
};

template <> struct json_type_to_enum<nlohmann::json::object_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::object;
};

template <> struct json_type_to_enum<nlohmann::json::array_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::array;
};

template <> struct json_type_to_enum<nlohmann::json::string_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::string;
};

template <> struct json_type_to_enum<nlohmann::json::boolean_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::boolean;
};

template <> struct json_type_to_enum<nlohmann::json::number_integer_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::number_integer;
};

template <> struct json_type_to_enum<nlohmann::json::number_unsigned_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::number_unsigned;
};

template <> struct json_type_to_enum<nlohmann::json::number_float_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::number_float;
};

template <> struct json_type_to_enum<nlohmann::json::binary_t> {
  static constexpr nlohmann::json::value_t value = nlohmann::json::value_t::binary;
};


namespace Eigen {

template<typename T, int R, int C>
using MapType = Eigen::Map<
    Eigen::Matrix<T, R, C>,
    Eigen::Unaligned,
    Eigen::InnerStride<sizeof(nlohmann::json) / sizeof(T)>
>;

template<typename T, int R, int C>
using ConstMapType = Eigen::Map<
    const Eigen::Matrix<T, R, C>,
    Eigen::Unaligned,
    Eigen::InnerStride<sizeof(nlohmann::json) / sizeof(T)>
>;

template<typename T>
auto MapMatrixXT(nlohmann::json &json, int rows, int cols) {
  if (!(json.is_array() && (json[0].is_number() || json[0].is_boolean()))) {
    throw std::runtime_error("input json object must be all numbers array");
  }

  if (json_type_to_enum<T>::value != json[0].type()) {
    throw std::runtime_error(
        std::string("Map to wrong type: ")
            + json_type_names[static_cast<int>(json_type_to_enum<T>::value)]
            + " != "
            + json_type_names[static_cast<int>(json[0].type())]);
  }

  T *ptr = json[0].get_ptr<T *>();
  MapType<T, Eigen::Dynamic, Eigen::Dynamic> map(ptr, rows, cols);
  return map;
}

template<typename T>
auto MapMatrixXT(const nlohmann::json &json, int rows, int cols) {
  if (!(json.is_array() && (json[0].is_number() || json[0].is_boolean()))) {
    throw std::runtime_error("input json object must be all numbers array");
  }

  if (json_type_to_enum<T>::value != json[0].type()) {
    throw std::runtime_error(
        std::string("Map to wrong type: ")
        + json_type_names[static_cast<int>(json_type_to_enum<T>::value)]
        + " != "
        + json_type_names[static_cast<int>(json[0].type())]);
  }

  const T *ptr = json[0].get_ptr<const T *>();
  ConstMapType<T, Eigen::Dynamic, Eigen::Dynamic> map(ptr, rows, cols);
  return map;
}

}