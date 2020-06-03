// Copyright Carlos Sebastián Diaz Geroli 2020

#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	//Check for the physics handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle){

	}else{
		UE_LOG(LogTemp,Error,TEXT("The Physics handle component was not found and is required! at object: %s"),*GetOwner()->GetName());
	}
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		UE_LOG(LogTemp,Warning,TEXT("The Input component was found at object: %s"),*GetOwner()->GetName());
		InputComponent->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
	}else{
		UE_LOG(LogTemp,Error,TEXT("The Input component MISSING at object: %s"),*GetOwner()->GetName());
	}
}
void UGrabber::Grab(){
	UE_LOG(LogTemp,Warning,TEXT("Grab was press"));
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	//Get Player Viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	//Loggin out to test
	//UE_LOG(LogTemp,Warning,TEXT("Player viewpoint location: %s"),*PlayerViewpointLocation.ToString());
	//UE_LOG(LogTemp,Warning,TEXT("Player viewpoint rotation: %s"),*PlayerViewpointRotation.ToString());
	//Draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
	//Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("Actors name: %s"), *ActorHit->GetName());
	}
	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5
	);
	//See what it hits
}
