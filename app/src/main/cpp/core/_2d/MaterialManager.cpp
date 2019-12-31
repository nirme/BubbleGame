#include "MaterialManager.h"


template<>
core::_2d::MaterialManager* Singleton<core::_2d::MaterialManager>::impl = nullptr;

namespace core
{
    namespace _2d
    {
		Material::Material(MaterialId _id, ShadingProgramPtr _program, std::vector<TexturePtr> &_texture) :
			id(_id),
			program(_program)
		{
			textures.swap(_texture);
		};



		MaterialManager::MaterialKey::MaterialKey(const MaterialDefinition &_materialHandles) :
			materialHandles(_materialHandles),
			hash(createHash(_materialHandles))
		{};


		MaterialManager::MaterialKey::MaterialKey(MaterialDefinition &&_materialHandles) :
			hash(createHash(_materialHandles))
		{
			materialHandles.swap(_materialHandles);
		};


		bool MaterialManager::MaterialKey::operator==(const MaterialKey &_rhs) const
		{
			return materialHandles == _rhs.materialHandles;
		};

		
		size_t MaterialManager::MaterialKey::createHash(const MaterialDefinition &_def) const
		{
			size_t output = (size_t)(materialHandles[0] << 16); // make sure material is the most significant part of hash
			for (unsigned int i = 1, iEnd = materialHandles.size(); i < iEnd; ++i)
				output ^= (materialHandles[i] << (iEnd - i - 1));
			return output;
		};


		size_t MaterialManager::MaterialKey::Hash::operator()(const MaterialKey &_rhs) const noexcept
		{
			return _rhs.hash;
		};



        MaterialManager::MaterialManager() :
            nextFreeId(1)
        {};


		MaterialPtr MaterialManager::generateMaterial(ShadingProgramPtr _program, std::vector<TexturePtr> &_textureList)
        {
			assert(_textureList.size() <= 8 && "2D Material cannot have more than 8 textures");

			unsigned int texCount = _textureList.size();
			MaterialKey::MaterialDefinition definition({ _program->getHandle(), 
				texCount > 0 && _textureList[0] ? _textureList[0]->getHandle() : 0,
				texCount > 1 && _textureList[1] ? _textureList[1]->getHandle() : 0,
				texCount > 2 && _textureList[2] ? _textureList[2]->getHandle() : 0,
				texCount > 3 && _textureList[3] ? _textureList[3]->getHandle() : 0,
				texCount > 4 && _textureList[4] ? _textureList[4]->getHandle() : 0,
				texCount > 5 && _textureList[5] ? _textureList[5]->getHandle() : 0,
				texCount > 6 && _textureList[6] ? _textureList[6]->getHandle() : 0,
				texCount > 7 && _textureList[7] ? _textureList[7]->getHandle() : 0
			});

			MaterialKey key(definition);

			auto it = materialIdsList.find(key);
			if (it != materialIdsList.end())
				return (*it).second.lock();

			// deleter func will take care of removing unused material
			MaterialPtr material(new Material(nextFreeId++, _program, _textureList),
				[](Material *_material)
				{ 
					MaterialManager::getSingleton().removeById(_material->id);
				}
			);

            auto newEntry = materialIdsList.emplace(key, material);
			materialsByIds.emplace(material->id, *newEntry.first);

            return material;
        };

		MaterialPtr MaterialManager::generateMaterial(ShadingProgramPtr _program, TexturePtr _tex0, TexturePtr _tex1, TexturePtr _tex2, TexturePtr _tex3, TexturePtr _tex4, TexturePtr _tex5, TexturePtr _tex6, TexturePtr _tex7)
		{
			std::vector<TexturePtr> texList = {_tex0, _tex1, _tex2, _tex3, _tex4, _tex5, _tex6, _tex7};
			//MaterialPtr material = generateMaterial(_program, texList);
			//return material;
			return generateMaterial(_program, texList);
		};



        MaterialPtr MaterialManager::getById(MaterialId _id)
        {
            auto it = materialsByIds.find(_id);
            return it != materialsByIds.end() ? (*it).second.second.lock() : nullptr;
        };


		void MaterialManager::removeById(MaterialId _id)
		{
			auto it = materialsByIds.find(_id);
			if (it != materialsByIds.end())
			{
				materialIdsList.erase((*it).second.first);
				materialsByIds.erase(it);
			}
		};

    }
}
