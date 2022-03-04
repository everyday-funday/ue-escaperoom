// TODO: Add copyright term notes

#include "Grabber.h"
#include "Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


/// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();

}

/// 
void UGrabber::FindPhysicsHandleComponent()
{
	///Look for attached physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	///Warning when handle is not attached
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s: Handle not detected "), *(GetOwner()->GetName()));
	}
}

/// Look for the attached input component (appears only at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!InputComponent) { return; }
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Input component not found"), *(GetOwner()->GetName()));
	}
}

/// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("%s: Handle not detected "), *(GetOwner()->GetName()));
		return; 
	}
	///If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		///Move the object being held.
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab()
{

	/// Line trace and see if reach any actors with physics body collision channel set.
	auto HitResult = GetFirstPhysicsBodyInReach(); // auto for type
	auto ComponentToGrab = HitResult.GetComponent();  //get mesh
	auto ActorHit = HitResult.GetActor();

	UE_LOG(LogTemp, Warning, TEXT("%s: Handle not detecte121d "), *(GetOwner()->GetName()));


	/// If we hit something then attach a physics handle
	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// DEBUG LINE: ray-cast out to reach distance
	//DrawDebugLine(
	//	GetWorld(),
	//	ViewLocation,
	//	LineTraceEnd,
	//	FColor(255, 0, 0),
	//	false,
	//	0.f,
	//	0.f,
	//	10.f
	//);

	/// Setup collision query params (simple trace)
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), true, GetOwner());

	/// line-trace (aka ray-cast) out to reach distance and return the object that it hits
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}


FVector UGrabber::GetReachLineStart() {
	FVector ViewLocation;
	FRotator ViewRotation;
	GetWorld() ->GetFirstPlayerController() ->GetPlayerViewPoint(
		OUT ViewLocation, 
		OUT ViewRotation
	);

	return ViewLocation;
}

FVector UGrabber::GetReachLineEnd(){

	FVector ViewLocation;
	FRotator ViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT ViewLocation, 
		OUT ViewRotation
	);

	// Get player view point this tick
	FVector LineTraceEnd = FVector(0.f, 0.f, 0.f) + ViewLocation + (ViewRotation.Vector() * VectorReach);

	return LineTraceEnd;
}