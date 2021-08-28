#pragma once

class SVGBaseEntity;

class FuncTimer : public SVGBaseEntity {
public:
	FuncTimer( Entity* entity );
	virtual ~FuncTimer() = default;

	DefineMapClass( "func_timer", FuncTimer, SVGBaseEntity );

	// Spawn flags
	static constexpr int32_t SF_StartOn = 1 << 0;

	void Spawn() override;
	void SpawnKey( const std::string& key, const std::string& value ) override;

	void TimerThink();
	void TimerUse( SVGBaseEntity* other, SVGBaseEntity* activator );

protected:
	float randomTime{ 0.0f };
};