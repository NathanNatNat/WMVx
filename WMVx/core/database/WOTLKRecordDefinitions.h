#pragma once

#include <cstdint>
#include "../utility/Vector3.h"
#include "DBCDefinitions.h"

namespace core {

	struct WOTLKDBCLangStringRef {
		DBCStringRef enUS; //also enGB
		DBCStringRef koKR;
		DBCStringRef frFR;
		DBCStringRef deDE;
		DBCStringRef enCN; //also zhCN
		DBCStringRef enTW; //also zhTW
		DBCStringRef esES;
		DBCStringRef esMX;
		DBCStringRef ruRU;
		DBCStringRef jaJP;
		DBCStringRef ptPT; //also ptBR
		DBCStringRef itIT;
		DBCStringRef unknown1;
		DBCStringRef unknown2;
		DBCStringRef unknown3;
		DBCStringRef unknown4;
		uint32_t flags;
	};

	struct WOTLKDBCAnimationDataRecord {
		uint32_t id;
		DBCStringRef name;
		uint32_t weaponFlags;
		uint32_t bodyFlags;
		uint32_t flags;
		uint32_t fallbackId;
		uint32_t behaviorId;
		uint32_t behaviorTier;
	};

	struct WOTLKDBCCharRacesRecord {
		uint32_t id;
		uint32_t flags;
		uint32_t factionId;
		uint32_t explorationSoundId;
		uint32_t maleModelDisplayId;
		uint32_t femaleModelDisplayId;
		DBCStringRef clientPrefix;
		uint32_t baseLanguage;
		uint32_t creatureTypeId;
		uint32_t resSicknessSpellId;
		uint32_t splashSoundId;
		DBCStringRef clientFileString;
		uint32_t cinematicSequenceId;
		uint32_t alliance; //0 alliance, 1 horde
		WOTLKDBCLangStringRef raceNameNeutral;
		WOTLKDBCLangStringRef raceNameFemale;
		WOTLKDBCLangStringRef raceNameMale;
		DBCStringRef facialHairCustomization[2];
		DBCStringRef hairCustomization;
		uint32_t requiredExpansion;
	};

	struct WOTLKDBCCharSectionsRecord {
		//wow dev wiki and wmv have different names for some of these
		//TODO decide which naming is best.
		uint32_t id;
		uint32_t raceId;
		uint32_t sexId;
		uint32_t type;
		DBCStringRef texture1;
		DBCStringRef texture2;
		DBCStringRef texture3;
		uint32_t flags;
		uint32_t section;
		uint32_t variationIndex;
	};

	struct WOTLKDBCCharacterFacialHairStylesRecord {
		uint32_t raceId;
		uint32_t sexId;
		uint32_t variationId;
		uint32_t geoset1;
		uint32_t geoset2;
		uint32_t geoset3;
		uint32_t geoset4;
		uint32_t geoset5;
	};

	struct WOTLKDBCCharHairGeosetsRecord {
		uint32_t id;
		uint32_t raceId;
		uint32_t sexId;
		uint32_t hairType;	//TODO check name ? wmv and wow wiki named differently
		uint32_t geoset;
		uint32_t showScalp;
	};

	struct WOTLKDBCCreatureModelDataRecord {
		uint32_t id;
		uint32_t flags;
		DBCStringRef modelName;
		uint32_t sizeClass;
		float modelScale;
		uint32_t bloodLevelId;
		uint32_t footprintTextureId;
		float footprintTextureLength;
		float footprintTextureWidth;
		float footprintTextureScale;
		uint32_t foleyMaterialId;
		uint32_t footstepShakeSize;
		uint32_t deathThudShakeSize;
		uint32_t soundDataId;
		float collisionWidth;
		float collisionHeight;
		float mountHeight;
		Vector3 geoBoxMin;
		Vector3 geoBoxMax;
		float worldEffectScale;
		float attachedEffectScale;
		float missileCollisionRadius;
		float missileCollisionPush;
		float missileCollisionRaise;
	};

	struct WOTLKDBCCreatureDisplayInfoRecord {
		uint32_t id;
		uint32_t modelId;
		uint32_t soundId;
		uint32_t extraDisplayInformationId;
		float scale;
		uint32_t opacity;
		DBCStringRef texture[3];
		DBCStringRef portaitTexture;
		uint32_t bloodLevelId;
		uint32_t bloodId;
		uint32_t npcSoundsId;
		uint32_t particlesId;
		uint32_t geosetData;
		uint32_t objectEffectPackageId;
	};

	struct WOTLKDBCItemRecord {
		uint32_t id;
		uint32_t classId;
		uint32_t subClassId;
		uint32_t soundOverrideSubclassId;
		uint32_t materialId;
		uint32_t itemDisplayInfoId;
		uint32_t inventorySlotId;
		uint32_t sheatheTypeId;
	};

	struct WOTLKDBCItemDisplayInfoRecord {
		uint32_t id;
		DBCStringRef modelLeft;
		DBCStringRef modelRight;
		DBCStringRef modelLeftTexture;
		DBCStringRef modelRightTexture;
		DBCStringRef icon[2];
		uint32_t geosetGlovesFlags;	//TODO GEOSET NAMES DONT APPEAR TO ACCURATELY REFLECT WHAT THEY ARE USED FOR!
		uint32_t geosetBracerFlags;
		uint32_t geosetRobeFlags;
		uint32_t geosetBootsFlags;
		uint32_t spellVisualId;
		uint32_t groupSoundIndexId;
		uint32_t helmetGeosetVisMaleId;
		uint32_t helmentGeosetVisFemaleId;
		DBCStringRef textureUpperArm;
		DBCStringRef textureLowerArm;
		DBCStringRef textureHands;
		DBCStringRef textureUpperChest;
		DBCStringRef textureLowerChest;
		DBCStringRef textureUpperLeg;
		DBCStringRef textureLowerLeg;
		DBCStringRef textureFoot;
		uint32_t itemVisualId;
		uint32_t particleColorId;
	};


	struct WOTLKDBCItemVisualRecord {
		uint32_t id;
		uint32_t visualEffectId[5];
	};

	struct WOTLKDBCItemVisualEffectRecord {
		uint32_t id;
		DBCStringRef model;
	};

	struct WOTLKDBCSpellItemEnchantmentRecord {
		uint32_t id;
		uint32_t charges;
		uint32_t effects[3];
		uint32_t effectPointsMin[3];
		uint32_t effectPointsMax[3];
		uint32_t effectArgs[3];
		WOTLKDBCLangStringRef name;
		uint32_t itemVisualId;
		uint32_t flags;
		uint32_t srcItemId;
		uint32_t conditionId;
		uint32_t skillId;
		uint32_t skillLevel;
		uint32_t requiredLevel;
	};

}