#pragma once

#include "JEngine/Renderer/ShaderUniform.h"

namespace JEngine {

	class OpenGLShaderUniformItem : public ShaderUniformItem {
	private:
		friend class OpenGLShaderUItemBuffer;
		friend class OpenGLShader;
	public:
		enum class Type {
			NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32, STRUCT
		};

	private:
		std::string m_Name;
		uint32_t m_Size;
		uint32_t m_Offset;
		uint32_t m_Count;
		
		ShaderDomain m_Domain;

		Type m_Type;
		//ShaderStruct
		mutable int32_t m_Location; //???

	public:
		OpenGLShaderUniformItem(ShaderDomain domain, Type type, const std::string& name, uint32_t count = 1);

		inline virtual const std::string& GetName() const override { return m_Name; }
		inline virtual uint32_t GetSize() const override { return m_Size; }
		inline virtual uint32_t GetCount() const override { return m_Count; }
		inline virtual uint32_t GetOffset() const override { return m_Offset; }
		inline virtual ShaderDomain GetDomain() const override { return m_Domain; }
		
		int32_t GetLocation() const { return m_Location; }
		inline Type GetType() const { return m_Type; }
		inline bool IsArray() const { return m_Count > 1; }

	protected:
		virtual void SetOffset(uint32_t offset) override;

	public:
		static uint32_t SizeOfUniformType(Type type);
		static Type StringToType(const std::string& type);
		static std::string TypeToString(Type type);
	};



	class OpenGLShaderUItemBuffer : public ShaderUItemBuffer {
	private:
		std::string m_Name;
		ShaderUItemList m_UItems;
		uint32_t m_Register;
		uint32_t m_Size;
		ShaderDomain m_Domain;

	public:
		OpenGLShaderUItemBuffer(const std::string& name, ShaderDomain domain);

		void PushUItem(OpenGLShaderUniformItem* uitem);

		inline virtual const std::string& GetName() const override { return m_Name; }
		inline virtual uint32_t GetRegister() const override { return m_Register; }
		inline virtual uint32_t GetSize() const override { return m_Size; }
		
		inline const ShaderUItemList& GetUniformItems() const override { return m_UItems; }
		ShaderUniformItem* FindUniformItem(const std::string& name) override;

		inline virtual ShaderDomain GetDomain() const { return m_Domain; }
	};
}