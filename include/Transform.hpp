#pragma once

#include <Quaternion.hpp>
#include <Vector3.hpp>

class Transform {
   public:
	inline Transform() {}

	inline Transform(const Transform &other) {
		if (this != &other)
			*this = other;
	}

	inline Transform &operator=(const Transform &other) {
		if (this == &other)
			return *this;
		pos = other.pos;
		rotation = other.rotation;
		return *this;
	}

	inline Vector3 Forward() const {
		return -this->rotation * Vector3(0, 0, 1);
	}

	inline Vector3 Right() const {
		return -this->rotation * Vector3(1, 0, 0);
	}

	inline Vector3 Up() const {
		return -this->rotation * Vector3(0, 1, 0);
	}

	inline ~Transform() {}

	Vector3 pos;
	Quaternion rotation;
};