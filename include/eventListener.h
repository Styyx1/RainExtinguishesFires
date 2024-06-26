#pragma once

namespace {
	/*
	* Class for easier event registration.
	*/
	template <typename T, class E>
	class EventClass : public RE::BSTEventSink<T> {
	public:
		static E* GetSingleton() {
			static E singleton;
			return std::addressof(singleton);
		}

		bool RegisterListener() {
			RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(this);
			return true;
		}

		bool UnregisterListener() {
			RE::ScriptEventSourceHolder::GetSingleton()->RemoveEventSink(this);
			return true;
		}

	protected:
		EventClass() = default;
		~EventClass() = default;

		EventClass(const EventClass&) = delete;
		EventClass(EventClass&&) = delete;
		EventClass& operator=(const EventClass&) = delete;
		EventClass& operator=(EventClass&&) = delete;
	};
}

namespace Events {
#define continueEvent RE::BSEventNotifyControl::kContinue

	namespace Hit {
		class HitEvenetManager :
			public EventClass<RE::TESHitEvent, HitEvenetManager> {
		private:
			RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;
		};
	}

	namespace Load {
		class LoadEventManager :
			public EventClass<RE::TESCellAttachDetachEvent, LoadEventManager> {
		private:
			RE::BSEventNotifyControl ProcessEvent(const RE::TESCellAttachDetachEvent* a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>* a_eventSource) override;
		};
	}

	namespace InteriorExterior {
		class InteriorExteriorEventManager :
			public RE::BSTEventSink<RE::BGSActorCellEvent>,
			public ISingleton<InteriorExteriorEventManager> {
		public:
			bool RegisterListener();
			void AddInteriorExteriorListener(const RE::TESForm* a_form, bool a_listen);
			void SendInteriorExteriorEvent(bool a_movedToInterior);

		private:
			RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource) override;

			//Members
			SKSE::RegistrationSet<bool, bool> interiorExteriorChange{ "OnInteriorExteriorChange"sv };
		};
	}

	namespace Weather {
#define rainyFlag RE::TESWeather::WeatherDataFlag::kRainy
#define snowyFlag RE::TESWeather::WeatherDataFlag::kSnow 
		class WeatherEventManager : public ISingleton<WeatherEventManager> {
		public:
			bool InstallHook();
			bool IsRaining();
			void SetRainingFlag(bool a_raining);
			void UpdateWeatherFlag();
			void AddWeatherChangeListener(const RE::TESForm* a_form, bool a_listen);
			void SendWeatherChangeEvent(bool newWeatherIsRainy);

			//Hook stuff
			static inline RE::TESWeather* currentWeather{ nullptr };
			static inline void thunk(RE::TESRegion* a_region, RE::TESWeather* a_currentWeather);
			static inline REL::Relocation<decltype(thunk)> func;

		private:
			//Members
			bool                        isRaining{ false };
			SKSE::RegistrationSet<bool> weatherTransition{ "OnWeatherChange"sv };
		};
	}

	bool RegisterForEvents();
}