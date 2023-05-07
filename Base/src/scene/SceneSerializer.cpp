#include "pch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "utils/yaml_specializations.h"
#include "render/gl/Texture.h"

#include "yaml-cpp/yaml.h"
#include <fstream>

#include "ScriptableEntity.h"
namespace Base {
	static inline void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << TagComponent::StructName;
			out << YAML::BeginMap; //Tag

			out << YAML::Key << "Tag" << YAML::Value << entity.GetTag();

			out << YAML::EndMap; //Tag
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << TransformComponent::StructName;
			out << YAML::BeginMap; //Transform

			auto& transform = entity.GetTransform();

			out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;

			out << YAML::EndMap; //Transform
		}

		if (entity.HasComponent<TextureComponent>())
		{
			out << YAML::Key << TextureComponent::StructName;
			out << YAML::BeginMap; //Sprite

			auto& texture = entity.GetComponent<TextureComponent>();

			out << YAML::Key << "Path" << YAML::Value << texture.Texture->GetPath();
			out << YAML::Key << "IsRelative" << YAML::Value << true; // TODO: Implement way to save texture path relative or full

			out << YAML::EndMap; //Sprite
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << SpriteComponent::StructName;
			out << YAML::BeginMap; //Sprite

			auto& sprite = entity.GetComponent<SpriteComponent>();

			out << YAML::Key << "Color" << YAML::Value << sprite.Color;

			out << YAML::EndMap; //Sprite
		}

		if (entity.HasComponent<CircleComponent>())
		{
			out << YAML::Key << CircleComponent::StructName;
			out << YAML::BeginMap; //Circle

			auto& circle = entity.GetComponent<CircleComponent>();

			out << YAML::Key << "Radius" << YAML::Value << circle.Radius;
			out << YAML::Key << "Thickness" << YAML::Value << circle.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circle.Fade;

			out << YAML::EndMap; //Circle
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << CameraComponent::StructName;
			out << YAML::BeginMap; //CameraComponent

			auto& camera_comp = entity.GetComponent<CameraComponent>();
			auto& camera = camera_comp.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; //Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; //Camera

			out << YAML::Key << "Primary" << YAML::Value << camera_comp.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << camera_comp.FixedAspectRatio;

			out << YAML::EndMap; //CameraComponent
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << RigidBody2DComponent::StructName;
			out << YAML::BeginMap; //RigidBody2D

			auto& rbody = entity.GetComponent<RigidBody2DComponent>();

			out << YAML::Key << "BodyType" << YAML::Value << (int)rbody.Type;
			out << YAML::Key << "FixedRotation" << YAML::Value << rbody.FixedRotation;

			out << YAML::EndMap; //RigidBody2D
		}

		if (entity.HasComponent<BoxColider2DComponent>())
		{
			out << YAML::Key << BoxColider2DComponent::StructName;
			out << YAML::BeginMap; //BoxColider2D

			auto& bcol = entity.GetComponent<BoxColider2DComponent>();

			out << YAML::Key << "Offset" << YAML::Value << bcol.Offset;
			out << YAML::Key << "Size" << YAML::Value << bcol.Size;
			out << YAML::Key << "Density" << YAML::Value << bcol.Density;
			out << YAML::Key << "Friction" << YAML::Value << bcol.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bcol.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bcol.RestitutionThreshold;

			out << YAML::EndMap; //BoxColider2D
		}

		if (entity.HasComponent<CircleColider2DComponent>())
		{
			out << YAML::Key << CircleColider2DComponent::StructName;
			out << YAML::BeginMap; //CircleColider2D

			auto& bcol = entity.GetComponent<CircleColider2DComponent>();

			out << YAML::Key << "Offset" << YAML::Value << bcol.Offset;
			out << YAML::Key << "Size" << YAML::Value << bcol.Size;
			out << YAML::Key << "Density" << YAML::Value << bcol.Density;
			out << YAML::Key << "Friction" << YAML::Value << bcol.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bcol.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bcol.RestitutionThreshold;
			out << YAML::Key << "Radius" << YAML::Value << bcol.Radius;

			out << YAML::EndMap; //CircleColider2D
		}

		if (entity.HasComponent<Perlin2dComponent>())
		{
			out << YAML::Key << Perlin2dComponent::StructName;
			out << YAML::BeginMap; //Perlin2d

			auto& perlin = entity.GetComponent<Perlin2dComponent>();

			out << YAML::Key << "Octaves" << YAML::Value << perlin.octaves;
			out << YAML::Key << "Bias" << YAML::Value << perlin.bias;
			out << YAML::Key << "ColorInterpolationPrecision" << YAML::Value << perlin.colorInterpolationPrecision;
			out << YAML::Key << "MinimumColor" << YAML::Value << perlin.minimumColor;
			out << YAML::Key << "NaxColor" << YAML::Value << perlin.maxColor;

			out << YAML::EndMap; //Perlin2d
		}

		out << YAML::EndMap; // Entity
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		//Not implemented
		BASE_ASSERT(false);
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (const YAML::ParserException& e)
		{
			return false;
		}
		catch (const std::exception& e)
		{
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		BASE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				//UUID
				UUID_T uuid = entity["Entity"].as<UUID_T>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				BASE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserialized_entity = m_Scene->CreateEntityWhithUUID(uuid, name);

				//Transform
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserialized_entity.GetTransform();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				}

				//Sprite
				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto& spr = deserialized_entity.AddComponent<SpriteComponent>();

					spr.Color = spriteComponent["Color"].as<glm::vec4>();
				}

				//Texture
				auto textureComponent = entity["TextureComponent"];
				if (textureComponent)
				{
					const std::string texturePath = textureComponent["Path"].as<std::string>();
					auto& texture = deserialized_entity.AddComponent<TextureComponent>(Base::render::Texture::CreateTexture(texturePath, "TODO: RENAME THIS IDK"));
				}

				//Circle
				auto circleComponent = entity["CircleComponent"];
				if (circleComponent)
				{
					auto& circle = deserialized_entity.AddComponent<CircleComponent>();

					circle.Radius = circleComponent["Radius"].as<float>();
					circle.Thickness = circleComponent["Thickness"].as<float>();
					circle.Fade = circleComponent["Fade"].as<float>();
				}

				//Camera
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserialized_entity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Camera.SetViewportSize(WindowProps().width, WindowProps().height);

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				//RigidBody2DComponent
				auto rigiComponent = entity["RigidBody2DComponent"];
				if (rigiComponent)
				{
					auto& rbody = deserialized_entity.AddComponent<RigidBody2DComponent>();

					rbody.Type = (RigidBody2DComponent::BodyType)rigiComponent["BodyType"].as<int>();
					rbody.FixedRotation = rigiComponent["FixedRotation"].as<bool>();
				}

				//BoxColider2DComponent
				auto boxComponent = entity["BoxColider2DComponent"];
				if (boxComponent)
				{
					auto& bbody = deserialized_entity.AddComponent<BoxColider2DComponent>();

					bbody.Offset = boxComponent["Offset"].as<glm::vec2>();
					bbody.Size = boxComponent["Size"].as<glm::vec2>();
					bbody.Density = boxComponent["Density"].as<float>();
					bbody.Friction = boxComponent["Friction"].as<float>();
					bbody.Restitution = boxComponent["Restitution"].as<float>();
					bbody.RestitutionThreshold = boxComponent["RestitutionThreshold"].as<float>();
				}

				//CircleColider2DComponent
				auto circleColiderComponent = entity["CircleColider2DComponent"];
				if (circleColiderComponent)
				{
					auto& circleBody = deserialized_entity.AddComponent<CircleColider2DComponent>();

					circleBody.Offset = circleColiderComponent["Offset"].as<glm::vec2>();
					circleBody.Size = circleColiderComponent["Size"].as<glm::vec2>();
					circleBody.Density = circleColiderComponent["Density"].as<float>();
					circleBody.Friction = circleColiderComponent["Friction"].as<float>();
					circleBody.Restitution = circleColiderComponent["Restitution"].as<float>();
					circleBody.RestitutionThreshold = circleColiderComponent["RestitutionThreshold"].as<float>();
					circleBody.Radius = circleColiderComponent["Radius"].as<float>();
				}

				//Perlin2dComponent
				auto perlinComponent = entity["Perlin2dComponent"];
				if (perlinComponent)
				{
					auto& perlin = deserialized_entity.AddComponent<Perlin2dComponent>();

					perlin.octaves = perlinComponent["Octaves"].as<float>();
					perlin.bias = perlinComponent["Bias"].as<float>();
					perlin.colorInterpolationPrecision = perlinComponent["colorInterpolationPrecision"].as<float>();
					perlin.maxColor = perlinComponent["MaxColor"].as<glm::vec3>();
					perlin.minimumColor = perlinComponent["MinimumColor"].as <glm::vec3>();
					// perlin.Noise = perlinComponent["Noise"].as<float>();
				}
			}
		}
		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		//Not implemented
		BASE_ASSERT(false);
		return false;
	}

}