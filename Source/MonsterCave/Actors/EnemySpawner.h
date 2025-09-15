#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MonsterCave/Characters/Enemy.h"

#include "EnemySpawner.generated.h"

UCLASS()
class MONSTERCAVE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.f;

	FTimerHandle SpawnTimer;

	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void OnSpawnTimerTimeout();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();
};