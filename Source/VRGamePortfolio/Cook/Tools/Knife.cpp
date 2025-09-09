#include "VRGamePortfolio/Cook/Tools/Knife.h"

AKnife::AKnife()
{
	PrimaryActorTick.bCanEverTick = true;
	InitComponents();

}

void AKnife::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKnife::InitComponents()
{
	// Load Assets
	const FString KnifeMeshPath = "/Game/Meshes/Prototype/PSM_Knife.PSM_Knife";
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KnifeMeshRef(*KnifeMeshPath);

	// Set Component
	KnifeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KnifeMesh"));
	KnifeMesh->SetStaticMesh(KnifeMeshRef.Object);
	RootComponent = KnifeMesh;
}

