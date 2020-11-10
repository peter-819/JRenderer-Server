#pragma once
#include "JEngine/Core/Buffer.h"
#include "JEngine/Core/core.h"
#include "JEngine/Renderer/Shader.h"
#include "JEngine/Renderer/ShaderUniform.h"
#include <unordered_set>

namespace JEngine {
	class Material {
		friend class MaterialInstance;

	public:
		static Ref<Material> Create(const Ref<Shader>& shader);

	public:
		Material(const Ref<Shader>& shader);
		virtual ~Material();
		void Bind() const;

		template<typename T>
		void Set(const std::string& name, const T& value) {
			auto item = FindUniformItem(name);
			JE_CORE_ASSERT(item, "Can not find uniform {0}", name);
			auto& buffer = GetUniformBufferTarget(item);
			buffer.Write(static_cast<byte*>(&value), item->GetSize(), item->GetOffset());

			for (auto instance : m_MaterialInstances)
				instance->OnMaterialValueUpdated(item);
		}

	private:
		void AllocateStorage();
		ShaderUniformItem* FindUniformItem(const std::string& name);
		Buffer& GetUniformBufferTarget(ShaderUniformItem* item);

	private:
		Ref<Shader> m_Shader;
		std::unordered_set<MaterialInstance*> m_MaterialInstances;
		Buffer m_VSUniformBuffer;
		Buffer m_PSUniformBuffer;
	};




	class MaterialInstance {
	public:
		static Ref<MaterialInstance> Create(const Ref<Material>& material);

	public:
		MaterialInstance(const Ref<Material>& material);
		virtual ~MaterialInstance();
		void Bind() const ;

	private:
		void AllocateStorage();
		Buffer& GetUniformBufferTarget(ShaderUniformItem* item);
		void OnMaterialValueUpdated(ShaderUniformItem* item);

	private:
		Ref<Material> m_Material;
		
		Buffer m_VSUniformBuffer;
		Buffer m_PSUniformBuffer;
	};
}