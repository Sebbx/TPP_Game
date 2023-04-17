#include "MyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "Items/Weapon.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MyCharContext, 0);
		}
	}
	
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);  
}

void AMyCharacter::EKeyPressed()
{
	//UE_LOG(LogTemp, Warning, TEXT("E"));
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
	}
}

void AMyCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void AMyCharacter::PlayAttackMontage() const
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack montage"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0: SectionName = FName("Attack1"); break;
		case 1: SectionName = FName("Attack2"); break;
		default: break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AMyCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AMyCharacter::CanAttack()
{
	return (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped);
}


void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookingAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(JumpingAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &AMyCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::Attack);
		//EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AMyCharacter::Dodge);
	}
}

