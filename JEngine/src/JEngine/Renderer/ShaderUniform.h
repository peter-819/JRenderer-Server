#pragma once

#include "JEngine/Core/core.h"

namespace JEngine {

	enum class ShaderDomain {
		None, Vertex, Pixel
	};

	class ShaderUniformItem {
	public:
		virtual const std::string& GetName() const = 0;
		virtual uint32_t GetSize() const = 0;
		virtual uint32_t GetCount() const = 0;
		virtual uint32_t GetOffset() const = 0;
		virtual ShaderDomain GetDomain() const = 0;

	protected:
		virtual void SetOffset(uint32_t offset) = 0;
	};
	
	typedef std::vector<ShaderUniformItem*> ShaderUItemList;

	class ShaderUItemBuffer {
	public:
		virtual const std::string& GetName() const = 0;
		virtual uint32_t GetRegister() const = 0;
		virtual uint32_t GetSize() const = 0;
		virtual const ShaderUItemList& GetUniformItems() const = 0;
		virtual ShaderUniformItem* FindUniformItem(const std::string& name) = 0;
	};

	typedef std::vector<ShaderUItemBuffer*> ShaderUItemBufferList;

}