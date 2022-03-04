// TODO: Add copyright term notes

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class CASTLEESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	

	UOpenDoor();							// Sets default values for this component's properties	
	

protected:

	virtual void BeginPlay() override;		// Called when the game starts

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		// Called every frame
	float GetWeightMeasured();
	 
	UPROPERTY(BlueprintAssignable)	FDoorEvent OnOpen;
	UPROPERTY(BlueprintAssignable)	FDoorEvent OnClose;

private:

	UPROPERTY(EditAnywhere)		ATriggerVolume*	PressurePlate = nullptr;
	UPROPERTY(EditAnywhere)		float	TriggerMass = 30;
	AActor* Owner = nullptr;					// Owning door

};
