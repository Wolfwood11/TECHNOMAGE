#include "BasePlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TechnoMage/Components/SpellCaster.h"
#include "TechnoMage/Components/DashComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Serialization/BufferArchive.h"
#include "TechnoMage/Components/CharacterResourcePool.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "TechnoMage/Components/LevelingComponent.h"
#include "TechnoMage/Subsystems/SaveSubsystem.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // Set rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f)); // Fixed top-down angle
	CameraBoom->TargetArmLength = 900.0f; // Adjust to set the camera distance
	CameraBoom->bDoCollisionTest = false; // Disable collision
	CameraBoom->bUsePawnControlRotation = false; // Prevent camera rotation with the pawn
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to the arm
	FollowCamera->FieldOfView = 90.0f; // Adjust field of view

	ManaPool = CreateDefaultSubobject<UCharacterResourcePool>(TEXT("ManaPool"));
	if (ManaPool)
	{
		ManaPool->SetModifierTypeAffects(EModifierType::Mana);
	}
	SpellCaster = CreateDefaultSubobject<USpellCaster>(TEXT("SpellCaster"));
	Dash = CreateDefaultSubobject<UDashComponent>(TEXT("Dash"));

	Leveling = CreateDefaultSubobject<ULevelingComponent>(TEXT("Leveling"));
}

void ABasePlayerCharacter::SaveData_Implementation(TArray<uint8>& OutData)
{
	FBufferArchive Archive;
	OutData = static_cast<TArray<unsigned char>>(Archive);
}

void ABasePlayerCharacter::LoadData_Implementation(const TArray<uint8>& InData)
{
	FMemoryReader Reader(InData);

	// Создаём мапу для быстрого поиска компонентов
	TMap<FName, UActorComponent*> ComponentMap;
	for (UActorComponent* Component : GetComponents())
	{
		if (Component)
		{
			ComponentMap.Add(Component->GetFName(), Component);
		}
	}

	// Загружаем данные для компонентов
	while (Reader.Tell() < Reader.TotalSize())
	{
		// Загружаем имя компонента
		FName ComponentName;
		Reader << ComponentName;

		// Загружаем размер данных компонента
		int32 DataSize;
		Reader << DataSize;

		// Загружаем данные компонента
		TArray<uint8> ComponentData;
		ComponentData.SetNum(DataSize);
		Reader.Serialize(ComponentData.GetData(), DataSize);

		// Находим компонент в мапе
		if (UActorComponent** FoundComponent = ComponentMap.Find(ComponentName))
		{
			if ((*FoundComponent)->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
			{
				ISaveableInterface::Execute_LoadData(*FoundComponent, ComponentData);
			}
		}
	}
}

void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SpellCaster && ManaPool)
	{
		SpellCaster->SetManaPool(ManaPool);
	}

	if (Dash && ManaPool)
	{
		Dash->SetManaPool(ManaPool);
	}

	if (USaveSubsystem* SaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		SaveSubsystem->LoadActorData(this, "Player");
	}
}

void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::ExecuteDash);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Move);

		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Fire);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void ABasePlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddMovementInput(FVector::ForwardVector, Input.Y);
	AddMovementInput(FVector::RightVector, Input.X);
}

void ABasePlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void ABasePlayerCharacter::Fire(const FInputActionValue& Value)
{
	if (SpellCaster)
	{
		SpellCaster->CastNextSpell();
	}
}

void ABasePlayerCharacter::ExecuteDash()
{
	if (Dash)
	{
		Dash->PerformDash();
	}
}

void ABasePlayerCharacter::AddExp_Implementation(int exp)
{
	Leveling->AddExperience(exp);
	if (USaveSubsystem* SaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		SaveSubsystem->SaveActorData(this, "Player");
	}
}

int32 ABasePlayerCharacter::GetLevel_Implementation() const
{
	return 0;
}

int32 ABasePlayerCharacter::GetExp_Implementation() const
{
	return 0;
}

int32 ABasePlayerCharacter::GetExpToNextLevel_Implementation() const
{
	return 0;
}

float ABasePlayerCharacter::GetMana_Implementation() const
{
	if (ManaPool)
	{
		return ManaPool->GetCurrentResource();
	}
	return 0;
}

float ABasePlayerCharacter::GetMaxMana_Implementation() const
{
	if (ManaPool)
	{
		return ManaPool->GetMaxResource();
	}
	return 1;
}
