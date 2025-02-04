#include "TrailManager.h"
#include "Kismet/GameplayStatics.h"

UTrailManager::UTrailManager()
{
	ParticleSystem = nullptr;
	StartSocketName = "";
	EndSocketName = "";
	SpawnedParticle = nullptr;
}

UTrailManager::~UTrailManager()
{
	ParticleSystem = nullptr;
	StartSocketName = "";
	EndSocketName = "";
	SpawnedParticle = nullptr;
}

void UTrailManager::Initialize(UParticleSystem* InParticleSystem, FName InStartSocketName, FName InEndSocketName)
{
	ParticleSystem = InParticleSystem;
	StartSocketName = InStartSocketName;
	EndSocketName = InEndSocketName;
	SpawnedParticle = nullptr;
}

void UTrailManager::StartTrail(USceneComponent* MeshComp)
{
	if (!ParticleSystem || !MeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("TrailManager: Invalid ParticleSystem or MeshComp."));
		return;
	}

	// Проверяем, является ли компонент SkeletalMesh или StaticMesh
	if (USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(MeshComp))
	{
		// Создаём Particle System и привязываем к Skeletal Mesh
		SpawnedParticle = UGameplayStatics::SpawnEmitterAttached(
			ParticleSystem,
			SkeletalMeshComp,
			StartSocketName, // Имя первого сокета
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if (SpawnedParticle && EndSocketName != NAME_None)
		{
			SpawnedParticle->BeginTrails(StartSocketName, EndSocketName, ETrailWidthMode_FromCentre, 1.f);
		}
	}
	else if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(MeshComp))
	{
		// Создаём Particle System и привязываем к Static Mesh
		SpawnedParticle = UGameplayStatics::SpawnEmitterAttached(
			ParticleSystem,
			StaticMeshComp,
			StartSocketName, // Имя первого сокета
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if (SpawnedParticle && EndSocketName != NAME_None)
		{
			SpawnedParticle->BeginTrails(StartSocketName, EndSocketName, ETrailWidthMode_FromCentre, 1.f);
		}
	}
	else
	{
		// Если компонент неподдерживаемого типа
		ensureAlwaysMsgf(false, TEXT("TrailManager: Unsupported component type passed to StartTrail."));
	}
}

void UTrailManager::StopTrail()
{
	if (SpawnedParticle)
	{
		SpawnedParticle->EndTrails();
		SpawnedParticle = nullptr;
	}
}
