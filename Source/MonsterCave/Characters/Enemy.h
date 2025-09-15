#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

#include "MonsterCave/Enums/EEnemyState.h"

#include "Enemy.generated.h"

class UStatsComponent;
class ATopDownCharacter;

UCLASS()
class MONSTERCAVE_API AEnemy : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere)
	float SpawnDelay{ 5.f };

	
public:	
	AEnemy();

	/*  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEnemyState> State;

	/* Pointer to components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* FlipbookComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStatsComponent* StatsComp;

	/* Enemy's animations flipbook */
	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	UPaperFlipbook* MoveFlip;

	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	UPaperFlipbook* SpawnFlip;

	/* Movement variables */
	UPROPERTY(VisibleAnywhere)
	FVector2D Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanFollow{ true };

	/* Pointer to the player */
	UPROPERTY(VisibleAnywhere)
	ATopDownCharacter* Player;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void StartSpawning();
	void EndSpawning();
};