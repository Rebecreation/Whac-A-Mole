#pragma once

#include "CoreMinimal.h"

namespace FWmCollisionChannel
{
	//const ECollisionChannel Structure = ECC_GameTraceChannel1;
	//const ECollisionChannel PlacementMultiTrace = ECC_GameTraceChannel2;
}

namespace FWmCollisionProfile
{
	const FName NoCollision = TEXT("NoCollision");
	
	const FName Gardener = TEXT("WmGardener");
	const FName Mole = TEXT("WmGardenerHit");
	const FName Mole = TEXT("WmMole");
}
