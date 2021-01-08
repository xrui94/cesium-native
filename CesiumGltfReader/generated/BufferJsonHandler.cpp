// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#include "BufferJsonHandler.h"
#include "CesiumGltf/Buffer.h"
#include <cassert>
#include <string>

using namespace CesiumGltf;

void BufferJsonHandler::reset(IJsonHandler* pParent, Buffer* pObject) {
  NamedObjectJsonHandler::reset(pParent, pObject);
  this->_pObject = pObject;
}

Buffer* BufferJsonHandler::getObject() {
  return this->_pObject;
}

void BufferJsonHandler::reportWarning(const std::string& warning, std::vector<std::string>&& context) {
  if (this->getCurrentKey()) {
    context.emplace_back(std::string(".") + this->getCurrentKey());
  }
  this->parent()->reportWarning(warning, std::move(context));
}

IJsonHandler* BufferJsonHandler::Key(const char* str, size_t /*length*/, bool /*copy*/) {
  assert(this->_pObject);
  return this->BufferKey(str, *this->_pObject);
}

IJsonHandler* BufferJsonHandler::BufferKey(const char* str, Buffer& o) {
  using namespace std::string_literals;

  if ("uri"s == str) return property("uri", this->_uri, o.uri);
  if ("byteLength"s == str) return property("byteLength", this->_byteLength, o.byteLength);

  return this->NamedObjectKey(str, *this->_pObject);
}
