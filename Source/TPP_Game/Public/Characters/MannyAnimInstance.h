// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MannyAnimInstance.generated.h"

class UCharacterMovementComponent;
class AMyCharacter;
UCLASS()
class TPP_GAME_API UMannyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	AMyCharacter* MannyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* MannyCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsFalling;
};
