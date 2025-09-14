#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

#include "EnhancedInputComponent.h"
#include "EnhancedinputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"

#include "MonsterCave/Components/StatsComponent.h"

#include "TopDownCharacter.generated.h"

UCLASS()
class MONSTERCAVE_API ATopDownCharacter : public APawn
{
	GENERATED_BODY()

	/* Variables used to measure character's attack cooldown */
	UPROPERTY(VisibleAnywhere)
	FTimerHandle AttackTimer;

	UPROPERTY(VisibleAnywhere)
	int32 AttackTimerCount{ 0 };

	/* Pointer to player's controller */
	UPROPERTY(VisibleAnywhere)
	APlayerController* PlayerController;

public:
	ATopDownCharacter();

	/* Pointers to components */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* FlipbookComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStatsComponent* StatsComp;

	/* Character's animations flipbooks */
	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	UPaperFlipbook* IdleFlip;

	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	UPaperFlipbook* MoveFlip;

	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	UPaperFlipbook* AttackFlip;

	/* Input pointers */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
	UInputAction* AttackAction;

	/* Movement variables and flags */
	UPROPERTY(VisibleAnywhere)
	FVector2D Direction;

	UPROPERTY(VisibleAnywhere)
	bool bCanMove{ true };

	UPROPERTY(VisibleAnywhere)
	bool bCanAttack{ true };

protected:
	virtual void BeginPlay() override;

	/* Functions to manage player's movement inputs */
	void MoveTriggered(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);

	/* Attack logic with cooldown */
	void BaseAttack(const FInputActionValue& Value);
	void OnAttackCooldown();

	UFUNCTION()
	void AttackFinish();

	/* Player's movement logic */
	void Move(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};