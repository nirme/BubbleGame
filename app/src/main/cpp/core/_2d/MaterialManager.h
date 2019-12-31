#pragma once

#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
#include <array>

#include "../ShadingProgram.h"
#include "../SingletonTemplate.h"

#include "../Texture.h"



namespace core
{
	namespace _2d
	{
		typedef unsigned int MaterialId;

		struct Material
		{
			MaterialId id;
			ShadingProgramPtr program;
			std::vector<TexturePtr> textures;


			// hide ctor
		protected:
			friend class MaterialManager;
			Material(MaterialId _id, ShadingProgramPtr _program, std::vector<TexturePtr> &_texture);
		};

		typedef std::shared_ptr<const Material> MaterialPtr;
		typedef std::weak_ptr<const Material> MaterialWeakPtr;


		class MaterialManager : public Singleton<MaterialManager>
		{
		protected:


			struct MaterialKey
			{
				typedef unsigned long MaterialHash;
				typedef std::array<ResourceHandle, 9> MaterialDefinition;

				size_t hash;
				MaterialDefinition materialHandles;

				MaterialKey(const MaterialDefinition &_materialHandles);
				MaterialKey(MaterialDefinition &&_materialHandles);

				bool operator==(const MaterialKey &_rhs) const;
				struct Hash
				{
					size_t operator()(const MaterialKey &_rhs) const noexcept;
				};

			private:
				size_t createHash(const MaterialDefinition &_def) const;
			};


			typedef std::unordered_map<MaterialKey, MaterialWeakPtr, MaterialKey::Hash> MaterialIdList;
			typedef std::unordered_map<MaterialId, std::pair<MaterialKey, MaterialWeakPtr>> MaterialDefByIdList;

			MaterialIdList materialIdsList;
			MaterialDefByIdList materialsByIds;

			MaterialId nextFreeId;


			MaterialId getNextFreeId()
			{
				return nextFreeId++;
			};

		public:

			MaterialManager();

			MaterialPtr generateMaterial(ShadingProgramPtr _program, std::vector<TexturePtr> &_textureList);
			MaterialPtr generateMaterial(ShadingProgramPtr _program, TexturePtr _tex0, TexturePtr _tex1 = nullptr, TexturePtr _tex2 = nullptr, TexturePtr _tex3 = nullptr, TexturePtr _tex4 = nullptr, TexturePtr _tex5 = nullptr, TexturePtr _tex6 = nullptr, TexturePtr _tex7 = nullptr);

			MaterialPtr getById(MaterialId _id);

			void removeById(MaterialId _id);


		};
	}
}
