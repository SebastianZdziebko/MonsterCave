#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

#include "MonsterCave/Components/StatsComponent.h"

#include "Enemy.generated.h"

UCLASS()
class MONSTERCAVE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/* Pointers to components */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* FlipbookComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStatsComponent* StatsComp;

	/* Enemy's animations flipbooks */
	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	UPaperFlipbook* IdleFlip;

	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	UPaperFlipbook* MoveFlip;

	/* Movement variables and flags */
	UPROPERTY(VisibleAnywhere)
	FVector2D Direction;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};