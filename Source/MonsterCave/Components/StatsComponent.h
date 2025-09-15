#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MonsterCave/Enums/EStat.h"

#include "StatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MONSTERCAVE_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();

	/* Declare stats map */
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EStat>, float> Stats;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};