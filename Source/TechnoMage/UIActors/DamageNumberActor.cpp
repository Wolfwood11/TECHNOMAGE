#include "DamageNumberActor.h"
#include "Components/TextRenderComponent.h"

ADamageNumberActor::ADamageNumberActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Создаём компонент текста
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	RootComponent = TextRenderComponent;

	// Настраиваем текст
	TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TextRenderComponent->SetTextRenderColor(FColor::Red);
	TextRenderComponent->SetWorldSize(45.f); // Уменьшаем размер шрифта ещё в 2 раза
	TextRenderComponent->SetRelativeScale3D(FVector(0.25f)); // Уменьшаем масштаб текста
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("/Game/fonts/unlitdefault.unlitdefault"));
	if (TextMaterial.Succeeded())
	{
		TextRenderComponent->SetTextMaterial(TextMaterial.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load material for DamageNumberActor"));
	}

	// Настройки по умолчанию
	ScaleFactor = 0.2f;
	TargetScale = 1.0f;
	ScaleSpeed = 4.0f;
	HoverTime = 1.0f;
	bIsHovering = false;
}

void ADamageNumberActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADamageNumberActor::Initialize(float DamageAmount, const FTransform& transform, AActor* NewInstigatorActor, bool bIsCritical)
{
	ActivateFromPoll(transform, NewInstigatorActor);

	// Устанавливаем текст
	TextRenderComponent->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));

	// Устанавливаем цвет для критического/обычного удара
	TextRenderComponent->SetTextRenderColor(bIsCritical ? FColor::Yellow : FColor::Red);

	// Увеличиваем дальность и высоту полёта
	Velocity = FVector(FMath::FRandRange(-75.0f, 75.0f), FMath::FRandRange(-75.0f, 75.0f), FMath::FRandRange(200.0f, 300.0f));
	Acceleration = FVector(0.0f, 0.0f, -500.0f); // Увеличиваем ускорение для более выразительной параболы

	// Сбрасываем таймеры
	HoverTimer = 0.1f; // Сокращаем время зависания
	RemainingLifeTime = 0.35f;    // Уменьшаем общее время жизни
	bIsHovering = false;

	// Устанавливаем начальный масштаб
	SetActorScale3D(FVector(ScaleFactor));
}

void ADamageNumberActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsHovering)
	{
		// Обновляем позицию с учётом параболической траектории
		FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;
		Velocity += Acceleration * DeltaTime;
		SetActorLocation(NewLocation);

		// Обновляем масштаб текста
		ScaleFactor = FMath::FInterpTo(ScaleFactor, TargetScale, DeltaTime, ScaleSpeed);
		SetActorScale3D(FVector(ScaleFactor));

		// Проверяем, достигли ли точки зависания
		if (Velocity.Z < 0.0f && FVector::DotProduct(Velocity, Acceleration) > 0.0f)
		{
			bIsHovering = true;
		}
	}
	else
	{
		// Уменьшаем таймер зависания
		HoverTimer -= DeltaTime;
		if (HoverTimer <= 0.0f)
		{
			// Уничтожаем или возвращаем в пул
			ReturnToPool();
		}
	}

	// Поворачиваем текст к камере
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (PlayerController->PlayerCameraManager)
		{
			FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(CameraLocation - GetActorLocation()).Rotator();
			LookAtRotation.Roll = 0.0f;
			TextRenderComponent->SetWorldRotation(LookAtRotation);
		}
	}
}
