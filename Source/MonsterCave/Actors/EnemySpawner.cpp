#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	StartSpawning();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::StartSpawning() 
{
	GetWorldTimerManager().SetTimer
	(
		SpawnTimer,
		this,
		&AEnemySpawner::OnSpawnTimerTimeout,
		SpawnTime,
		false,
		SpawnTime
	);
}

void AEnemySpawner::StopSpawning() 
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::SpawnEnemy() 
{
	FVector EnemyLocation = GetActorLocation();
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemyLocation, FRotator::ZeroRotator);
}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}