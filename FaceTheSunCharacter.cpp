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
#include "Components/SpotLightComponent.h"
#include "FaceTheSunGameState.h"
#include "FaceTheSunInstance.h"
#include "GameFramework/PlayerController.h"
#include "FaceTheSunPlayerController.h"
#include "FaceTheSunPlayerState.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "BossKrakenHP.h"
#include "BossKraken.h"

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
	FirstPersonCameraComponent->SetIsReplicated(true);

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

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(FirstPersonCameraComponent);
	FlashLight->SetVisibility(true);
	FlashLight->SetIsReplicated(true);
	PlayerSciAnim = Cast<UPlayerSciFiAnimation>(GetMesh()->GetAnimInstance());
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
	TeamId = FGenericTeamId(ID);
	MyGameState = Cast<AFaceTheSunGameState>(GetWorld()->GetGameState());
	if (MyGameState)
	{
		MyGameState->AddPlayerToList(this);
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

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::ServerRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AFaceTheSunCharacter::ServerStopRun);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AFaceTheSunCharacter::StopFire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::Reloading);
		EnhancedInputComponent->BindAction(LightAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::ToggleLight);
		EnhancedInputComponent->BindAction(NextCameraAction, ETriggerEvent::Triggered, this, &AFaceTheSunCharacter::SpectateNextPlayer);
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
	bCIsCrouch = !bCIsCrouch;
	GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = true;
	if (!bCIsCrouch)
	{
		ServerStopCrouch();
	}
	else
	{
		ServerCrouch();
	}
}


void AFaceTheSunCharacter::Reloading()
{
	if(!bIsReloadingNow)
		ServerReloading();
}

void AFaceTheSunCharacter::MulticastReloading_Implementation()
{
	Gun->Reloading();
}

void AFaceTheSunCharacter::ServerReloading_Implementation()
{
	bIsReloadingNow = true;
	MulticastReloading();
	ClientReloading();
}

void AFaceTheSunCharacter::ClientReloading_Implementation()
{
	Gun1P->Reloading1P();
}

void AFaceTheSunCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AFaceTheSunCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AFaceTheSunCharacter::CallWinFunc_Implementation()
{
	auto PC = Cast<AFaceTheSunPlayerController>(GetController());
	if (PC)
	{
		PC->GoToWinMenu();
	}
}
void AFaceTheSunCharacter::CallDefeatFunc_Implementation()
{
	auto PC = Cast<AFaceTheSunPlayerController>(GetController());
	if (PC)
		PC->GoToDefeatMenu();
}


void AFaceTheSunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		if (!IsDead)
		{
			if (FlashLight)
				FlashLight->SetWorldRotation(GetControlRotation());
			if (FirstPersonCameraComponent)
				FirstPersonCameraComponent->SetWorldRotation(GetControlRotation());
		}
		// we need replicate the spotlight rotation
	}
	if (bCIsCrouch)
		GetMesh()->SetRelativeLocation(FVector(-17.f, -4.f, -45.f));

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if(GetWorld()->LineTraceSingleByChannel(Hit,GetActorLocation(),GetActorLocation()+GetActorForwardVector()*2000.f,ECollisionChannel::ECC_Visibility,Params))
	{
		ABossKraken* Boss = Cast<ABossKraken>(Hit.GetActor());
		if(Boss)
		{
			auto PC = Cast<AFaceTheSunPlayerController>(GetController());
			if (PC)
				PC->ShowBossHP();
		}
	}	
}

void AFaceTheSunCharacter::Fire()
{
	// make automatic fire
	if (bIsShot && !bIsReloadingNow && !IsDead)
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
	//for another client fire
	Gun->TPFire();
}

void AFaceTheSunCharacter::ClientFire_Implementation()
{
	//for client fire
	Gun1P->P1Fire();
}

void AFaceTheSunCharacter::SmoothCrouchInterpReturn(float Value)
{
	// smooth crouch
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(90.f, 45.f, Value));
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.0f, 3.f, (FMath::Lerp(60.0f, 30.0f, Value))));
	GetMesh()->SetRelativeLocation(FVector(-17.f, -4.f, (FMath::Lerp(-90.f, -45.f, Value))));
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
	auto MeshAnimInstance = Cast<UPlayerSciFiAnimation>(GetMesh()->GetAnimInstance());
	check(MeshAnimInstance);
	SmoothCrouchingCurveTimeline->Play();
	MeshAnimInstance->bIsCrouch = true;
}
void AFaceTheSunCharacter::ServerCrouch_Implementation()
{
	MulticastCrouch();
}
void AFaceTheSunCharacter::MulticastStopCrouch_Implementation()
{
	auto MeshAnimInstance = Cast<UPlayerSciFiAnimation>(GetMesh()->GetAnimInstance());
	check(MeshAnimInstance);
	SmoothCrouchingCurveTimeline->Reverse();
	MeshAnimInstance->bIsCrouch = false;
}
void AFaceTheSunCharacter::ServerStopCrouch_Implementation()
{
	MulticastStopCrouch();
}

void AFaceTheSunCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFaceTheSunCharacter, bIsShot);
	DOREPLIFETIME(AFaceTheSunCharacter, bIsReloadingNow);
	DOREPLIFETIME(AFaceTheSunCharacter, bIsLightOn);
	DOREPLIFETIME(AFaceTheSunCharacter, IsDead);
	DOREPLIFETIME(AFaceTheSunCharacter, HP);
}

void AFaceTheSunCharacter::ToggleLight()
{
	ServerLight();
}

void AFaceTheSunCharacter::SpectateNextPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("NextPlayer"));
	if (IsDead)
	{
		PlayerCameraIndex++;
		auto CurrGameState = Cast<AFaceTheSunGameState>(GetWorld()->GetGameState());
		if (CurrGameState)
		{
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(CurrGameState->GetPlayerList()[PlayerCameraIndex % CurrGameState->GetPlayerList().Num()]);
		}
	}
}

void AFaceTheSunCharacter::MulticastLight_Implementation()
{
	FlashLight->SetVisibility(bIsLightOn);
}
void AFaceTheSunCharacter::ServerLight_Implementation()
{
	bIsLightOn = !bIsLightOn;
	MulticastLight();
}

float AFaceTheSunCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	HP -= Damage;
	if (HP <= 0)
	{
		HP = 0;
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
		IsDead = true;
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		FlashLight->SetActive(false);
		auto PS = Cast<AFaceTheSunPlayerState>(GetPlayerState());
		if (PS)
		{
			PS->IsDead = true;
		}
		ServerOnDeath();
	}
	else
	{
		ServerOnHit();
	}
	return Damage;
}

void AFaceTheSunCharacter::ServerOnDeath_Implementation()
{
	MultiOnDeath();
}
void AFaceTheSunCharacter::MultiOnDeath_Implementation()
{
	if(DeathSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation(), 0.3f);
	auto PAI = Cast<UPlayerSciFiAnimation>(GetMesh()->GetAnimInstance());
	if (PAI)
		PAI->bIsDeath = true;
	PAI = Cast<UPlayerSciFiAnimation>(GetMesh1P()->GetAnimInstance());
	if (PAI)
		PAI->bIsDeath = true;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-300, -3.f, 90.f));
	FirstPersonCameraComponent->SetRelativeRotation(FQuat(0.f, -20.f, 0.f, 1.f));
}

void AFaceTheSunCharacter::ServerOnHit_Implementation()
{
		MultiOnHit();
}
void AFaceTheSunCharacter::MultiOnHit_Implementation()
{
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation(), GetActorRotation(), 0.3f);
	if (HitAnimation)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitAnimation);
	}
}

void AFaceTheSunCharacter::MultiHeal_Implementation()
{
	if (HealSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HealSound, GetActorLocation(), GetActorRotation(), 0.3f);
	if (HealParticleEffect)
	{
		pe = UGameplayStatics::SpawnEmitterAttached(HealParticleEffect, GetMesh());
	}
}
void AFaceTheSunCharacter::ServerHeal_Implementation()
{
	MultiHeal();
}
void AFaceTheSunCharacter::MultiAmmo_Implementation()
{
	if (RechargeAmmoSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RechargeAmmoSound, GetActorLocation(), GetActorRotation(), 0.3f);
	if (AmmoParticleEffect)
	{
		pe = UGameplayStatics::SpawnEmitterAttached(AmmoParticleEffect, GetMesh());
	}
}
void AFaceTheSunCharacter::ServerAmmo_Implementation()
{
	MultiAmmo();
}

void AFaceTheSunCharacter::FellOutOfWorld(const UDamageType& DmgType)
{
	auto PS = Cast<AFaceTheSunPlayerState>(GetPlayerState());
		if (PS)
		{
			PS->IsDead = true;
		}
	MultiOnDeath();
	Super::FellOutOfWorld(DmgType);
}