#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knife.generated.h"

class UBoxComponent;

UCLASS()
class VRGAMEPORTFOLIO_API AKnife : public AActor
{
	GENERATED_BODY()
	
public:	
	AKnife();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> KnifeMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> SliceCollision;

	void InitComponents();
	void BindCollisionFunction();

	UFUNCTION()
	void SliceCollisionStart(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void SliceCollisionEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
