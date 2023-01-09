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
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerSciFiAnimation.h"


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
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->CastShadow = false;
	GetMesh()->bCastDynamicShadow = false;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Gun = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("GunMesh"));
	Gun->SetOwnerNoSee(true);
	Gun->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	Gun->AttachWeapon(this);
	Gun->SetNetAddressable();
	Gun->SetIsReplicated(true);
	Gun1P = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("GunMesh1P"));
	Gun1P->SetOnlyOwnerSee(true);
	Gun1P->AttachToComponent(Mesh1P, AttachmentRules, FName(TEXT("GripPoint")));
	Gun1P->AttachWeapon(this);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	SetHasRifle(true);

	SmoothCrouchingCurveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineFront"));
	SmoothCrouchInterpFunction.BindUFunction(this, FName("SmoothCrouchInterpReturn"));
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
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;
	SmoothCrouchTimelineSetting();
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

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::ServerRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AFaceTheSunCharacter::ServerStopRun);
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

void AFaceTheSunCharacter::MulticastRun_Implementation()
{
	bIsRun = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	GetCharacterMovement()->MaxSwimSpeed = 200.0f;
}

void AFaceTheSunCharacter::MulticastStopRun_Implementation()
{
	bIsRun = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;
	GetCharacterMovement()->MaxSwimSpeed = 100.0f;
}
void AFaceTheSunCharacter::ServerRun_Implementation()
{
	MulticastRun();
}
void AFaceTheSunCharacter::ServerStopRun_Implementation()
{
	MulticastStopRun();
}

void AFaceTheSunCharacter::StartCrouch()
{
	SmoothCrouchingCurveTimeline->Play();
	ServerCrouch();
}

void AFaceTheSunCharacter::StopCrouch()
{
	SmoothCrouchingCurveTimeline->Reverse();
	ServerStopCrouch();
}

void AFaceTheSunCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AFaceTheSunCharacter::GetHasRifle()
{
	return bHasRifle;
}


void AFaceTheSunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFaceTheSunCharacter::Fire()
{
	// 추후 3인칭 함수는 RPC로 호출하도록 
	if (bIsShot)
	{
		ServerFire();
		GetWorld()->GetTimerManager().SetTimer(CharacterTimer, this, &AFaceTheSunCharacter::Fire, Gun->GetFireSpeed(), false);
	}
}

void AFaceTheSunCharacter::StopFire()
{
	bIsShot = false;
	GetWorldTimerManager().ClearTimer(CharacterTimer);
}

void AFaceTheSunCharacter::StartFire()
{
	bIsShot = true;
	Fire();
}

void AFaceTheSunCharacter::ServerFire_Implementation()
{
	MulticastFire();
	ClientFire();
}

void AFaceTheSunCharacter::MulticastFire_Implementation()
{
	//만약 해당 캐릭터를 소유한사람 즉, 총발사자한테는 2개의 파티클 이펙트와 2개의 총알이 생성될 필요가 없음. 직접 조종하지 않는 사람들이 볼 화면
	Gun->TPFire();
}

void AFaceTheSunCharacter::ClientFire_Implementation()
{
	//직접 조종하는 사람만 볼 화면
	Gun1P->P1Fire();
}

void AFaceTheSunCharacter::SmoothCrouchInterpReturn(float Value)
{
	// 반으로 줄인다. 근데 Collision 뿐만아니라 카메라도 같이 움직이고 Collision은 위치 이동이 안되기 때문에 Mesh도 같이 이동해준다.
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(90.f, 45.f, Value));
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.0f, 3.f, (FMath::Lerp(60.0f, 30.0f, Value))));
	GetMesh()->SetRelativeLocation(FVector(-17.f, -4.f, (FMath::Lerp(-92.f, -45.f, Value))));
}

void AFaceTheSunCharacter::SmoothCrouchTimelineSetting()
{
	if (SmoothCrouchingCurveFloat)
	{
		SmoothCrouchingCurveTimeline->AddInterpFloat(SmoothCrouchingCurveFloat, SmoothCrouchInterpFunction);
		SmoothCrouchingCurveTimeline->SetLooping(false);
	}
}

void AFaceTheSunCharacter::MulticastCrouch_Implementation()
{
	auto PlayerSciAnim = Cast<UPlayerSciFiAnimation>(GetMesh()->GetAnimInstance());
	PlayerSciAnim->bIsCrouch = true;
}
void AFaceTheSunCharacter::ServerCrouch_Implementation()
{
	MulticastCrouch();
}
void AFaceTheSunCharacter::MulticastStopCrouch_Implementation()
{
	auto PlayerSciAnim = Cast<UPlayerSciFiAnimation>(GetMesh()->GetAnimInstance());
	PlayerSciAnim->bIsCrouch = false;
}
void AFaceTheSunCharacter::ServerStopCrouch_Implementation()
{
	MulticastStopCrouch();
}

void AFaceTheSunCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFaceTheSunCharacter, bIsShot);
}