#pragma once

#include "Library.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <optional>

namespace CesiumGeometry {

/**
 * @brief A ray that extends infinitely from the provided origin in the provided
 * direction.
 */
class CESIUMGEOMETRY_API Ray final {
public:
  /**
   * @brief Construct a new ray.
   *
   * @param origin The origin of the ray.
   * @param direction The direction of the ray (normalized).
   *
   * @exception std::exception `direction` must be normalized.
   */
  Ray(const glm::dvec3& origin, const glm::dvec3& direction);

  /**
   * @brief Gets the origin of the ray.
   */
  const glm::dvec3& getOrigin() const noexcept { return this->_origin; }

  /**
   * @brief Gets the direction of the ray.
   */
  const glm::dvec3& getDirection() const noexcept { return this->_direction; }

  /**
   * @brief Calculates the point on the ray that corresponds to the given
   * parameter `t`. If `t` is negative, the function returns std::nullopt.
   *
   * @param t The parameter value used in the ray equation.
   * @return The point along the ray, or std::nullopt if t is negative.
   */
  std::optional<glm::dvec3> getPointAlongRay(double t) const noexcept;

  /**
   * @brief Transforms the ray using a given 4x4 transformation matrix.
   *
   * @param transformation The 4x4 transformation matrix used to transform the
   * ray.
   * @return The transformed ray.
   */

  Ray transform(const glm::dmat4x4& transformation) const noexcept;

  /**
   * @brief Constructs a new ray with its direction opposite this one.
   */
  Ray operator-() const noexcept;

private:
  glm::dvec3 _origin;
  glm::dvec3 _direction;
};
} // namespace CesiumGeometry
