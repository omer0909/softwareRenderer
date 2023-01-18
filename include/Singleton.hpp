#pragma once

template <typename T> class Singleton
{
      public:
	inline static T &Get()
	{
		static T instance;
		return instance;
	}
};