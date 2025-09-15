#include "TopDownCharacter.h"

ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Create new capsule component and set it as root component */
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	/* Create flipbook component to store active sprite and attach it to capsule component */
	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	FlipbookComp->SetupAttachment(CapsuleComp);

	/* Create stats component for managing character's statistics */
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComp"));
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	
	/* Initialize input mapping context */
	if (PlayerController) 
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer());

		if (Subsystem)
			Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	FlipbookComp->OnFinishedPlaying.AddDynamic(this, &ATopDownCharacter::AttackFinish);
}

void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputController = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputController) return;

	/* Setup enhanced input action for start moving */
	EnhancedInputController->BindAction(
		MoveAction,
		ETriggerEvent::Triggered,
		this,
		&ATopDownCharacter::MoveTriggered);

	/* Setup enhanced input action for end moving */
	EnhancedInputController->BindAction(
		MoveAction,
		ETriggerEvent::Completed,
		this,
		&ATopDownCharacter::MoveCompleted);

	/* Setup enhanced input action for attack action */
	EnhancedInputController->BindAction(
		AttackAction,
		ETriggerEvent::Started,
		this,
		&ATopDownCharacter::BaseAttack);
}

void ATopDownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	FVector2D MoveActionValue = Value.Get<FVector2D>();

	Direction = MoveActionValue;

	FlipbookComp->SetFlipbook(MoveFlip);
	FVector FlipbookScale = FlipbookComp->GetComponentScale();

	/* Change sprite direction */
	if (Direction.X < 0.f && FlipbookScale.X > 0.f)
		FlipbookComp->SetWorldScale3D(FVector(-1.f, 1.f, 1.f));
	else if (Direction.X > 0.f && FlipbookScale.X < 0.f)
		FlipbookComp->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	Direction = FVector2D{ 0.f, 0.f };

	FlipbookComp->SetFlipbook(IdleFlip);
}

void ATopDownCharacter::BaseAttack(const FInputActionValue& Value)
{
	if (!bCanAttack) return;

	/* Direction's reset */
	Direction = FVector2D::ZeroVector;

	/* Setting flags to false. Player can't move during the attack */
	/* and can't attack again because cooldown */
	bCanAttack = false;
	bCanMove = false;

	/* Update flipbook component */
	FlipbookComp->SetFlipbook(AttackFlip);
	FlipbookComp->SetLooping(false);
	FlipbookComp->PlayFromStart();

	/* Disable controller for attack's time */
	if (PlayerController)
		DisableInput(PlayerController);

	/* Setup timer for character's attack cooldown */
	AttackTimerCount = 0;

	GetWorld()->GetTimerManager().SetTimer
	(
		AttackTimer,
		this,
		&ATopDownCharacter::OnAttackCooldown,
		6.f,
		true
	);
}

void ATopDownCharacter::OnAttackCooldown()
{
	AttackTimerCount++;
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	bCanAttack = true;
}

void ATopDownCharacter::AttackFinish()
{
	if (FlipbookComp->GetFlipbook() != AttackFlip) return;

	FlipbookComp->SetFlipbook(IdleFlip);
	FlipbookComp->SetLooping(true);
	FlipbookComp->PlayFromStart();
	bCanMove = true;
	EnableInput(PlayerController);
}

void ATopDownCharacter::Move(float DeltaTime) {

	if (!bCanMove) return;
	if (Direction.Length() == 0.f) return;

	Direction = Direction.GetSafeNormal();

	/* Calculating move values */
	float PlayerSpeed = StatsComp->Stats[EStat::Speed];
	FVector StartLocation = GetActorLocation();

	FVector Offset(
		Direction.X * PlayerSpeed * DeltaTime,
		0.f,
		Direction.Y * PlayerSpeed * DeltaTime
	);

	/*  */
	FHitResult Hit;
	AddActorWorldOffset(Offset, true, &Hit);

	if (!Hit.bBlockingHit) return;

	float RemainingAlpha = 1.f - Hit.Time;
	FVector Remaining = Offset * RemainingAlpha;

	FVector Slide = Remaining - FVector::DotProduct(Remaining, Hit.Normal) * Hit.Normal;

	FHitResult HitSlide;
	AddActorWorldOffset(Slide, true, &HitSlide);
}