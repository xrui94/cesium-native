// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#include "SamplerJsonHandler.h"
#include "CesiumGltf/Sampler.h"
#include <cassert>
#include <string>

using namespace CesiumGltf;

void SamplerJsonHandler::reset(IJsonHandler* pParent, Sampler* pObject) {
  NamedObjectJsonHandler::reset(pParent, pObject);
  this->_pObject = pObject;
}

Sampler* SamplerJsonHandler::getObject() {
  return this->_pObject;
}

void SamplerJsonHandler::reportWarning(const std::string& warning, std::vector<std::string>&& context) {
  if (this->getCurrentKey()) {
    context.emplace_back(std::string(".") + this->getCurrentKey());
  }
  this->parent()->reportWarning(warning, std::move(context));
}

IJsonHandler* SamplerJsonHandler::Key(const char* str, size_t /*length*/, bool /*copy*/) {
  assert(this->_pObject);
  return this->SamplerKey(str, *this->_pObject);
}

IJsonHandler* SamplerJsonHandler::SamplerKey(const char* str, Sampler& o) {
  using namespace std::string_literals;

  if ("magFilter"s == str) return property("magFilter", this->_magFilter, o.magFilter);
  if ("minFilter"s == str) return property("minFilter", this->_minFilter, o.minFilter);
  if ("wrapS"s == str) return property("wrapS", this->_wrapS, o.wrapS);
  if ("wrapT"s == str) return property("wrapT", this->_wrapT, o.wrapT);

  return this->NamedObjectKey(str, *this->_pObject);
}
