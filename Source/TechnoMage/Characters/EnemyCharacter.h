#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class TECHNOMAGE_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exp", meta = (AllowPrivateAccess = "true"))
	int Exp = 10;

	virtual void Die() override;

protected:
	virtual void BeginPlay() override;

	void ConfigureMovement() const;

	virtual void Tick(float DeltaTime) override;
};
