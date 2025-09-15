#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EEnemyState
{
	Spawning UMETA(DisplayName = "Spawning"),
	Waiting UMETA(DisplayName = "Waiting"),
	Alive UMETA(DisplayName = "Alive"),
	Dead UMETA(DisplayName = "Dead")
};