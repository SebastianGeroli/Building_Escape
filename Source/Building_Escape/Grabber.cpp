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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the physics handle is attach
	if (!PhysicsHandle)
			return;
	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

void UGrabber::Grab()
{
	//Try and reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicBodyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	//If we  hit something then attach the physics handle
	AActor *ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		if (!PhysicsHandle)
			return;
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach());
	}

	//TODO attach physics handle
}

void UGrabber::ReleaseGrab()
{
	if (!PhysicsHandle)
			return;
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseGrab);
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("The Physics handle component was not found and is required! at object: %s"), *GetOwner()->GetName());
	}
}

FVector UGrabber::GetPlayerWorldPos() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	return PlayerViewpointLocation;
}

FVector UGrabber::GetPlayersReach() const
{
	//Get Player Viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	//Calculate the end of the ray
	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}

FHitResult UGrabber::GetFirstPhysicBodyInReach() const
{

	//Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);
	return Hit;
}
