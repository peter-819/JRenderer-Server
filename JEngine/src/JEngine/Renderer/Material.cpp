#include "JEpch.h"
#include "Material.h"

namespace JEngine {

	//------------------------------- Class Material -------------------------------//

	Ref<Material> Material::Create(const Ref<Shader>& shader) {
		return std::make_shared<Material>(shader);
	}

	Material::Material(const Ref<Shader>& shader) {
		m_Shader = shader;
		AllocateStorage();
	}

	Material::~Material(){}

	void Material::Bind() const {
		m_Shader->Bind();
		if (m_VSUniformBuffer)
			m_Shader->SetVSMaterialUniformBuffer(m_VSUniformBuffer);
		if (m_PSUniformBuffer)
			m_Shader->SetPSMaterialUniformBuffer(m_PSUniformBuffer);
	}

	void Material::AllocateStorage() {
		const auto& vsbuffer = m_Shader->GetVSMaterialUItemBuffer();
		m_VSUniformBuffer.Allocate(vsbuffer.GetSize());
		m_VSUniformBuffer.ZeroInitialize();

		const auto& psbuffer = m_Shader->GetPSMaterialUItemBuffer();
		m_PSUniformBuffer.Allocate(psbuffer.GetSize());
		m_PSUniformBuffer.ZeroInitialize();
	}

	ShaderUniformItem* Material::FindUniformItem(const std::string& name) {
		if (m_VSUniformBuffer) {
			auto& items = m_Shader->GetVSMaterialUItemBuffer().GetUniformItems();
			for (auto item : items) {
				if (item->GetName() == name) 
					return item;
			}
		}
		if (m_PSUniformBuffer) {
			auto& items = m_Shader->GetPSMaterialUItemBuffer().GetUniformItems();
			for (auto item : items) {
				if (item->GetName() == name)
					return item;
			}
		}
		return nullptr;
	}

	Buffer& Material::GetUniformBufferTarget(ShaderUniformItem* item) {
		switch (item->GetDomain()) {
		case ShaderDomain::Vertex: return m_VSUniformBuffer;
		case ShaderDomain::Pixel: return m_PSUniformBuffer;
		}
		JE_CORE_ASSERT(0, "Invalid Shader Domain");
		return m_VSUniformBuffer;
	}

	//------------------------------- Class MaterialInstance -------------------------------//

	Ref<MaterialInstance> Create(const Ref<Material>& material) {
		return std::make_shared<MaterialInstance>(material);
	}

	MaterialInstance::MaterialInstance(const Ref<Material>& material) {
		m_Material = material;
		m_Material->m_MaterialInstances.insert(this);
		AllocateStorage();
	}
	
	MaterialInstance::~MaterialInstance() {
		m_Material->m_MaterialInstances.erase(this);
	}
	
	void MaterialInstance::Bind() const {
		if (m_VSUniformBuffer)
			m_Material->m_Shader->SetVSMaterialUniformBuffer(m_VSUniformBuffer);
		if (m_PSUniformBuffer)
			m_Material->m_Shader->SetPSMaterialUniformBuffer(m_PSUniformBuffer);
		
	}

	void MaterialInstance::AllocateStorage() {
		const auto& vsbuffer = m_Material->m_Shader->GetVSMaterialUItemBuffer();
		m_VSUniformBuffer.Allocate(vsbuffer.GetSize());
		memcpy(m_VSUniformBuffer.Data, m_Material->m_VSUniformBuffer.Data, vsbuffer.GetSize());

		const auto& psbuffer = m_Material->m_Shader->GetPSMaterialUItemBuffer();
		m_PSUniformBuffer.Allocate(psbuffer.GetSize());
		memcpy(m_PSUniformBuffer.Data, m_Material->m_PSUniformBuffer.Data, psbuffer.GetSize());
	}

	void MaterialInstance::OnMaterialValueUpdated(ShaderUniformItem* item) {
		auto& buffer = GetUniformBufferTarget(item);
		auto& materialbuffer = m_Material->GetUniformBufferTarget(item);
		buffer.Write(materialbuffer.Data + item->GetOffset(), item->GetSize(), item->GetOffset());
	}

	Buffer& MaterialInstance::GetUniformBufferTarget(ShaderUniformItem* item) {
		switch (item->GetDomain()) {
		case ShaderDomain::Vertex:	return m_VSUniformBuffer;
		case ShaderDomain::Pixel:	return m_PSUniformBuffer;
		}
		JE_CORE_ASSERT(0, "Invalid Shader Domain");
		return m_VSUniformBuffer;
	}
}