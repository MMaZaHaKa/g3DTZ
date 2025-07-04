#include "Extract.h"
#include "AnimManager.h"
#include "Utils.h"
#include "FileSystem.h"
#include <map>
#include <string>
#include <span>
#include <vector>
#include <memory>
#ifndef CPP17
#include <format>
#endif

static std::map<int, const char*> bones{
	{ 0, "Root"   },
	{ 1, "Pelvis" },
	{ 2, "Spine"  },
	{ 3, "Spine1" },
	{ 4, "Neck"   },
	{ 5, "Head"   },
	{ 6, "jaw"    },

	{ 21, "Bip01 R Clavicle" },
	{ 22, "R UpperArm"       },
	{ 23, "R Forearm"        },
	{ 24, "R Hand"           },
	{ 25, "R Finger"         },
	{ 26, "R Finger01"       },

	{ 31, "Bip01 L Clavicle" },
	{ 32, "L UpperArm"       },
	{ 33, "L Forearm"        },
	{ 34, "L Hand"           },
	{ 35, "L Finger"         },
	{ 36, "L Finger01"       },

	{ 41, "L Thigh"          },
	{ 42, "L Calf"           },
	{ 43, "L Foot"           },
	{ 44, "L Toe0"           },

	{ 51, "R Thigh" },
	{ 52, "R Calf"  },
	{ 53, "R Foot"  },
	{ 54, "R Toe0"  },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * IFP structures and write code based on aap's convifp                  *
 * https://github.com/aap/librwgta/blob/master/tools/convifp/convifp.cpp *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ifp
{
	enum eNodeType : int
	{
		HAS_ROT = 1,
		HAS_TRANS = 2,
		HAS_SCALE = 4
	};

	struct Header
	{
		char ident[4];
		uint32 size;
	};

	struct KeyFrame
	{
		CQuaternion rotation;
		float time;
		CVector translation;
		CVector scale;
	};

	class AnimNode
	{
	public:
		char name[24];
		int nodeId;
		int unknownId;
		int type;
		int numKeyFrames;
		KeyFrame* keyFrames = nullptr;

		~AnimNode() { delete[] keyFrames; }
		uint32 GetSize()
		{
			// ANIM
			uint32 sz = 8 + (unknownId == -2 ? 44 : 48);

			// K...
			if (numKeyFrames != 0)
			{
				sz += 8 + 4 * numKeyFrames;
				if (type & HAS_ROT)
					sz += 16 * numKeyFrames;
				if (type & HAS_TRANS)
					sz += 12 * numKeyFrames;
				if (type & HAS_SCALE)
					sz += 12 * numKeyFrames;
			}

			return sz;
		}
	};

	class AnimHierarchy
	{
	public:
		char name[24];
		int unknown;
		int numNodes;
		AnimNode* nodes = nullptr;

		~AnimHierarchy() { delete[] nodes; }
		uint32 GetSize()
		{
			uint32 sz = 8 + 8; // INFO
			for (int i = 0; i < numNodes; i++)
				sz += 8 + nodes[i].GetSize(); // CPAN
			return sz;
		}
	};

	class AnimPackage
	{
	private:
		static int rounded(int n) { return (n + 3) & ~3; }
		static void writestr(const char* s, FILE* f)
		{
			char buf[64];
			memset(buf, 0, 64);
			strncpy(buf, s, 64);
			fwrite(buf, 1, rounded(strlen(s) + 1), f);
		}

	public:
		char name[24]{};
		int numAnimations = 0;
		AnimHierarchy* animations = nullptr;

		~AnimPackage() { delete[] animations; }
		void WriteIFP(FILE* f)
		{
			static ifp::Header anpk = { { 'A', 'N', 'P', 'K' }, 0 };
			static ifp::Header info = { { 'I', 'N', 'F', 'O' }, 0 };
			static ifp::Header name = { { 'N', 'A', 'M', 'E' }, 0 };
			static ifp::Header dgan = { { 'D', 'G', 'A', 'N' }, 0 };
			static ifp::Header cpan = { { 'C', 'P', 'A', 'N' }, 0 };
			static ifp::Header anim = { { 'A', 'N', 'I', 'M' }, 0 };
			static ifp::Header kf;
			char buf[256];

			uint32 sz = 8 + 4 + rounded(strlen(this->name) + 1); // INFO
			for (int i = 0; i < this->numAnimations; ++i)
			{
				sz += 8 + rounded(strlen(this->animations[i].name) + 1); // NAME
				sz += 8 + this->animations[i].GetSize();                 // DGAN
			}

			anpk.size = sz;
			fwrite(&anpk, 1, sizeof(ifp::Header), f);
			info.size = 4 + strlen(this->name) + 1;
			fwrite(&info, 1, sizeof(ifp::Header), f);
			fwrite(&this->numAnimations, 1, 4, f);
			writestr(this->name, f);

			for (int i = 0; i < this->numAnimations; ++i)
			{
				ifp::AnimHierarchy& hier = this->animations[i];
				name.size = strlen(hier.name) + 1;
				fwrite(&name, 1, sizeof(ifp::Header), f);
				writestr(hier.name, f);

				dgan.size = hier.GetSize();
				fwrite(&dgan, 1, sizeof(ifp::Header), f);

				info.size = 8;
				fwrite(&info, 1, sizeof(ifp::Header), f);
				*reinterpret_cast<int*>(buf) = hier.numNodes;
				*reinterpret_cast<int*>(buf + 4) = hier.unknown; // unused
				fwrite(buf, 1, 8, f);

				for (int j = 0; j < hier.numNodes; ++j)
				{
					ifp::AnimNode& node = hier.nodes[j];
					cpan.size = node.GetSize();
					fwrite(&cpan, 1, sizeof(ifp::Header), f);
					anim.size = node.unknownId == -2 ? 44 : 48;
					fwrite(&anim, 1, sizeof(ifp::Header), f);
					memset(buf, 0, 24);
					strncpy(buf, node.name, 24);
					*reinterpret_cast<int*>(buf + 24) = 0;                     // unk unused
					*reinterpret_cast<int*>(buf + 28) = node.numKeyFrames;
					*reinterpret_cast<int*>(buf + 32) = 0;                     // unused. always 0. first key frame?
					*reinterpret_cast<int*>(buf + 36) = node.numKeyFrames - 1; // unused
					*reinterpret_cast<int*>(buf + 40) = node.nodeId;
					*reinterpret_cast<int*>(buf + 44) = node.unknownId;
					fwrite(buf, 1, anim.size, f);

					if (node.numKeyFrames == 0)
						continue;

					kf.ident[0] = 'K';
					kf.size = 4;
					if (node.type & HAS_ROT)
					{
						kf.ident[1] = 'R';
						kf.size += 16;
					}
					else kf.ident[1] = '0';

					if (node.type & HAS_TRANS)
					{
						kf.ident[2] = 'T';
						kf.size += 12;
					}
					else kf.ident[2] = '0';

					if (node.type & HAS_SCALE)
					{
						kf.ident[3] = 'S';
						kf.size += 12;
					}
					else kf.ident[3] = '0';
					kf.size *= node.numKeyFrames;
					fwrite(&kf, 1, sizeof(ifp::Header), f);

					for (int k = 0; k < node.numKeyFrames; k++)
					{
						ifp::KeyFrame& frame = node.keyFrames[k];
						if (node.type & HAS_ROT)
							fwrite(&frame.rotation, 4, 4, f);
						if (node.type & HAS_TRANS)
							fwrite(&frame.translation, 4, 3, f);
						if (node.type & HAS_SCALE)
							fwrite(&frame.scale, 4, 3, f);
						fwrite(&frame.time, 4, 1, f);
					}
				}
			}
		}
	};
}

#ifndef CPP17
void
ExportAnimations(std::FILE* outFile, std::string_view blockName, std::span<CAnimBlendTree*> animTrees) {
	// The name that will be used for unused bone names
	char unknownBoneTagName[32] = "Unknown BoneTag ";
	/*
	* Here we create a ifp::AnimPackage based on GAME.DTZ structures.
	* This ifp::AnimPackage will then be passed to WriteIFP() that will generate the desired .ifp file.
	*/
	ifp::AnimPackage animPackage{};
	blockName.copy(animPackage.name, std::size(animPackage.name) - 1);
	animPackage.numAnimations = animTrees.size();
	animPackage.animations = new ifp::AnimHierarchy[animPackage.numAnimations]();

	// Create anim hierarchies for animPackage
	for (std::size_t i = 0; i < animTrees.size(); ++i) {
		CAnimBlendTree* hier = animTrees[i];
		strcpy(animPackage.animations[i].name, hier->name);
		animPackage.animations[i].nodes = new ifp::AnimNode[hier->numSequences]();
		/*
		 * Some CAnimBlendSequences in VCS have 0 keyframes (because the sequence is linked to an unused bone and, unlike LCS, VCS ignores them completely).
		 * An ifp file isn't supposed to have empty sequences.
		 * This can make the game crash when trying to load extracted ifps,
		 * because it tries to calculate the total animation time based on keyframes that don't exist!
		 * So in the extraction we only keep the non-empty ones.
		 */
		int currSeq = 0;
		for (int32 n = 0; n < hier->numSequences; ++n)
		{
			CAnimBlendSequence& seq = hier->blendSequences[n];
			if (seq.numFrames == 0)
				continue;

			if (bones[seq.boneTag])
			{
				strcpy(animPackage.animations[i].nodes[currSeq].name, bones[seq.boneTag]);
			}
			else
			{
				_itoa(currSeq, unknownBoneTagName + 17, 10);
				strcpy(animPackage.animations[i].nodes[currSeq].name, unknownBoneTagName);
			}
			animPackage.animations[i].nodes[currSeq].nodeId = seq.boneTag;
			animPackage.animations[i].nodes[currSeq].type = seq.flag & 3;
			/*
			 * We're doing & 3 to keep only the first 2 flags:
			 *     1: SEQ_HAS_ROTATION    or HAS_ROT
			 *     2: SEQ_HAS_TRANSLATION or HAS_TRANS
			 * The rest of the flags would be assigned by the game when loading the anims, so we don't need them for ifp export.
			 * The original ifp file would also have flag 4 : HAS_SCALE, but this is ignored by the game, so in the end we're left with a HAS_TRANS or HAS_ROT sequence!
			 * This doesn't affect the overall integrity of the exported animation sequence at all.
			 */
			animPackage.animations[i].nodes[currSeq].numKeyFrames = seq.numFrames;
			animPackage.animations[i].nodes[currSeq].keyFrames = new ifp::KeyFrame[seq.numFrames]();
			for (int32 k = 0; k < seq.numFrames; ++k)
			{
				ifp::AnimNode& node = animPackage.animations[i].nodes[currSeq];
				if (seq.flag & SEQ_HAS_TRANSLATION)
				{
					KeyFrameTrans*& frames = reinterpret_cast<KeyFrameTrans*&>(seq.keyFrames);
					node.keyFrames[k].translation = frames[k].GetTranslation();
					node.keyFrames[k].time = (k > 0 ? node.keyFrames[k - 1].time : 0) + frames[k].GetDeltaTime(); // We add each dt because the frame time in ifp files is absolute
					if (seq.flag & SEQ_HAS_ROTATION)
						node.keyFrames[k].rotation = frames[k].GetRotation().GetInverted(); // The rotation quaternion is inverted in memory, so we revert it back
				}
				else if (seq.flag & SEQ_HAS_ROTATION)
				{
					node.keyFrames[k].rotation = seq.keyFrames[k].GetRotation().GetInverted(); // The rotation quaternion is inverted in memory, so we revert it back
					node.keyFrames[k].time = (k > 0 ? node.keyFrames[k - 1].time : 0) + seq.keyFrames[k].GetDeltaTime();
				}
			}
			currSeq++;
		}
		animPackage.animations[i].numNodes = currSeq;
	}
	animPackage.WriteIFP(outFile);
}

bool ExtractAnimations() {
#ifdef VCS
	if (!CFileSystem::CreateAndChangeFolder("anim")) {
		ErrorBoxCannotCreateFolder("anim");
		return false;
	}
#endif
	/*
	 * Extract each CAnimBlock that's loaded in memory
	 * Expected ones for LCS: ped.ifp
	 * Expected ones for VCS: ped.ifp, driveby.ifp, fight.ifp, swim.ifp
	 */
	for (int32 i = 0; i < CAnimManager::GetNumAnimBlocks(); ++i) try {
		CAnimBlock* pAnimBlock = CAnimManager::GetAnimationBlock(i);
		if (!pAnimBlock->m_loaded)
			continue;
		/* Create the output file */
		auto f = [pAnimBlock] {
			char filename[128]{};
			std::format_to(filename, "{}.ifp", pAnimBlock->m_name);
			if (std::FILE* f = fopen(filename, "wb")) {
				return std::unique_ptr<std::FILE, int(*)(std::FILE*)>(f, std::fclose);
			}
			throw std::runtime_error("Can't open output IFP file.");
		}();
		/* Collect all anim trees */
		std::vector<CAnimBlendTree*> treePtrs;
		treePtrs.reserve(pAnimBlock->numAnims);
		for (int32 i = 0; i < pAnimBlock->numAnims; ++i)
			treePtrs.push_back(CAnimManager::GetAnimation(pAnimBlock->firstIndex + i));
		/* Write all trees to the output file */
		ExportAnimations(f.get(), pAnimBlock->m_name, treePtrs);
	} catch (const std::exception& e) {
		ErrorBox("%s", e.what());
		return false;
	} catch (...) {
		ErrorBox("An error occurred.");
		return false;
	}

#ifdef VCS
	CFileSystem::ResetFolder();
#endif
	return true;
}
#endif