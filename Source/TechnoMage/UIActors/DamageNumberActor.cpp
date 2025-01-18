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
	TextRenderComponent->SetWorldSize(50.0f); // Размер шрифта
	TextRenderComponent->SetRelativeScale3D(FVector(1.0f)); // Масштаб текста
}

void ADamageNumberActor::BeginPlay()
{
	Super::BeginPlay();

	RemainingLifeTime = LifeSpan;
}

void ADamageNumberActor::Initialize(float DamageAmount)
{
	// Устанавливаем текст
	TextRenderComponent->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));
}

void ADamageNumberActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Поднимаем текст вверх
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += RiseSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	// Уменьшаем оставшееся время жизни
	RemainingLifeTime -= DeltaTime;
	if (RemainingLifeTime <= 0.0f)
	{
		Destroy();
	}
}
