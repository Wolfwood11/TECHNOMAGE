#include "DamageCameraShake.h"
#include "Shakes//WaveOscillatorCameraShakePattern.h"
#include "Camera/CameraShakeBase.h"

UDamageCameraShake::UDamageCameraShake() : UCameraShakeBase(FObjectInitializer::Get())
{
	bSingleInstance = true;
	UWaveOscillatorCameraShakePattern* ShakePattern = NewObject<UWaveOscillatorCameraShakePattern>();

	if (ShakePattern)
	{
		ShakePattern->Duration = 0.4f;  // Длительность эффекта
		ShakePattern->BlendInTime = 0.1f;
		ShakePattern->BlendOutTime = 0.1f;

		// Минимальное вращение (убираем Pitch/Roll)
		ShakePattern->RotationAmplitudeMultiplier = 0.5f;
		ShakePattern->RotationFrequencyMultiplier = 1.5f;

		// Лёгкое горизонтальное смещение (X/Y) без дерганий вверх
		ShakePattern->LocationAmplitudeMultiplier = 1.0f;
		ShakePattern->LocationFrequencyMultiplier = 2.0f;

		SetRootShakePattern(ShakePattern);
	}
}
