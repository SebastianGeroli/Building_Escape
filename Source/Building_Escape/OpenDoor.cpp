// Copyright Carlos Sebastián Diaz Geroli 2020
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.h"
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialAngle = GetOwner()->GetActorRotation().Yaw;
	CurrentAngle = InitialAngle;
	OpenAngle += InitialAngle;
	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() > DoorLastOpen + DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}
float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor *> OverlapingActors;
	if (!PressurePlate)
		return TotalMass;
	PressurePlate->GetOverlappingActors(OUT OverlapingActors);
	if (OverlapingActors.Num() == 0)
		return TotalMass;
	for (AActor *Actor : OverlapingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentAngle = FMath::Lerp(CurrentAngle, OpenAngle, DoorOpenSpeed * DeltaTime);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentAngle;
	GetOwner()->SetActorRotation(DoorRotation);
	if (!AudioComponent)
		return;
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = false;
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentAngle = FMath::Lerp(CurrentAngle, InitialAngle, DoorCloseSpeed * 3.f * DeltaTime);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentAngle;
	GetOwner()->SetActorRotation(DoorRotation);
	if (!AudioComponent)
		return;
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
		OpenDoorSound = false;
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The object %s does NOT have the AudioComponent"), *GetOwner()->GetName());
	}
}
void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The object %s does NOT have a ATriggerVolume set as Pressure Plate"), *GetOwner()->GetName());
	}
}