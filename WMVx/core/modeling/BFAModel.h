#pragma once

#include "BFAM2Definitions.h"
#include "../filesystem/CascFileSystem.h"
#include "../database/BFAGameDatabase.h"
#include "../utility/Color.h"
#include "../utility/Matrix.h"
#include "../modeling/RawModel.h"
#include "../filesystem/ChunkedFile.h"

namespace core {

	class BFAModel : public RawModel
	{
	public:
		BFAModel() = default;
		BFAModel(BFAModel&&) = default;
		virtual ~BFAModel() = default;

		virtual void load(GameFileSystem* fs, GameFileUri uri, TextureCallback loadTexture);

		const std::vector<AnimationSequenceM2< M2_VER_RANGE::FROM(M2_VER_LEGION_PLUS)>>& getAnimationSequences() const {
			return animationSequences;
		}
		
		const std::vector<ModelGeosetM2< M2_VER_RANGE::FROM(M2_VER_LEGION_PLUS)>>& getGeosets() const {
			return geosets;
		}

		const std::vector<ModelAttachmentM2< M2_VER_RANGE::FROM(M2_VER_LEGION_PLUS)>>& getAttachmentDefinitions() const {
			return attachmentDefinitions;
		}


	protected:
		
		// apply a callback for each skeleton file, starting the at top most parent.
		template<typename fn>
		void processSkelFiles(GameFileSystem* fs, ArchiveFile* file, const ChunkedFile& chunked, fn callback, int32_t file_index = 0) {
			if (file != nullptr) {
				const auto skpd_chunk = chunked.get("SKPD");
				if (skpd_chunk != nullptr) {
					M2Chunk_SKPD skpd;
					assert(sizeof(skpd) <= skpd_chunk->size);
					file->read(&skpd, sizeof(skpd), skpd_chunk->offset);

					if (skpd.parentSkelFileId) {
						std::unique_ptr<CascFile> parent_file((CascFile*)fs->openFile(skpd.parentSkelFileId).release());
						if (parent_file != nullptr) {
							ChunkedFile parent_chunked;
							parent_chunked.open(parent_file.get());
							processSkelFiles(fs, parent_file.get(), parent_chunked, callback, ++file_index);
						}
					}
				}

				callback(file, chunked, file_index);
			}
		}


		BFAModelHeaderM2 header;

		std::vector<ModelGeosetM2< M2_VER_RANGE::FROM(M2_VER_LEGION_PLUS)>> geosets;
		std::vector<AnimationSequenceM2< M2_VER_RANGE::FROM(M2_VER_LEGION_PLUS)>> animationSequences;

		std::vector<ModelAttachmentM2< M2_VER_RANGE::FROM(M2_VER_LEGION_PLUS)>> attachmentDefinitions;

	};

};
