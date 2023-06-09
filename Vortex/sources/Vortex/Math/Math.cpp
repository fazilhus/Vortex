#include <vtpch.hpp>
#include "Vortex/Math/Math.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Vortex {

	namespace Math {

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
			using namespace glm;

			mat4 localMatrix{ transform };

			if (epsilonEqual(localMatrix[3][3], static_cast<float>(0), epsilon<float>())) {
				return false;
			}

			if (epsilonEqual(localMatrix[0][3], static_cast<float>(0), epsilon<float>()) ||
				epsilonEqual(localMatrix[1][3], static_cast<float>(0), epsilon<float>()) ||
				epsilonEqual(localMatrix[2][3], static_cast<float>(0), epsilon<float>())) {
				localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<float>(0);
				localMatrix[3][3] = static_cast<float>(1);
			}

			translation = vec3(localMatrix[3]);
			localMatrix[3] = vec4(0, 0, 0, localMatrix[3][3]);

			vec3 row[3];

			for (length_t i = 0; i < 3; ++i) {
				for (length_t j = 0; j < 3; ++j) {
					row[i][j] = localMatrix[i][j];
				}
			}

			scale.x = length(row[0]);
			row[0] = detail::scale(row[0], static_cast<float>(1));
			scale.y = length(row[1]);
			row[1] = detail::scale(row[1], static_cast<float>(1));
			scale.z = length(row[2]);	
			row[2] = detail::scale(row[2], static_cast<float>(1));
			
			rotation.y = asin(-row[0][2]);
			if (cos(rotation.y) != 0) {
				rotation.x = atan2(row[1][2], row[2][2]);
				rotation.z = atan2(row[0][1], row[0][0]);
			}
			else {
				rotation.x = atan2(-row[2][0], row[1][1]);
				rotation.z = 0;
			}

			return true;
		}

	}

}