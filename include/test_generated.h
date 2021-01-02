// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TEST_H_
#define FLATBUFFERS_GENERATED_TEST_H_

#include "flatbuffers/flatbuffers.h"

struct FbEigenMat;
struct FbEigenMatBuilder;
struct FbEigenMatT;

struct FbConf;
struct FbConfBuilder;
struct FbConfT;

inline const flatbuffers::TypeTable *FbEigenMatTypeTable();

inline const flatbuffers::TypeTable *FbConfTypeTable();

struct FbEigenMatT : public flatbuffers::NativeTable {
  typedef FbEigenMat TableType;
  std::string dtype{};
  uint32_t rows = 1;
  uint32_t cols = 1;
  std::vector<float> data{};
  std::string order{};
};

struct FbEigenMat FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FbEigenMatT NativeTableType;
  typedef FbEigenMatBuilder Builder;
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return FbEigenMatTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DTYPE = 4,
    VT_ROWS = 6,
    VT_COLS = 8,
    VT_DATA = 10,
    VT_ORDER = 12
  };
  const flatbuffers::String *dtype() const {
    return GetPointer<const flatbuffers::String *>(VT_DTYPE);
  }
  uint32_t rows() const {
    return GetField<uint32_t>(VT_ROWS, 1);
  }
  uint32_t cols() const {
    return GetField<uint32_t>(VT_COLS, 1);
  }
  const flatbuffers::Vector<float> *data() const {
    return GetPointer<const flatbuffers::Vector<float> *>(VT_DATA);
  }
  const flatbuffers::String *order() const {
    return GetPointer<const flatbuffers::String *>(VT_ORDER);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DTYPE) &&
           verifier.VerifyString(dtype()) &&
           VerifyField<uint32_t>(verifier, VT_ROWS) &&
           VerifyField<uint32_t>(verifier, VT_COLS) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           VerifyOffset(verifier, VT_ORDER) &&
           verifier.VerifyString(order()) &&
           verifier.EndTable();
  }
  FbEigenMatT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(FbEigenMatT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<FbEigenMat> Pack(flatbuffers::FlatBufferBuilder &_fbb, const FbEigenMatT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct FbEigenMatBuilder {
  typedef FbEigenMat Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_dtype(flatbuffers::Offset<flatbuffers::String> dtype) {
    fbb_.AddOffset(FbEigenMat::VT_DTYPE, dtype);
  }
  void add_rows(uint32_t rows) {
    fbb_.AddElement<uint32_t>(FbEigenMat::VT_ROWS, rows, 1);
  }
  void add_cols(uint32_t cols) {
    fbb_.AddElement<uint32_t>(FbEigenMat::VT_COLS, cols, 1);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<float>> data) {
    fbb_.AddOffset(FbEigenMat::VT_DATA, data);
  }
  void add_order(flatbuffers::Offset<flatbuffers::String> order) {
    fbb_.AddOffset(FbEigenMat::VT_ORDER, order);
  }
  explicit FbEigenMatBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<FbEigenMat> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FbEigenMat>(end);
    return o;
  }
};

inline flatbuffers::Offset<FbEigenMat> CreateFbEigenMat(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> dtype = 0,
    uint32_t rows = 1,
    uint32_t cols = 1,
    flatbuffers::Offset<flatbuffers::Vector<float>> data = 0,
    flatbuffers::Offset<flatbuffers::String> order = 0) {
  FbEigenMatBuilder builder_(_fbb);
  builder_.add_order(order);
  builder_.add_data(data);
  builder_.add_cols(cols);
  builder_.add_rows(rows);
  builder_.add_dtype(dtype);
  return builder_.Finish();
}

struct FbEigenMat::Traits {
  using type = FbEigenMat;
  static auto constexpr Create = CreateFbEigenMat;
};

inline flatbuffers::Offset<FbEigenMat> CreateFbEigenMatDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *dtype = nullptr,
    uint32_t rows = 1,
    uint32_t cols = 1,
    const std::vector<float> *data = nullptr,
    const char *order = nullptr) {
  auto dtype__ = dtype ? _fbb.CreateString(dtype) : 0;
  auto data__ = data ? _fbb.CreateVector<float>(*data) : 0;
  auto order__ = order ? _fbb.CreateString(order) : 0;
  return CreateFbEigenMat(
      _fbb,
      dtype__,
      rows,
      cols,
      data__,
      order__);
}

flatbuffers::Offset<FbEigenMat> CreateFbEigenMat(flatbuffers::FlatBufferBuilder &_fbb, const FbEigenMatT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct FbConfT : public flatbuffers::NativeTable {
  typedef FbConf TableType;
  std::unique_ptr<FbEigenMatT> mat{};
};

struct FbConf FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FbConfT NativeTableType;
  typedef FbConfBuilder Builder;
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return FbConfTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MAT = 4
  };
  const FbEigenMat *mat() const {
    return GetPointer<const FbEigenMat *>(VT_MAT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MAT) &&
           verifier.VerifyTable(mat()) &&
           verifier.EndTable();
  }
  FbConfT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(FbConfT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<FbConf> Pack(flatbuffers::FlatBufferBuilder &_fbb, const FbConfT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct FbConfBuilder {
  typedef FbConf Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_mat(flatbuffers::Offset<FbEigenMat> mat) {
    fbb_.AddOffset(FbConf::VT_MAT, mat);
  }
  explicit FbConfBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<FbConf> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FbConf>(end);
    return o;
  }
};

inline flatbuffers::Offset<FbConf> CreateFbConf(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<FbEigenMat> mat = 0) {
  FbConfBuilder builder_(_fbb);
  builder_.add_mat(mat);
  return builder_.Finish();
}

struct FbConf::Traits {
  using type = FbConf;
  static auto constexpr Create = CreateFbConf;
};

flatbuffers::Offset<FbConf> CreateFbConf(flatbuffers::FlatBufferBuilder &_fbb, const FbConfT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline FbEigenMatT *FbEigenMat::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::make_unique<FbEigenMatT>();
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void FbEigenMat::UnPackTo(FbEigenMatT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = dtype(); if (_e) _o->dtype = _e->str(); }
  { auto _e = rows(); _o->rows = _e; }
  { auto _e = cols(); _o->cols = _e; }
  { auto _e = data(); if (_e) { _o->data.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->data[_i] = _e->Get(_i); } } }
  { auto _e = order(); if (_e) _o->order = _e->str(); }
}

inline flatbuffers::Offset<FbEigenMat> FbEigenMat::Pack(flatbuffers::FlatBufferBuilder &_fbb, const FbEigenMatT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateFbEigenMat(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<FbEigenMat> CreateFbEigenMat(flatbuffers::FlatBufferBuilder &_fbb, const FbEigenMatT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const FbEigenMatT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _dtype = _o->dtype.empty() ? _fbb.CreateSharedString("") : _fbb.CreateString(_o->dtype);
  auto _rows = _o->rows;
  auto _cols = _o->cols;
  auto _data = _fbb.CreateVector(_o->data);
  auto _order = _o->order.empty() ? _fbb.CreateSharedString("") : _fbb.CreateString(_o->order);
  return CreateFbEigenMat(
      _fbb,
      _dtype,
      _rows,
      _cols,
      _data,
      _order);
}

inline FbConfT *FbConf::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::make_unique<FbConfT>();
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void FbConf::UnPackTo(FbConfT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = mat(); if (_e) _o->mat = std::unique_ptr<FbEigenMatT>(_e->UnPack(_resolver)); }
}

inline flatbuffers::Offset<FbConf> FbConf::Pack(flatbuffers::FlatBufferBuilder &_fbb, const FbConfT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateFbConf(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<FbConf> CreateFbConf(flatbuffers::FlatBufferBuilder &_fbb, const FbConfT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const FbConfT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _mat = _o->mat ? CreateFbEigenMat(_fbb, _o->mat.get(), _rehasher) : 0;
  return CreateFbConf(
      _fbb,
      _mat);
}

inline const flatbuffers::TypeTable *FbEigenMatTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_FLOAT, 1, -1 },
    { flatbuffers::ET_STRING, 0, -1 }
  };
  static const char * const names[] = {
    "dtype",
    "rows",
    "cols",
    "data",
    "order"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 5, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *FbConfTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_SEQUENCE, 0, 0 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    FbEigenMatTypeTable
  };
  static const char * const names[] = {
    "mat"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 1, type_codes, type_refs, nullptr, nullptr, names
  };
  return &tt;
}

inline const FbConf *GetFbConf(const void *buf) {
  return flatbuffers::GetRoot<FbConf>(buf);
}

inline const FbConf *GetSizePrefixedFbConf(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<FbConf>(buf);
}

inline bool VerifyFbConfBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<FbConf>(nullptr);
}

inline bool VerifySizePrefixedFbConfBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<FbConf>(nullptr);
}

inline void FinishFbConfBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FbConf> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedFbConfBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FbConf> root) {
  fbb.FinishSizePrefixed(root);
}

inline std::unique_ptr<FbConfT> UnPackFbConf(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<FbConfT>(GetFbConf(buf)->UnPack(res));
}

inline std::unique_ptr<FbConfT> UnPackSizePrefixedFbConf(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<FbConfT>(GetSizePrefixedFbConf(buf)->UnPack(res));
}

#endif  // FLATBUFFERS_GENERATED_TEST_H_
