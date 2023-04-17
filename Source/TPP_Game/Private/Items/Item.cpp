// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"

#include "Characters/MyCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnComponentEndOverlap);

}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MannyCharacter = Cast<AMyCharacter>(OtherActor);
	if(MannyCharacter)
	{
		MannyCharacter->SetOverlapingItem(this);
	}
}

void AItem::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* MannyCharacter = Cast<AMyCharacter>(OtherActor);
	if(MannyCharacter)
	{
		MannyCharacter->SetOverlapingItem(nullptr);
	}	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ItemState == EItemState::EIS_Hovering)
	{
		RunningTime += DeltaTime;
		AddActorWorldOffset(FVector(0, 0, TransformedSin()));
	}
}

