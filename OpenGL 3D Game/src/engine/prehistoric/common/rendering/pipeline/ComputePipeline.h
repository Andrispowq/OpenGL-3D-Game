#ifndef COMPUTE_PIPELINE_H
#define COMPUTE_PIPELINE_H

#include "Pipeline.h"

#include "engine/prehistoric/core/math/Math.h"

static uint32_t READ_ONLY = 0x1;
static uint32_t WRITE_ONLY = 0x2;
static uint32_t READ_WRITE = 0x3;

typedef uint32_t AccessMask;

class ComputePipeline
{
public:
	ComputePipeline(Vector3u invocationSize) : invocationSize(invocationSize) {}
	ComputePipeline() {}

	virtual ~ComputePipeline() {}

	std::unordered_map<uint32_t, std::pair<Texture*, AccessMask>>& getTextureBindingTable() { return textureBindingTable; }
	Vector3u getInvocationSize() const { return invocationSize; }

	void addTextureBinding(uint32_t binding, Texture* texture, AccessMask accessMask) { textureBindingTable.insert(std::make_pair(binding, std::make_pair(texture, accessMask))); }
	void setInvocationSize(const Vector3u& size) { this->invocationSize = size; }
protected:
	Vector3u invocationSize;

	std::unordered_map<uint32_t, std::pair<Texture*, AccessMask>> textureBindingTable;
};

#endif