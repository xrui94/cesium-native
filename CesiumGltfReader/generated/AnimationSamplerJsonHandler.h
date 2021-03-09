// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/AnimationSampler.h"
#include "ExtensibleObjectJsonHandler.h"
#include "IntegerJsonHandler.h"

namespace CesiumGltf {
struct AnimationSampler;

class AnimationSamplerJsonHandler : public ExtensibleObjectJsonHandler {
public:
  void reset(IJsonHandler* pHandler, AnimationSampler* pObject);
  AnimationSampler* getObject();
  virtual void reportWarning(
      const std::string& warning,
      std::vector<std::string>&& context = std::vector<std::string>()) override;

  virtual IJsonHandler* Key(const char* str, size_t length, bool copy) override;

protected:
  IJsonHandler* AnimationSamplerKey(const char* str, AnimationSampler& o);

private:
  class InterpolationJsonHandler : public JsonHandler {
  public:
    void reset(IJsonHandler* pParent, AnimationSampler::Interpolation* pEnum);
    virtual IJsonHandler*
    String(const char* str, size_t length, bool copy) override;

  private:
    AnimationSampler::Interpolation* _pEnum = nullptr;
  };

  AnimationSampler* _pObject = nullptr;
  IntegerJsonHandler<int32_t> _input;
  InterpolationJsonHandler _interpolation;
  IntegerJsonHandler<int32_t> _output;
};
} // namespace CesiumGltf
