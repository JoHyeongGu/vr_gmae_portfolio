#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.generated.h"

class AGrabSpawner;
class UProceduralMeshComponent;

UCLASS()
class VRGAMEPORTFOLIO_API AIngredient : public AActor
{
	GENERATED_BODY()
	
public:
	AIngredient();
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> ProceMesh;

	UPROPERTY(VisibleAnywhere)
	UMaterial* SlicedMaterial;

	void SetProceMeshAttr();
	void Slice(FVector Position, FVector Normal);

protected:
	UPROPERTY(VisibleAnywhere)
	FString MeshPath;

	UPROPERTY(VisibleAnywhere)
	FString SlicedMaterialPath;

	virtual void BeginPlay() override;
	virtual void InitMesh();

private:
	UPROPERTY(VisibleAnywhere)
	AGrabSpawner* GrabSpawner;

	void InitComponents();
};
