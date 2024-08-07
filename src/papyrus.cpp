#include "papyrus.h"
#include "eventListener.h"
#include "fireManipulator.h"
#include "fireRegister.h"

namespace Papyrus {
	std::vector<int> GetVersion(STATIC_ARGS) {
		std::vector<int> response{};
		response.push_back(Version::MAJOR);
		response.push_back(Version::MINOR);
		response.push_back(Version::PATCH);

		return response;
	}

	void RegisterForAccurateWeatherChange(STATIC_ARGS, const RE::TESForm* a_form) {
		if (!a_form) return;
		Events::Weather::WeatherEventManager::GetSingleton()->AddWeatherChangeListener(a_form, true);
	}

	void UnRegisterForAccurateWeatherChange(STATIC_ARGS, const RE::TESForm* a_form) {
		if (!a_form) return;
		Events::Weather::WeatherEventManager::GetSingleton()->AddWeatherChangeListener(a_form, false);
	}

	void RegisterForInteriorExteriorChange(STATIC_ARGS, const RE::TESForm* a_form) {
		if (!a_form) return;
		Events::InteriorExterior::InteriorExteriorEventManager::GetSingleton()->AddInteriorExteriorListener(a_form, true);
	}

	void UnRegisterForInteriorExteriorChange(STATIC_ARGS, const RE::TESForm* a_form) {
		if (!a_form) return;
		Events::InteriorExterior::InteriorExteriorEventManager::GetSingleton()->AddInteriorExteriorListener(a_form, false);
	}

	void ExtinguishAllLoadedFires(STATIC_ARGS) {
		FireManipulator::Manager::GetSingleton()->ExtinguishAllFires();
	}

	std::vector<RE::TESObjectREFR*> GetNearbyAssociatedReferences(STATIC_ARGS, RE::TESObjectREFR* a_center) {
		if (a_center != nullptr) {
			auto baseForm = a_center->GetBaseObject();
			const FireData* data = CachedData::Fires::GetSingleton()->GetFireData(baseForm);
			auto response = FireManipulator::Manager::GetSingleton()->GetNearbyAssociatedReferences(a_center, data);
			return response;
		}
	}

	void SetRainingFlag(STATIC_ARGS, bool a_isRaining) {
		Events::Weather::WeatherEventManager::GetSingleton()->SetRainingFlag(a_isRaining);
	}

	void FreezeFire(STATIC_ARGS, RE::TESObjectREFR* a_ref) {
		FireManipulator::Manager::GetSingleton()->FreezeReference(a_ref);
	}

	void UnFreezeFire(STATIC_ARGS, RE::TESObjectREFR* a_ref) {
		FireManipulator::Manager::GetSingleton()->UnFreezeReference(a_ref);
	}

	bool IsRaining(STATIC_ARGS) {
		return Events::Weather::WeatherEventManager::GetSingleton()->IsRaining();
	}

	void Bind(VM& a_vm) {
		BIND(GetVersion);
		BIND(ExtinguishAllLoadedFires);
		BIND(FreezeFire);
		BIND(UnFreezeFire);
		BIND(SetRainingFlag);
		BIND(GetNearbyAssociatedReferences);
		BIND(IsRaining);
		BIND_EVENT(RegisterForAccurateWeatherChange, true);
		BIND_EVENT(UnRegisterForAccurateWeatherChange, true);
		BIND_EVENT(RegisterForInteriorExteriorChange, true);
		BIND_EVENT(UnRegisterForInteriorExteriorChange, true);
	}

	bool RegisterFunctions(VM* a_vm) {
		Bind(*a_vm);
		return true;
	}
}