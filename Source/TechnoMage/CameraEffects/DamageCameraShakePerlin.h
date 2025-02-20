#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "DamageCameraShakePerlin.generated.h"

UCLASS()
class TECHNOMAGE_API UDamageCameraShakePerlin : public UCameraShakeBase
{
	GENERATED_BODY()

public:
	UDamageCameraShakePerlin();
};
