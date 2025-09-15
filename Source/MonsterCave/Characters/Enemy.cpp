#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

#include "MonsterCave/Components/StatsComponent.h"

#include "TopDownCharacter.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Create enemy's components */
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	FlipbookComp->SetupAttachment(RootComponent);

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComp"));

	/* Set enemy state */
	State = Spawning;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (!Player) {
		AActor* PlayerActor = UGameplayStatics::GetActorOfClass
		(
			GetWorld(),
			ATopDownCharacter::StaticClass()
		);

		if (PlayerActor) 
			Player = Cast<ATopDownCharacter>(PlayerActor);
	}

	StartSpawning();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == Waiting) 
	{
		SpawnDelay -= DeltaTime;

		if (SpawnDelay > 0) return;
		
		State = Alive;
	}

	if (State != Alive || !Player) return;

	/* Calculating movement direction */
	FVector StartLocation = GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();
	float EnemySpeed = StatsComp->Stats[EStat::Speed];

	FVector DirectionToPlayer = PlayerLocation - StartLocation;
	DirectionToPlayer = DirectionToPlayer.GetSafeNormal();

	/* Update enemy location based on offset */
	FVector OffSet(
		DirectionToPlayer.X * EnemySpeed * DeltaTime,
		0.f,
		DirectionToPlayer.Z * EnemySpeed * DeltaTime
	);

	AddActorWorldOffset(OffSet, true);
}

void AEnemy::StartSpawning()
{
	FlipbookComp->SetFlipbook(SpawnFlip);
	FlipbookComp->SetLooping(false);
	FlipbookComp->PlayFromStart();

	GetWorldTimerManager().SetTimer
	(
		SpawnTimer,
		this,
		&AEnemy::EndSpawning,
		SpawnFlip->GetTotalDuration(),
		false
	);
}

void AEnemy::EndSpawning()
{
	FlipbookComp->SetFlipbook(MoveFlip);
	FlipbookComp->SetLooping(true);
	FlipbookComp->PlayFromStart();
	State = Waiting;
}