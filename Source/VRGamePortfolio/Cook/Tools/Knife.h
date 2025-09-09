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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
