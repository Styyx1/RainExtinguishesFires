#pragma once

namespace FireManipulator {
#define continueContainer RE::BSContainer::ForEachResult::kContinue

	class Manager : public ISingleton<Manager> {
	public:
		void FreezeReference(RE::TESObjectREFR* a_ref);
		void UnFreezeReference(RE::TESObjectREFR* a_ref);
		bool IsRefFrozen(RE::TESObjectREFR* a_ref);

		void ExtinguishAllFires();
		void ExtinguishFire(RE::TESObjectREFR* a_fire, const FireData* a_data);
		void RelightFire(RE::TESObjectREFR* a_fire);

	private:
		std::unordered_map<RE::TESObjectREFR*, bool> frozenRefs;
	};
}