#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "TrailManager.generated.h"

UCLASS()
class TECHNOMAGE_API UTrailManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
	UParticleSystem* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
	FName StartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
	FName EndSocketName;

private:
	UParticleSystemComponent* SpawnedParticle;

public:
	void Initialize(UParticleSystem* InParticleSystem, FName InStartSocketName, FName InEndSocketName);
	void StartTrail(USceneComponent* MeshComp);
	void StopTrail();
};
