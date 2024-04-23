#pragma once
#include "RawModel.h"
#include "Animator.h"
#include "Attachment.h"
#include "MergedModel.h"
#include "ModelSupport.h"
#include <memory>
#include <vector>
#include "../game/GameClientAdaptor.h"
#include "TabardCustomization.h"
#include "TextureSet.h"

namespace core {

	/// <summary>
	/// Helper class for managing an equiped item, specifically for when a 'item' record doesnt exist in the DB, so a dummy is created which is owned by wrapper.
	/// </summary>
	class CharacterItemWrapper {
	public:
		CharacterItemWrapper() : _item(nullptr), _display(nullptr), _owning(false), _owned_item(nullptr) {}
		virtual ~CharacterItemWrapper() {};

		static CharacterItemWrapper make(const ItemRecordAdaptor* item, GameDatabase* db) {
			const auto* display = db->itemDisplayDB->findById(item->getItemDisplayInfoId());
			if (display == nullptr) {
				throw std::runtime_error("Unable to find display record.");
			}
			return CharacterItemWrapper(item, display);
		}

		static CharacterItemWrapper make(ItemInventorySlotId slot, const ItemDisplayRecordAdaptor* display) {
			std::shared_ptr<ItemRecordAdaptor> dummy_src = std::make_shared<DummyItemRecordAdaptor>();
			auto* dummy = (DummyItemRecordAdaptor*)dummy_src.get();
			dummy->display_info_id = display->getId();
			dummy->inventory_slot_id = slot;
			dummy->sheath_type = SheathTypes::SHEATHETYPE_NONE;
			dummy->name = QString("Display Id: %1").arg(dummy->display_info_id);
			return CharacterItemWrapper(dummy_src, display);
		}

		const ItemRecordAdaptor* item() const {

			if (_owning) {
				return _owned_item.get();
			}

			return _item;
		}

		const ItemDisplayRecordAdaptor* display() const {
			return _display;
		}

		bool operator==(const CharacterItemWrapper& rhs) const {
			return (item()->getId() == rhs.item()->getId()) && 
				(display()->getId() && rhs.display()->getId());
		}

	protected:

		const ItemRecordAdaptor* _item;
		std::shared_ptr<ItemRecordAdaptor> _owned_item;
		const ItemDisplayRecordAdaptor* _display;

		class DummyItemRecordAdaptor : public ItemRecordAdaptor {
		public:
			constexpr uint32_t getId() const override {
				return 0;
			}

			constexpr uint32_t getItemDisplayInfoId() const override {
				return display_info_id;
			}

			constexpr ItemInventorySlotId getInventorySlotId() const override {
				return inventory_slot_id;
			}

			constexpr SheathTypes getSheatheTypeId() const override {
				return sheath_type;
			}

			constexpr ItemQualityId getItemQuality() const override {
				return ItemQualityId::NORMAL;
			}

			QString getName() const override {
				return name;
			}

			uint32_t display_info_id;
			ItemInventorySlotId inventory_slot_id;
			SheathTypes sheath_type;
			QString name;
		};

		CharacterItemWrapper(const ItemRecordAdaptor* item_adaptor,
			const ItemDisplayRecordAdaptor* display_adaptor) :
			_item(item_adaptor),
			_display(display_adaptor),
			_owning(false),
			_owned_item(nullptr) {}

		CharacterItemWrapper(std::shared_ptr<ItemRecordAdaptor> owned_item_adaptor,
			const ItemDisplayRecordAdaptor* display_adaptor) :
		_item(nullptr),
		_display(display_adaptor),
		_owning(true),
		_owned_item(owned_item_adaptor)
		{}

		bool _owning;
	};

	class Scene;
	struct ModelMeta {
	public:
		friend class Scene;
		using id_t = uint32_t;

		ModelMeta() {
			_id = 0;
		}
		operator id_t() const { return _id; }
		ModelMeta& operator=(const ModelMeta&) = delete;

		id_t getId() const {
			return _id;
		}

		const QString& getName() const {
			return _name;
		}

		void setName(QString name) {
			_name = name;
		}

	private:
		id_t _id;
		QString _name;
	};

	class Model : public ModelTextureInfo, public ModelAnimationInfo, public ModelGeosetInfo, public HasMergedModels
	{
	public:
		Model(RawModel::Factory& factory);
		Model(Model&&) = default;
		virtual ~Model() {}

		void initialise(const GameFileUri& uri, GameFileSystem* fs, GameDatabase* db, TextureManager& manager);

		void update(uint32_t delta_time_msecs);

		ModelMeta meta;

		std::unique_ptr<RawModel> model;
		TextureSet textureSet;

		// character specific options
		std::map<CharacterSlot, CharacterItemWrapper> characterEquipment;
		CharacterCustomizations characterCustomizationChoices;
		std::optional<TabardCustomizationOptions> tabardCustomizationChoices;
		CharacterRenderOptions characterOptions;
		//


		bool animate;
		Animator animator;

		ModelRenderOptions renderOptions;

		const std::vector<Attachment*>& getAttachments() const {
			return reinterpret_cast<const std::vector<Attachment*>&>(attachments);
		}

		void addAttachment(std::unique_ptr<Attachment> attachment) {

			const auto pos = attachment->attachmentPosition;
			const auto slot = attachment->getSlot();

			std::erase_if(attachments, [pos, slot](const std::unique_ptr<Attachment>& att) -> bool {
				return att->attachmentPosition == pos && att->getSlot() == slot;
			});

			attachments.push_back(std::move(attachment));
		}


		void removeAttachments(CharacterSlot slot) {
			std::erase_if(attachments, [slot](const std::unique_ptr<Attachment>& att) -> bool {
				return att->getSlot() == slot;
			});
		}

		void setAttachmentPosition(Attachment* attachment, AttachmentPosition position) const {

			if (position >= AttachmentPosition::MAX) {
				throw std::runtime_error("Attachment position not valid.");
			}

			auto lookup_val = model->getAttachmentLookups()[(size_t)position];
			const auto& attachDef = model->getAttachmentDefintionAdaptors().at(lookup_val);
			
			attachment->setPosition(
				position, 
				attachDef->getBone(), 
				Vector3::yUpToZUp(attachDef->getPosition())
			);
		}



		const std::optional<CharacterDetails>& getCharacterDetails() const {
			assert(model->isCharacter() == characterDetails.has_value());
			return characterDetails;
		}

	protected:

		std::vector<std::unique_ptr<Attachment>> attachments;

		std::optional<CharacterDetails> characterDetails;

	};

};