#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "TrailManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TECHNOMAGE_API UTrailManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UTrailManager();
	~UTrailManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
	TObjectPtr<UParticleSystem> ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
	FName StartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
	FName EndSocketName;

private:
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> SpawnedParticle;

public:
	void Initialize(UParticleSystem* InParticleSystem, FName InStartSocketName, FName InEndSocketName);
	void StartTrail(USceneComponent* MeshComp);
	void StopTrail();
};
