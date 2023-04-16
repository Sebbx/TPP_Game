// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MannyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UMannyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MannyCharacter =  Cast<AMyCharacter>(TryGetPawnOwner());
	if (MannyCharacter)
	{
		MannyCharacterMovement = MannyCharacter->GetCharacterMovement();
	}
}

void UMannyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(MannyCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MannyCharacterMovement->Velocity);
		bIsFalling = MannyCharacterMovement->IsFalling();
		CharacterState = MannyCharacter->GetCharacterState();
	}
}
