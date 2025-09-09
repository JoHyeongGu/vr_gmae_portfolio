#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knife.generated.h"

UCLASS()
class VRGAMEPORTFOLIO_API AKnife : public AActor
{
	GENERATED_BODY()
	
public:	
	AKnife();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> KnifeMesh;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:	
	void InitComponents();
};
