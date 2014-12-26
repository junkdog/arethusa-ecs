#pragma once

namespace ecs {
class NoCopy {
  protected:
	NoCopy() = default;
	~NoCopy() = default;
	NoCopy(const NoCopy&) = delete;
	NoCopy& operator=(const NoCopy&) = delete;
};
}
