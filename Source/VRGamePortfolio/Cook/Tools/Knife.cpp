#include "Kismet/GameplayStatics.h"
#include "VRGamePortfolio/Cook/Tools/Knife.h"
#include "VRGamePortfolio/Components/GrabSpawner.h"

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
	KnifeMesh->SetCollisionProfileName("BlockAll");
	KnifeMesh->SetSimulatePhysics(true);
	KnifeMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = KnifeMesh;

	// Add Grab Component
	AGrabSpawner* GrabSpawner = (AGrabSpawner*)UGameplayStatics::GetActorOfClass(GetWorld(), AGrabSpawner::StaticClass());
	if (GrabSpawner) GrabSpawner->AttachGrabComponent(this);
}

