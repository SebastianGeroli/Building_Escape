// Copyright Carlos Sebastián Diaz Geroli 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

#define OUT
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindAudioComponent();
	void FindPressurePlate();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	float TotalMassOfActors()const;
	//Tracks if sounds have been played
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
private:
	float CurrentAngle;
	float InitialAngle;
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = .8f;
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.4f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate = nullptr;
	float DoorLastOpen = 0.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = .5f;
	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 25.f;
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
