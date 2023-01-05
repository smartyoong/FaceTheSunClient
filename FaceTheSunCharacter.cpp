// Copyright Epic Games, Inc. All Rights Reserved.

#include "FaceTheSunCharacter.h"
#include "FaceTheSunProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_WeaponComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// AFaceTheSunCharacter

AFaceTheSunCharacter::AFaceTheSunCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 12.f;
	GetMesh()->SetOwnerNoSee(true);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Gun = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("GunMesh"));
	Gun->SetOwnerNoSee(true);
	Gun->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	Gun->AttachWeapon(this);
	Gun1P = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("GunMesh1P"));
	Gun1P->SetOnlyOwnerSee(true);
	Gun1P->AttachToComponent(Mesh1P, AttachmentRules, FName(TEXT("GripPoint")));
	Gun1P->AttachWeapon(this);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	SetHasRifle(true);

	FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Sound"));
	FireSound->bAutoActivate = false;
	FireSound->SetupAttachment(RootComponent);
}

void AFaceTheSunCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AFaceTheSunCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::Look);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFaceTheSunCharacter::StopCrouch);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AFaceTheSunCharacter::StopRun);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AFaceTheSunCharacter::StopFire);
	}
}


void AFaceTheSunCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFaceTheSunCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFaceTheSunCharacter::Run(const FInputActionValue& Value)
{
	bIsFire = false;
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 500.0f;
	GetCharacterMovement()->MaxSwimSpeed = 200.0f;
}

void AFaceTheSunCharacter::StopRun(const FInputActionValue& Value)
{
	bIsFire = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;
	GetCharacterMovement()->MaxSwimSpeed = 100.0f;
}

void AFaceTheSunCharacter::StartCrouch(const FInputActionValue& Value)
{
	Crouch();
}

void AFaceTheSunCharacter::StopCrouch(const FInputActionValue& Value)
{
	UnCrouch();
}

void AFaceTheSunCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AFaceTheSunCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AFaceTheSunCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaleHalfHeightAdjust)
{
	/*������ �ɱ�� �׳� ī�޸� �����̵��ѰŶ�� �� �ɱ� ������ ���������� ���������� ���� */
	if (HalfHeightAdjust == 0.f)
	{
		return;
	}
	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaleHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight + HalfHeightAdjust;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight),false);
}
void AFaceTheSunCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaleHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f)
	{
		return;
	}
	Super::OnEndCrouch(HalfHeightAdjust, ScaleHalfHeightAdjust);
}
void AFaceTheSunCharacter::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	if (FirstPersonCameraComponent)
	{
		FirstPersonCameraComponent->GetCameraView(DeltaTime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

void AFaceTheSunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// �ɱ� ������ ���� ���ʸ��� ��ġ ���
	float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.f, CrouchInterpTime) * CrouchEyeOffset;
}

void AFaceTheSunCharacter::Fire()
{
	// ���� 3��Ī �Լ��� RPC�� ȣ���ϵ��� �����Ұ�
	if (bIsFire)
	{
		Gun->Fire(GetMesh());
		Gun1P->Fire(GetMesh1P());
		//������Ÿ�� ���ٰ�
		GetWorld()->GetTimerManager().SetTimer(CharacterTimer, this, &AFaceTheSunCharacter::Fire, 1.25f, false);
	}
}

void AFaceTheSunCharacter::StopFire()
{
	bIsFire = false;
	FireSound->Stop();
}

void AFaceTheSunCharacter::StartFire()
{
	bIsFire = true;
	FireSound = UGameplayStatics::SpawnSoundAttached(FireSoundCue,Gun1P);
	Fire();
}