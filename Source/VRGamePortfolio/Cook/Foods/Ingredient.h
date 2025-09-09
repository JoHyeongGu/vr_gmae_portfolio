#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.generated.h"

class UProceduralMeshComponent;

UCLASS()
class VRGAMEPORTFOLIO_API AIngredient : public AActor
{
	GENERATED_BODY()
	
public:	
	AIngredient();

protected:
	UPROPERTY(VisibleAnywhere)
	FString MeshPath;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> ProceMesh;

	UPROPERTY(VisibleAnywhere)
	UMaterial* SlicedMaterial;

	virtual void InitComponents();
	void CopyProceduralMeshFromStaticMesh(UProceduralMeshComponent* ProceduralMesh, UStaticMesh* StaticMesh);
	void Slice(FVector Position, FVector Normal);
};
