#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EStat
{
	MaxHealth UMETA(DisplayName = "Max Health"),
	Health UMETA(DisplayName = "Health"),
	Speed UMETA(DisplayName = "Speed"),
	Damage UMETA(DisplayName = "Damage"),
	Mastery UMETA(DisplayName = "Mastery")
};