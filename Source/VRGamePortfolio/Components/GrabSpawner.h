#pragma once

#include "CoreMinimal.h"
#include "GrabSpawner.generated.h"

class AActor;

UCLASS()
class VRGAMEPORTFOLIO_API AGrabSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttachGrabComponent(AActor* TargetActor);
	virtual void AttachGrabComponent_Implementation(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttachGrabComponentInAllGrabActors();
	virtual void AttachGrabComponentInAllGrabActors_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeleteGrabComponent(AActor* TargetActor);
	virtual void DeleteGrabComponent_Implementation(AActor* TargetActor);
};
