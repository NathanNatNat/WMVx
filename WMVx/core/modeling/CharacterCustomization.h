#pragma once
#include "ModelSupport.h"
#include "../database/BFARecordDefinitions.h"
#include "../database/DFDatasetAdaptors.h"
#include "../database/DB2File.h"
#include <unordered_set>
#include <string>


namespace core {

	class GameFileSystem;
	class GameDatabase;
	class Model;
	class Scene;

	//TODO tidy name
	using ChrCustomization = std::unordered_map<std::string, uint32_t>;

	struct CharacterDetails {
		uint32_t raceId = 0;
		core::Gender gender = core::Gender::MALE;
		bool isHd = false;
	};

	class CharacterCustomizationProvider {
	public:
		CharacterCustomizationProvider() = default;
		CharacterCustomizationProvider(CharacterCustomizationProvider&&) = default;
		virtual ~CharacterCustomizationProvider() {}

		virtual void initialise(const CharacterDetails&) = 0;
		virtual void reset() = 0;

		/*
			Apply the choices to the model, this is when the provider is also able to update its internal context used for updating model.
			'apply' and 'update' are two seperate steps are there are chases where the model needs to be refreshed, but the choices/context wont have changed.
		*/
		bool apply(Model* model, const CharacterDetails& details, const ChrCustomization& choices);
		virtual bool updateContext(const CharacterDetails& details, const ChrCustomization& choices) = 0;

		/*
			use the internal state/context to update the model.
		*/
		virtual bool update(Model* model, CharacterTextureBuilder* builder, Scene* scene) = 0;

		virtual const ChrCustomization& getAvailableOptions() = 0;

		virtual CharacterComponentTextureAdaptor* getComponentTextureAdaptor(const CharacterDetails& details) = 0;
	};


	/// <summary>
	/// Character customization support for pre-shadowlands
	/// </summary>
	class LegacyCharacterCustomizationProvider : public CharacterCustomizationProvider {
		//TODO simplify by making seperate providers for (1.12 & 3.3.5) and 8.3
	public:

		struct Context {
			CharacterSectionRecordAdaptor* skin;
			CharacterSectionRecordAdaptor* face;
			CharacterSectionRecordAdaptor* hairColour;
			CharacterHairGeosetRecordAdaptor* hairStyle;
			CharacterFacialHairStyleRecordAdaptor* facialStyle;
			CharacterSectionRecordAdaptor* facialColour;
			CharacterSectionRecordAdaptor* underwear;

			Context() {
				skin = nullptr;
				face = nullptr;
				hairColour = nullptr;
				hairStyle = nullptr;
				facialStyle = nullptr;
				facialColour = nullptr;
				underwear = nullptr;
			}

			bool isValid() const {
				return skin != nullptr &&
					face != nullptr &&
					hairColour != nullptr &&
					hairStyle != nullptr &&
					facialStyle != nullptr;
				//note facial colour intentionally missed out, it isnt always needed. same with underwear
			}
		};



		LegacyCharacterCustomizationProvider(GameFileSystem* fs, GameDatabase* db) 
			: CharacterCustomizationProvider(),
			gameFS(fs), gameDB(db) {
			// ...
		}
		LegacyCharacterCustomizationProvider(LegacyCharacterCustomizationProvider&&) = default;
		virtual ~LegacyCharacterCustomizationProvider() {}

		virtual void initialise(const CharacterDetails& details); 
		virtual void reset();

	
		virtual bool updateContext(const CharacterDetails& details, const ChrCustomization& choices);
		virtual bool update(Model* model, CharacterTextureBuilder* builder, Scene* scene);


		virtual const ChrCustomization& getAvailableOptions() {
			return known_options;
		}

		virtual CharacterComponentTextureAdaptor* getComponentTextureAdaptor(const CharacterDetails& details);

	protected:
		GameFileSystem* gameFS;
		GameDatabase* gameDB;

	private:
		ChrCustomization known_options;	//value is option total

		std::unique_ptr<Context> context;

	};

	/// <summary>
	/// Character customization for shadowlands +
	/// </summary>
	class ModernCharacterCustomizationProvider : public CharacterCustomizationProvider {
	public:

		struct Context {

			struct Material {
				uint32_t custMaterialId;
				GameFileUri uri;
				int32_t textureType;
				int32_t layer;
				int32_t blendMode;
				uint16_t region;	//TODO real type
			};

			struct Option {
				std::vector<DFDB2ChrCustomizationGeosetRecord> geosets;
			/*	std::vector<DFDB2ChrCustomizationSkinnedModelRecord> models;*/
				std::vector<Material> materials;
			};

			std::vector<Option> options;
		};

		ModernCharacterCustomizationProvider(GameFileSystem* fs, GameDatabase* db);
		ModernCharacterCustomizationProvider(ModernCharacterCustomizationProvider&&) = default;
		virtual ~ModernCharacterCustomizationProvider() {}

		virtual void initialise(const CharacterDetails& details);
		virtual void reset();

		virtual bool updateContext(const CharacterDetails& details, const ChrCustomization& choices);
		virtual bool update(Model* model, CharacterTextureBuilder* builder, Scene* scene);

		virtual const ChrCustomization& getAvailableOptions() {
			return known_options;
		}

		virtual CharacterComponentTextureAdaptor* getComponentTextureAdaptor(const CharacterDetails& details);

	protected:
		GameFileSystem* gameFS;
		GameDatabase* gameDB;
		FileDataGameDatabase* fileDataDB;

	private:

		std::unique_ptr<Context> context;

		uint32_t getModelIdForCharacter(const CharacterDetails& details);

		ChrCustomization known_options;	//value is option total

		// name -> id format.
		std::unordered_map<std::string, uint32_t> cacheOptions;

		// option_id -> [choice_id...] format.
		std::unordered_map<uint32_t, std::vector<uint32_t>> cacheChoices;

		template<typename T>
		const T* findRecordById(const DB2File<T>& source, uint32_t id) {
			for (const auto& section : source.getSections()) {
				for (const auto& row : section.records) {
					if (row.data.id == id) {
						return &row;
					}
				}
			}


			return nullptr;
		}

		GameFileUri::id_t findTextureFileByMaterialId(uint32_t materialResId);

	};
}