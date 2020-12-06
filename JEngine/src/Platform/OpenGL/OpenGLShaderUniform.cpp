#include "JEpch.h"
#include "OpenGLShaderUniform.h"

namespace JEngine {
	//---------------------------- Class OpenGLShaderUniformItem ----------------------------//
	OpenGLShaderUniformItem::OpenGLShaderUniformItem(ShaderDomain domain, Type type, const std::string& name, uint32_t count) {
		m_Type = type;
		m_Name = name;
		m_Domain = domain;
		m_Count = count;
		m_Size = SizeOfUniformType(type) * count;
	}

	void OpenGLShaderUniformItem::SetOffset(uint32_t offset) {
		//ShaderStruct
		m_Offset = offset;
	}

	uint32_t OpenGLShaderUniformItem::SizeOfUniformType(Type type) {
		switch (type) {
		case OpenGLShaderUniformItem::Type::INT32:      return 4;
		case OpenGLShaderUniformItem::Type::FLOAT32:    return 4;
		case OpenGLShaderUniformItem::Type::VEC2:       return 4 * 2;
		case OpenGLShaderUniformItem::Type::VEC3:       return 4 * 3;
		case OpenGLShaderUniformItem::Type::VEC4:       return 4 * 4;
		case OpenGLShaderUniformItem::Type::MAT3:       return 4 * 3 * 3;
		case OpenGLShaderUniformItem::Type::MAT4:       return 4 * 4 * 4;
		}
		return 0;
	}
	
	OpenGLShaderUniformItem::Type OpenGLShaderUniformItem::StringToType(const std::string& type) {
		if (type == "int32")    return Type::INT32;
		if (type == "float")    return Type::FLOAT32;
		if (type == "vec2")     return Type::VEC2;
		if (type == "vec3")     return Type::VEC3;
		if (type == "vec4")     return Type::VEC4;
		if (type == "mat3")     return Type::MAT3;
		if (type == "mat4")     return Type::MAT4;

		return Type::NONE;
	}

	std::string OpenGLShaderUniformItem::TypeToString(Type type) {
		switch (type)
		{
		case OpenGLShaderUniformItem::Type::INT32:      return "int32";
		case OpenGLShaderUniformItem::Type::FLOAT32:    return "float";
		case OpenGLShaderUniformItem::Type::VEC2:       return "vec2";
		case OpenGLShaderUniformItem::Type::VEC3:       return "vec3";
		case OpenGLShaderUniformItem::Type::VEC4:       return "vec4";
		case OpenGLShaderUniformItem::Type::MAT3:       return "mat3";
		case OpenGLShaderUniformItem::Type::MAT4:       return "mat4";
		}
		return "Invalid Type";
	}

	//---------------------------- Class OpenGLShaderUItemBuffer ----------------------------//
	OpenGLShaderUItemBuffer::OpenGLShaderUItemBuffer(const std::string& name, ShaderDomain domain) {
		m_Name = name;
		m_Domain = domain;
		m_Size = 0;
		m_Register = 0;
	}

	void OpenGLShaderUItemBuffer::PushUItem(OpenGLShaderUniformItem* uitem) {
		uint32_t offset = 0;
		if (m_UItems.size()) {
			OpenGLShaderUniformItem* pre = dynamic_cast<OpenGLShaderUniformItem*>(m_UItems.back());
			offset = pre->GetOffset() + pre->GetSize();
		}
		uitem->SetOffset(offset);
		m_Size += uitem->GetSize();
		m_UItems.push_back(uitem);
	}

	ShaderUniformItem* OpenGLShaderUItemBuffer::FindUniformItem(const std::string& name) {
		for (auto item : m_UItems) {
			if (item->GetName() == name)
				return item;
		}
		return nullptr;
	}
}