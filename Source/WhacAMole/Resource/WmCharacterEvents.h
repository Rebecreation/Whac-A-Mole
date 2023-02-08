#pragma once

struct FWmCharacterEvents
{
public:
	
	FVector2D GardenerMovement = FVector2D::ZeroVector;
	bool bGardenerHit = false;
	bool bGardenerTryPickUp = false;
	
	FVector2D MoleMovement = FVector2D::ZeroVector;
	bool bMoleToggleBurrow = false;
	bool bMoleTryPickUp = false;

public:
	
	static FWmCharacterEvents* Get(const UObject* Context);
};
