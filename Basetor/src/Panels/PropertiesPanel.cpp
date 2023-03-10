#include "pch.h"
#include "PropertiesPanel.h"
#include "scene/SceneCamera.h"

#include <imgui.h>

namespace Base {
	PropertiesPanel::PropertiesPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void PropertiesPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void PropertiesPanel::SetSelectionContext(Entity selection)
	{
		m_SelectionContext = selection;
	}

	void PropertiesPanel::OnImGuiRender()
	{
		BASE_CORE_ASSERT(m_Context != nullptr, "No context (Scene) provided to the panel (SceneHierarchyPanel)");
		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents();
		}
		ImGui::End();
	}

	void PropertiesPanel::DrawComponents()
	{
		auto& entity = m_SelectionContext;
		if (!entity) return;

		if (entity.HasComponent< TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent< TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component"))
			{
				auto& translation = entity.GetComponent<TransformComponent>().Translation;
				ImGui::DragFloat3("Position", &translation.x, 0.05f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CircleComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CircleComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Circle Component"))
			{
				auto& circle = entity.GetComponent<CircleComponent>();

				ImGui::SliderFloat("Thickness", &circle.Thickness, 0.0f, 1.0f);
				ImGui::SliderFloat("Fade", &circle.Fade, 0.0f, 1.0f);
				ImGui::SliderFloat("Radius", &circle.Radius, 0.0f, 1.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<Perlin2dComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(Perlin2dComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Perlin Component"))
			{
				auto& perlin = entity.GetComponent<Perlin2dComponent>();


				bool changedO = ImGui::SliderInt("Octaves", &perlin.octaves, 1, 8);
				bool changedB = ImGui::SliderFloat("Bias", &perlin.bias, 0.001f, 10.0f);
				bool changedPrecision = ImGui::SliderFloat("Color interpolation precision", &perlin.colorInterpolationPrecision, 0.001f, 1.0f);
				bool changedImportant = changedO || changedB;

				if (changedPrecision && !changedImportant)
				{
					perlin.Noise->SetColorInterpolationPrecision(perlin.colorInterpolationPrecision);
					if (entity.HasComponent<TextureComponent>())
					{
						auto& sprite = entity.GetComponent<TextureComponent>().Texture;
						sprite = perlin.Noise->GenerateNoiseTexture();
					}
				}
				else if (changedImportant)
				{
					perlin.Noise->SetOctaves(perlin.octaves);
					perlin.Noise->SetBias(perlin.bias);
					perlin.Noise->SetColorInterpolationPrecision(perlin.colorInterpolationPrecision);
					perlin.Noise->GenerateNoise();
					if (entity.HasComponent<TextureComponent>())
					{
						auto& sprite = entity.GetComponent<TextureComponent>().Texture;
						sprite = perlin.Noise->GenerateNoiseTexture();
					}
				}

				ImGui::TreePop();
			}
		}

		// Physics
		if (entity.HasComponent< RigidBody2DComponent>())
		{
			if (entity.HasComponent<BoxColider2DComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(BoxColider2DComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Box Colider Component"))
				{
					auto& ent_bcol = entity.GetComponent<Base::BoxColider2DComponent>();

					ImGui::SliderFloat("Friction", &ent_bcol.Friction, 0.0f, 1.0f);
					ImGui::SliderFloat("Density", &ent_bcol.Density, 0.0f, 1.0f);
					ImGui::SliderFloat("Restitution", &ent_bcol.Restitution, 0.0f, 1.0f);
					ImGui::SliderFloat("RestitutionThreshold", &ent_bcol.RestitutionThreshold, 0.0f, 1.0f);

					ImGui::TreePop();
				}
			}
			else if (entity.HasComponent<CircleColider2DComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(CircleColider2DComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Circle Colider Component"))
				{
					auto& ent_ccol = entity.GetComponent<Base::CircleColider2DComponent>();

					ImGui::SliderFloat("Friction", &ent_ccol.Friction, 0.0f, 1.0f);
					ImGui::SliderFloat("Density", &ent_ccol.Density, 0.0f, 1.0f);
					ImGui::SliderFloat("Restitution", &ent_ccol.Restitution, 0.0f, 1.0f);
					ImGui::SliderFloat("RestitutionThreshold", &ent_ccol.RestitutionThreshold, 0.0f, 1.0f);
					ImGui::SliderFloat("Radius", &ent_ccol.Radius, 0.0f, 1.0f);

					ImGui::TreePop();
				}
			}
		}

		//Camera
		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera Component"))
			{
				auto& camera = entity.GetComponent<CameraComponent>();

				static const char* projectionTypeString[] = { "Perspective", "Orthographic", "OrthographicPX" };
				const char* currentProjection = projectionTypeString[(int)camera.Camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjection))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentProjection == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i]), isSelected)
						{
							currentProjection = projectionTypeString[i];
							camera.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::TreePop();
			}
		}
	}
}
