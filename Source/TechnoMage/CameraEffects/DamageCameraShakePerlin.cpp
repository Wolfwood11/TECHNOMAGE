#include "DamageCameraShakePerlin.h"
#include "Shakes//PerlinNoiseCameraShakePattern.h"
#include "Camera/CameraShakeBase.h"

UDamageCameraShakePerlin::UDamageCameraShakePerlin() : UCameraShakeBase(FObjectInitializer::Get())
{
	bSingleInstance = true;
	UPerlinNoiseCameraShakePattern* ShakePattern = NewObject<UPerlinNoiseCameraShakePattern>();

	if (ShakePattern)
	{
		ShakePattern->Duration = 0.6f;  // Длительность хаотичной тряски
		ShakePattern->BlendInTime = 0.1f;
		ShakePattern->BlendOutTime = 0.2f;

		// Добавляем хаотичные смещения
		ShakePattern->LocationAmplitudeMultiplier = 2.0f;
		ShakePattern->LocationFrequencyMultiplier = 1.5f;

		// Умеренные повороты камеры (Yaw)
		ShakePattern->RotationAmplitudeMultiplier = 1.0f;
		ShakePattern->RotationFrequencyMultiplier = 2.0f;

		SetRootShakePattern(ShakePattern);
	}
}
