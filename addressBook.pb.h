// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: addressBook.proto

#ifndef PROTOBUF_INCLUDED_addressBook_2eproto
#define PROTOBUF_INCLUDED_addressBook_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3006001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_addressBook_2eproto

// Internal implementation detail -- do not use these members.
struct TableStruct_addressBook_2eproto {
  static const ::google::protobuf::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors_addressBook_2eproto();
namespace tutorial {
class AddressBook;
class AddressBookDefaultTypeInternal;
extern AddressBookDefaultTypeInternal _AddressBook_default_instance_;
class Person;
class PersonDefaultTypeInternal;
extern PersonDefaultTypeInternal _Person_default_instance_;
class Person_PhoneNumber;
class Person_PhoneNumberDefaultTypeInternal;
extern Person_PhoneNumberDefaultTypeInternal _Person_PhoneNumber_default_instance_;
}  // namespace tutorial
namespace google {
namespace protobuf {
template<> ::tutorial::AddressBook* Arena::CreateMaybeMessage<::tutorial::AddressBook>(Arena*);
template<> ::tutorial::Person* Arena::CreateMaybeMessage<::tutorial::Person>(Arena*);
template<> ::tutorial::Person_PhoneNumber* Arena::CreateMaybeMessage<::tutorial::Person_PhoneNumber>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace tutorial {

enum Person_PhoneType {
  Person_PhoneType_MOBILE = 0,
  Person_PhoneType_HOME = 1,
  Person_PhoneType_WORK = 2
};
bool Person_PhoneType_IsValid(int value);
const Person_PhoneType Person_PhoneType_PhoneType_MIN = Person_PhoneType_MOBILE;
const Person_PhoneType Person_PhoneType_PhoneType_MAX = Person_PhoneType_WORK;
const int Person_PhoneType_PhoneType_ARRAYSIZE = Person_PhoneType_PhoneType_MAX + 1;

const ::google::protobuf::EnumDescriptor* Person_PhoneType_descriptor();
inline const ::std::string& Person_PhoneType_Name(Person_PhoneType value) {
  return ::google::protobuf::internal::NameOfEnum(
    Person_PhoneType_descriptor(), value);
}
inline bool Person_PhoneType_Parse(
    const ::std::string& name, Person_PhoneType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Person_PhoneType>(
    Person_PhoneType_descriptor(), name, value);
}
// ===================================================================

class Person_PhoneNumber : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tutorial.Person.PhoneNumber) */ {
 public:
  Person_PhoneNumber();
  virtual ~Person_PhoneNumber();

  Person_PhoneNumber(const Person_PhoneNumber& from);

  inline Person_PhoneNumber& operator=(const Person_PhoneNumber& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Person_PhoneNumber(Person_PhoneNumber&& from) noexcept
    : Person_PhoneNumber() {
    *this = ::std::move(from);
  }

  inline Person_PhoneNumber& operator=(Person_PhoneNumber&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const Person_PhoneNumber& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Person_PhoneNumber* internal_default_instance() {
    return reinterpret_cast<const Person_PhoneNumber*>(
               &_Person_PhoneNumber_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Person_PhoneNumber* other);
  friend void swap(Person_PhoneNumber& a, Person_PhoneNumber& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Person_PhoneNumber* New() const final {
    return CreateMaybeMessage<Person_PhoneNumber>(NULL);
  }

  Person_PhoneNumber* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Person_PhoneNumber>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Person_PhoneNumber& from);
  void MergeFrom(const Person_PhoneNumber& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  static const char* _InternalParse(const char* begin, const char* end, void* object, ::google::protobuf::internal::ParseContext* ctx);
  ::google::protobuf::internal::ParseFunc _ParseFunc() const final { return _InternalParse; }
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Person_PhoneNumber* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string number = 1;
  bool has_number() const;
  void clear_number();
  static const int kNumberFieldNumber = 1;
  const ::std::string& number() const;
  void set_number(const ::std::string& value);
  #if LANG_CXX11
  void set_number(::std::string&& value);
  #endif
  void set_number(const char* value);
  void set_number(const char* value, size_t size);
  ::std::string* mutable_number();
  ::std::string* release_number();
  void set_allocated_number(::std::string* number);

  // optional .tutorial.Person.PhoneType type = 2 [default = HOME];
  bool has_type() const;
  void clear_type();
  static const int kTypeFieldNumber = 2;
  ::tutorial::Person_PhoneType type() const;
  void set_type(::tutorial::Person_PhoneType value);

  // @@protoc_insertion_point(class_scope:tutorial.Person.PhoneNumber)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr number_;
  int type_;
  friend struct ::TableStruct_addressBook_2eproto;
};
// -------------------------------------------------------------------

class Person : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tutorial.Person) */ {
 public:
  Person();
  virtual ~Person();

  Person(const Person& from);

  inline Person& operator=(const Person& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Person(Person&& from) noexcept
    : Person() {
    *this = ::std::move(from);
  }

  inline Person& operator=(Person&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const Person& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Person* internal_default_instance() {
    return reinterpret_cast<const Person*>(
               &_Person_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Person* other);
  friend void swap(Person& a, Person& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Person* New() const final {
    return CreateMaybeMessage<Person>(NULL);
  }

  Person* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Person>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Person& from);
  void MergeFrom(const Person& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  static const char* _InternalParse(const char* begin, const char* end, void* object, ::google::protobuf::internal::ParseContext* ctx);
  ::google::protobuf::internal::ParseFunc _ParseFunc() const final { return _InternalParse; }
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Person* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef Person_PhoneNumber PhoneNumber;

  typedef Person_PhoneType PhoneType;
  static const PhoneType MOBILE =
    Person_PhoneType_MOBILE;
  static const PhoneType HOME =
    Person_PhoneType_HOME;
  static const PhoneType WORK =
    Person_PhoneType_WORK;
  static inline bool PhoneType_IsValid(int value) {
    return Person_PhoneType_IsValid(value);
  }
  static const PhoneType PhoneType_MIN =
    Person_PhoneType_PhoneType_MIN;
  static const PhoneType PhoneType_MAX =
    Person_PhoneType_PhoneType_MAX;
  static const int PhoneType_ARRAYSIZE =
    Person_PhoneType_PhoneType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  PhoneType_descriptor() {
    return Person_PhoneType_descriptor();
  }
  static inline const ::std::string& PhoneType_Name(PhoneType value) {
    return Person_PhoneType_Name(value);
  }
  static inline bool PhoneType_Parse(const ::std::string& name,
      PhoneType* value) {
    return Person_PhoneType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // repeated .tutorial.Person.PhoneNumber phone = 4;
  int phone_size() const;
  void clear_phone();
  static const int kPhoneFieldNumber = 4;
  ::tutorial::Person_PhoneNumber* mutable_phone(int index);
  ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >*
      mutable_phone();
  const ::tutorial::Person_PhoneNumber& phone(int index) const;
  ::tutorial::Person_PhoneNumber* add_phone();
  const ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >&
      phone() const;

  // required string name = 1;
  bool has_name() const;
  void clear_name();
  static const int kNameFieldNumber = 1;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  #if LANG_CXX11
  void set_name(::std::string&& value);
  #endif
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // optional string email = 3;
  bool has_email() const;
  void clear_email();
  static const int kEmailFieldNumber = 3;
  const ::std::string& email() const;
  void set_email(const ::std::string& value);
  #if LANG_CXX11
  void set_email(::std::string&& value);
  #endif
  void set_email(const char* value);
  void set_email(const char* value, size_t size);
  ::std::string* mutable_email();
  ::std::string* release_email();
  void set_allocated_email(::std::string* email);

  // required uint32 id = 2;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 2;
  ::google::protobuf::uint32 id() const;
  void set_id(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:tutorial.Person)
 private:
  class HasBitSetters;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber > phone_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::internal::ArenaStringPtr email_;
  ::google::protobuf::uint32 id_;
  friend struct ::TableStruct_addressBook_2eproto;
};
// -------------------------------------------------------------------

class AddressBook : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tutorial.AddressBook) */ {
 public:
  AddressBook();
  virtual ~AddressBook();

  AddressBook(const AddressBook& from);

  inline AddressBook& operator=(const AddressBook& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  AddressBook(AddressBook&& from) noexcept
    : AddressBook() {
    *this = ::std::move(from);
  }

  inline AddressBook& operator=(AddressBook&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const AddressBook& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AddressBook* internal_default_instance() {
    return reinterpret_cast<const AddressBook*>(
               &_AddressBook_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(AddressBook* other);
  friend void swap(AddressBook& a, AddressBook& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline AddressBook* New() const final {
    return CreateMaybeMessage<AddressBook>(NULL);
  }

  AddressBook* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<AddressBook>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const AddressBook& from);
  void MergeFrom(const AddressBook& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  static const char* _InternalParse(const char* begin, const char* end, void* object, ::google::protobuf::internal::ParseContext* ctx);
  ::google::protobuf::internal::ParseFunc _ParseFunc() const final { return _InternalParse; }
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(AddressBook* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .tutorial.Person person = 1;
  int person_size() const;
  void clear_person();
  static const int kPersonFieldNumber = 1;
  ::tutorial::Person* mutable_person(int index);
  ::google::protobuf::RepeatedPtrField< ::tutorial::Person >*
      mutable_person();
  const ::tutorial::Person& person(int index) const;
  ::tutorial::Person* add_person();
  const ::google::protobuf::RepeatedPtrField< ::tutorial::Person >&
      person() const;

  // @@protoc_insertion_point(class_scope:tutorial.AddressBook)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::tutorial::Person > person_;
  friend struct ::TableStruct_addressBook_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Person_PhoneNumber

// required string number = 1;
inline bool Person_PhoneNumber::has_number() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Person_PhoneNumber::clear_number() {
  number_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000001u;
}
inline const ::std::string& Person_PhoneNumber::number() const {
  // @@protoc_insertion_point(field_get:tutorial.Person.PhoneNumber.number)
  return number_.GetNoArena();
}
inline void Person_PhoneNumber::set_number(const ::std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tutorial.Person.PhoneNumber.number)
}
#if LANG_CXX11
inline void Person_PhoneNumber::set_number(::std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  number_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:tutorial.Person.PhoneNumber.number)
}
#endif
inline void Person_PhoneNumber::set_number(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  _has_bits_[0] |= 0x00000001u;
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tutorial.Person.PhoneNumber.number)
}
inline void Person_PhoneNumber::set_number(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000001u;
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tutorial.Person.PhoneNumber.number)
}
inline ::std::string* Person_PhoneNumber::mutable_number() {
  _has_bits_[0] |= 0x00000001u;
  // @@protoc_insertion_point(field_mutable:tutorial.Person.PhoneNumber.number)
  return number_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person_PhoneNumber::release_number() {
  // @@protoc_insertion_point(field_release:tutorial.Person.PhoneNumber.number)
  if (!has_number()) {
    return NULL;
  }
  _has_bits_[0] &= ~0x00000001u;
  return number_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person_PhoneNumber::set_allocated_number(::std::string* number) {
  if (number != NULL) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  number_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), number);
  // @@protoc_insertion_point(field_set_allocated:tutorial.Person.PhoneNumber.number)
}

// optional .tutorial.Person.PhoneType type = 2 [default = HOME];
inline bool Person_PhoneNumber::has_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Person_PhoneNumber::clear_type() {
  type_ = 1;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::tutorial::Person_PhoneType Person_PhoneNumber::type() const {
  // @@protoc_insertion_point(field_get:tutorial.Person.PhoneNumber.type)
  return static_cast< ::tutorial::Person_PhoneType >(type_);
}
inline void Person_PhoneNumber::set_type(::tutorial::Person_PhoneType value) {
  assert(::tutorial::Person_PhoneType_IsValid(value));
  _has_bits_[0] |= 0x00000002u;
  type_ = value;
  // @@protoc_insertion_point(field_set:tutorial.Person.PhoneNumber.type)
}

// -------------------------------------------------------------------

// Person

// required string name = 1;
inline bool Person::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Person::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000001u;
}
inline const ::std::string& Person::name() const {
  // @@protoc_insertion_point(field_get:tutorial.Person.name)
  return name_.GetNoArena();
}
inline void Person::set_name(const ::std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tutorial.Person.name)
}
#if LANG_CXX11
inline void Person::set_name(::std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:tutorial.Person.name)
}
#endif
inline void Person::set_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tutorial.Person.name)
}
inline void Person::set_name(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000001u;
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tutorial.Person.name)
}
inline ::std::string* Person::mutable_name() {
  _has_bits_[0] |= 0x00000001u;
  // @@protoc_insertion_point(field_mutable:tutorial.Person.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person::release_name() {
  // @@protoc_insertion_point(field_release:tutorial.Person.name)
  if (!has_name()) {
    return NULL;
  }
  _has_bits_[0] &= ~0x00000001u;
  return name_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:tutorial.Person.name)
}

// required uint32 id = 2;
inline bool Person::has_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Person::clear_id() {
  id_ = 0u;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::google::protobuf::uint32 Person::id() const {
  // @@protoc_insertion_point(field_get:tutorial.Person.id)
  return id_;
}
inline void Person::set_id(::google::protobuf::uint32 value) {
  _has_bits_[0] |= 0x00000004u;
  id_ = value;
  // @@protoc_insertion_point(field_set:tutorial.Person.id)
}

// optional string email = 3;
inline bool Person::has_email() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Person::clear_email() {
  email_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000002u;
}
inline const ::std::string& Person::email() const {
  // @@protoc_insertion_point(field_get:tutorial.Person.email)
  return email_.GetNoArena();
}
inline void Person::set_email(const ::std::string& value) {
  _has_bits_[0] |= 0x00000002u;
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tutorial.Person.email)
}
#if LANG_CXX11
inline void Person::set_email(::std::string&& value) {
  _has_bits_[0] |= 0x00000002u;
  email_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:tutorial.Person.email)
}
#endif
inline void Person::set_email(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  _has_bits_[0] |= 0x00000002u;
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tutorial.Person.email)
}
inline void Person::set_email(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000002u;
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tutorial.Person.email)
}
inline ::std::string* Person::mutable_email() {
  _has_bits_[0] |= 0x00000002u;
  // @@protoc_insertion_point(field_mutable:tutorial.Person.email)
  return email_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person::release_email() {
  // @@protoc_insertion_point(field_release:tutorial.Person.email)
  if (!has_email()) {
    return NULL;
  }
  _has_bits_[0] &= ~0x00000002u;
  return email_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person::set_allocated_email(::std::string* email) {
  if (email != NULL) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  email_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), email);
  // @@protoc_insertion_point(field_set_allocated:tutorial.Person.email)
}

// repeated .tutorial.Person.PhoneNumber phone = 4;
inline int Person::phone_size() const {
  return phone_.size();
}
inline void Person::clear_phone() {
  phone_.Clear();
}
inline ::tutorial::Person_PhoneNumber* Person::mutable_phone(int index) {
  // @@protoc_insertion_point(field_mutable:tutorial.Person.phone)
  return phone_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >*
Person::mutable_phone() {
  // @@protoc_insertion_point(field_mutable_list:tutorial.Person.phone)
  return &phone_;
}
inline const ::tutorial::Person_PhoneNumber& Person::phone(int index) const {
  // @@protoc_insertion_point(field_get:tutorial.Person.phone)
  return phone_.Get(index);
}
inline ::tutorial::Person_PhoneNumber* Person::add_phone() {
  // @@protoc_insertion_point(field_add:tutorial.Person.phone)
  return phone_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >&
Person::phone() const {
  // @@protoc_insertion_point(field_list:tutorial.Person.phone)
  return phone_;
}

// -------------------------------------------------------------------

// AddressBook

// repeated .tutorial.Person person = 1;
inline int AddressBook::person_size() const {
  return person_.size();
}
inline void AddressBook::clear_person() {
  person_.Clear();
}
inline ::tutorial::Person* AddressBook::mutable_person(int index) {
  // @@protoc_insertion_point(field_mutable:tutorial.AddressBook.person)
  return person_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::tutorial::Person >*
AddressBook::mutable_person() {
  // @@protoc_insertion_point(field_mutable_list:tutorial.AddressBook.person)
  return &person_;
}
inline const ::tutorial::Person& AddressBook::person(int index) const {
  // @@protoc_insertion_point(field_get:tutorial.AddressBook.person)
  return person_.Get(index);
}
inline ::tutorial::Person* AddressBook::add_person() {
  // @@protoc_insertion_point(field_add:tutorial.AddressBook.person)
  return person_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::tutorial::Person >&
AddressBook::person() const {
  // @@protoc_insertion_point(field_list:tutorial.AddressBook.person)
  return person_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace tutorial

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::tutorial::Person_PhoneType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::tutorial::Person_PhoneType>() {
  return ::tutorial::Person_PhoneType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // PROTOBUF_INCLUDED_addressBook_2eproto
