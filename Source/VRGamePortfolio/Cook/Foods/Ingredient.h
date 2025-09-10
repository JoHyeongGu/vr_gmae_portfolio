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

	void Slice(FVector Position, FVector Normal);
	virtual void InitComponents();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	FString MeshPath;

	UPROPERTY(VisibleAnywhere)
	FString SlicedMaterialPath;

	void CopyProceduralMeshFromStaticMesh(UProceduralMeshComponent* ProceduralMesh, UStaticMesh* StaticMesh);

private:
	UPROPERTY(VisibleAnywhere)
	AGrabSpawner* GrabSpawner;
};
