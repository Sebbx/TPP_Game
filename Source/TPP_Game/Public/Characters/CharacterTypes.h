#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Uneqipped"),
	ECS_EquippedOneHanded UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHanded UMETA(DisplayName = "Equipped Two-Handed Weapon")
};