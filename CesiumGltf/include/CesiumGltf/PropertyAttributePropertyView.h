#pragma once

#include "CesiumGltf/AccessorView.h"
#include "CesiumGltf/PropertyAttributeProperty.h"
#include "CesiumGltf/PropertyTransformations.h"
#include "CesiumGltf/PropertyTypeTraits.h"
#include "CesiumGltf/PropertyView.h"

#include <cassert>
#include <cmath>
#include <cstdint>

namespace CesiumGltf {
/**
 * @brief Indicates the status of a property attribute property view.
 *
 * The {@link PropertyAttributePropertyView} constructor always completes
 * successfully. However it may not always reflect the actual content of the
 * corresponding property texture property. This enumeration provides the
 * reason.
 */
class PropertyAttributePropertyViewStatus : public PropertyViewStatus {
public:
  /**
   * @brief This property view was initialized from an invalid
   * {@link PropertyAttribute}.
   */
  static const int ErrorInvalidPropertyAttribute = 13;

  /**
   * @brief This property view is associated with a {@link ClassProperty} of an
   * unsupported type.
   */
  static const int ErrorUnsupportedProperty = 14;

  /**
   * @brief This property view was initialized with a primitive that does not
   * contain the specified attribute.
   */
  static const int ErrorMissingAttribute = 15;

  /**
   * @brief This property view's attribute does not have a valid accessor index.
   */
  static const int ErrorInvalidAccessor = 16;

  /**
   * @brief This property view's type does not match the type of the accessor it
   * uses.
   */
  static const int ErrorAccessorTypeMismatch = 17;

  /**
   * @brief This property view's component type does not match the type of the
   * accessor it uses.
   */
  static const int ErrorAccessorComponentTypeMismatch = 18;

  /**
   * @brief This property view's normalization does not match the normalization
   * of the accessor it uses.
   */
  static const int ErrorAccessorNormalizationMismatch = 19;

  /**
   * @brief This property view uses an accessor that does not have a valid
   * buffer view index.
   */
  static const int ErrorInvalidBufferView = 20;

  /**
   * @brief This property view uses a buffer view that does not have a valid
   * buffer index.
   */
  static const int ErrorInvalidBuffer = 21;

  /**
   * @brief This property view uses an accessor that points outside the bounds
   * of its target buffer view.
   */
  static const PropertyViewStatusType ErrorAccessorOutOfBounds = 22;

  /**
   * @brief This property view uses a buffer view that points outside the bounds
   * of its target buffer.
   */
  static const PropertyViewStatusType ErrorBufferViewOutOfBounds = 23;
};

/**
 * @brief A view of the data specified by a {@link PropertyAttributeProperty}.
 *
 * Ideally, property attribute properties can be initialized as vertex
 * attributes in the target rendering context. However, some runtime engines do
 * not allow custom vertex attributes. To compensate, this view can be used to
 * sample the property attributes property via vertex index.
 *
 * @tparam ElementType The type of the elements represented in the property view
 * @tparam Normalized Whether or not the property is normalized. If normalized,
 * the elements can be retrieved as normalized floating-point numbers, as
 * opposed to their integer values.
 */
template <typename ElementType, bool Normalized = false>
class PropertyAttributePropertyView;

/**
 * @brief A view of the non-normalized data specified by a
 * {@link PropertyAttributeProperty}.
 *
 * Ideally, property attribute properties can be initialized as vertex
 * attributes in the target rendering context. However, some runtime engines do
 * not allow custom vertex attributes. This view can be used instead to sample
 * the property attributes property via vertex index.
 *
 * @tparam ElementType The type of the elements represented in the property view
 */
template <typename ElementType>
class PropertyAttributePropertyView<ElementType, false>
    : public PropertyView<ElementType, false> {
public:
  /**
   * @brief Constructs an invalid instance for a non-existent property.
   */
  PropertyAttributePropertyView() noexcept
      : PropertyView<ElementType, false>(), _accessor{} {}

  /**
   * @brief Constructs an invalid instance for an erroneous property.
   *
   * @param status The code from {@link PropertyAttributePropertyViewStatus} indicating the error with the property.
   */
  PropertyAttributePropertyView(PropertyViewStatusType status) noexcept
      : PropertyView<ElementType, false>(status), _accessor{} {
    assert(
        this->_status != PropertyAttributePropertyViewStatus::Valid &&
        "An empty property view should not be constructed with a valid status");
  }

  /**
   * @brief Construct a view of the data specified by a {@link PropertyAttributeProperty}.
   *
   * @param property The {@link PropertyAttributeProperty}
   * @param classProperty The {@link ClassProperty} this property conforms to.
   * @param accsesorView The {@link AccessorView} for the data that this property is
   * associated with.
   */
  PropertyAttributePropertyView(
      const PropertyAttributeProperty& property,
      const ClassProperty& classProperty,
      const AccessorView<ElementType>& accessorView) noexcept
      : PropertyView<ElementType, false>(classProperty, property),
        _accessor{accessorView} {}

  /**
   * @brief Gets the value of the property for the given texture coordinates
   * with all value transforms applied. That is, if the property specifies an
   * offset and scale, they will be applied to the value before the value is
   * returned.
   *
   * If this property has a specified "no data" value, this will return the
   * property's default value for any elements that equal this "no data" value.
   * If the property did not specify a default value, this returns std::nullopt.
   *
   * @param index The vertex index.
   *
   * @return The value of the property for the given vertex, or std::nullopt if
   * it matches the "no data" value
   */
  std::optional<ElementType> get(int64_t index) const noexcept {
    ElementType value = getRaw(index);

    if (value == this->noData()) {
      return this->defaultValue();
    }

    return transformValue(value, this->offset(), this->scale());
  }

  /**
   * @brief Gets the raw value of the property for the given vertex index.
   *
   * If this property has a specified "no data" value, the raw value will still
   * be returned, even if it equals the "no data" value.
   *
   * @param index The vertex index.
   *
   * @return The value of the property for the given vertex.
   */
  ElementType getRaw(int64_t index) const noexcept {
    assert(
        this->_status == PropertyAttributePropertyViewStatus::Valid &&
        "Check the status() first to make sure view is valid");
    assert(
        size() > 0 &&
        "Check the size() of the view to make sure it's not empty");
    assert(index >= 0 && "index must be non-negative");
    assert(index < size() && "index must be less than size");

    return _accessor[index];
  }

  /**
   * @brief Get the number of elements in this PropertyAttributePropertyView.
   * If the view is valid, this returns the count of the elements in the
   * attribute's accessor. Otherwise, this returns 0.
   *
   * @return The number of elements in this PropertyAttributePropertyView.
   */
  int64_t size() const noexcept {
    if (this->_status != PropertyAttributePropertyViewStatus::Valid) {
      return 0;
    }

    return _accessor.size();
  }

private:
  AccessorView<ElementType> _accessor;
};

/**
 * @brief A view of the normalized data specified by a
 * {@link PropertyAttributeProperty}.
 *
 * Ideally, property attribute properties can be initialized as vertex
 * attributes in the target rendering context. However, some runtime engines do
 * not allow custom vertex attributes. This view can be used instead to sample
 * the property attributes property via vertex index.
 *
 * @tparam ElementType The type of the elements represented in the property view
 */
template <typename ElementType>
class PropertyAttributePropertyView<ElementType, true>
    : public PropertyView<ElementType, true> {
private:
  using NormalizedType = typename TypeToNormalizedType<ElementType>::type;

public:
  /**
   * @brief Constructs an invalid instance for a non-existent property.
   */
  PropertyAttributePropertyView() noexcept
      : PropertyView<ElementType, true>(), _accessor{} {}

  /**
   * @brief Constructs an invalid instance for an erroneous property.
   *
   * @param status The code from {@link PropertyAttributePropertyViewStatus} indicating the error with the property.
   */
  PropertyAttributePropertyView(PropertyViewStatusType status) noexcept
      : PropertyView<ElementType, true>(status), _accessor{} {
    assert(
        this->_status != PropertyAttributePropertyViewStatus::Valid &&
        "An empty property view should not be constructed with a valid status");
  }

  /**
   * @brief Construct a view of the data specified by a {@link PropertyAttributeProperty}.
   *
   * @param property The {@link PropertyAttributeProperty}
   * @param classProperty The {@link ClassProperty} this property conforms to.
   * @param accsesorView The {@link AccessorView} for the data that this property is
   * associated with.
   */
  PropertyAttributePropertyView(
      const PropertyAttributeProperty& property,
      const ClassProperty& classProperty,
      const AccessorView<ElementType>& accessorView) noexcept
      : PropertyView<ElementType, true>(classProperty, property),
        _accessor{accessorView} {}

  /**
   * @brief Gets the value of the property for the given texture coordinates
   * with all value transforms applied. That is, if the property specifies an
   * offset and scale, they will be applied to the value before the value is
   * returned.
   *
   * If this property has a specified "no data" value, this will return the
   * property's default value for any elements that equal this "no data" value.
   * If the property did not specify a default value, this returns std::nullopt.
   *
   * @param index The vertex index.
   *
   * @return The value of the property for the given vertex, or std::nullopt if
   * it matches the "no data" value
   */
  std::optional<NormalizedType> get(int64_t index) const noexcept {
    ElementType value = getRaw(index);

    if (value == this->noData()) {
      return this->defaultValue();
    }

    if constexpr (IsMetadataScalar<ElementType>::value) {
      return transformValue<NormalizedType>(
          normalize<ElementType>(value),
          this->offset(),
          this->scale());
    }

    if constexpr (IsMetadataVecN<ElementType>::value) {
      constexpr glm::length_t N = ElementType::length();
      using T = typename ElementType::value_type;
      using NormalizedT = typename NormalizedType::value_type;
      return transformValue<glm::vec<N, NormalizedT>>(
          normalize<N, T>(value),
          this->offset(),
          this->scale());
    }

    if constexpr (IsMetadataMatN<ElementType>::value) {
      constexpr glm::length_t N = ElementType::length();
      using T = typename ElementType::value_type;
      using NormalizedT = typename NormalizedType::value_type;
      return transformValue<glm::mat<N, N, NormalizedT>>(
          normalize<N, T>(value),
          this->offset(),
          this->scale());
    }
  }

  /**
   * @brief Gets the raw value of the property for the given vertex index.
   *
   * If this property has a specified "no data" value, the raw value will still
   * be returned, even if it equals the "no data" value.
   *
   * @param index The vertex index.
   *
   * @return The value of the property for the given vertex.
   */
  ElementType getRaw(int64_t index) const noexcept {
    assert(
        this->_status == PropertyAttributePropertyViewStatus::Valid &&
        "Check the status() first to make sure view is valid");
    assert(
        size() > 0 &&
        "Check the size() of the view to make sure it's not empty");
    assert(index >= 0 && "index must be non-negative");
    assert(index < size() && "index must be less than size");

    return _accessor[index];
  }

  /**
   * @brief Get the number of elements in this PropertyAttributePropertyView.
   * If the view is valid, this returns the count of the elements in the
   * attribute's accessor. Otherwise, this returns 0.
   *
   * @return The number of elements in this PropertyAttributePropertyView.
   */
  int64_t size() const noexcept {
    if (this->_status != PropertyAttributePropertyViewStatus::Valid) {
      return 0;
    }

    return _accessor.size();
  }

private:
  AccessorView<ElementType> _accessor;
};

} // namespace CesiumGltf
