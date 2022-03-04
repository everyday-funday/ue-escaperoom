// TODO: Add copyright term notes

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"

#define OUT

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
	Owner = GetOwner();
	if (!Owner) {
		UE_LOG(LogTemp, Warning, TEXT("%s: Owner not detected 1"), *(GetOwner()->GetName()));
		return;  }
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Poll the Trigger Volume every frame. Check whether the target actor overlaps with the trigger volume.
	if (GetWeightMeasured() > TriggerMass) // TODO: Make it on editab
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}

}


float UOpenDoor::GetWeightMeasured() {

	if (!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("%s: PressurePlate not detected "), *(GetOwner()->GetName()));
		return 0;
	}

	//find overlapping actors
	TArray<AActor*> OverlappingActors;
	float TotalMass = 0.f;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{

		UE_LOG(LogTemp, Warning, TEXT("%s Detects actor"), *Actor->GetName());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("Total Mass: %10.2f"), TotalMass);
	}

	//return sum weight
	return TotalMass;
}
