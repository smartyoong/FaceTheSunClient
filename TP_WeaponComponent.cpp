// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FaceTheSunCharacter.h"
#include "FaceTheSunProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

// fire의 스켈레탈이 각자 다르기때문에 2번 중첩으로 재생되는 효과가 존재해서 분리
void UTP_WeaponComponent::MulticastFire_Implementation()
{
	if (AmmoCount == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptyAmmoSound, Character->GetActorLocation(), Character->GetActorRotation(), 0.3f);
	}
	else
	{
		if (Character == nullptr)
			return;
		//만약 해당 캐릭터를 소유한사람 즉, 총발사자한테는 2개의 파티클 이펙트와 2개의 총알이 생성될 필요가 없음.
		if (Character->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
		{
			if (ParticleEffect)
			{
				UGameplayStatics::SpawnEmitterAttached(ParticleEffect, this, TEXT("Muzzle"));
				if (ProjectileClass != nullptr)
				{
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// Spawn the projectile at the muzzle
					GetWorld()->SpawnActor<AFaceTheSunProjectile>(ProjectileClass, this->GetSocketLocation(TEXT("Muzzle")), GetSocketRotation(TEXT("Muzzle")), ActorSpawnParams);
				}
			}
			AmmoCount--;
		}
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, Character->GetActorLocation(), Character->GetActorRotation(), 0.3f);
	}
	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
			AnimInstance->Montage_Play(FireAnimation);
	}
}

void UTP_WeaponComponent::ServerFire_Implementation()
{
	MulticastFire();
}

void UTP_WeaponComponent::P1Fire()
{
	if (AmmoCount > 0)
	{
		if (Character == nullptr)
			return;
		if (ParticleEffect)
			UGameplayStatics::SpawnEmitterAttached(ParticleEffect, this, TEXT("Muzzle"));
		// Try and fire a projectile
		if (ProjectileClass != nullptr)
		{
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AFaceTheSunProjectile>(ProjectileClass, this->GetSocketLocation(TEXT("Muzzle")), GetSocketRotation(TEXT("Muzzle")), ActorSpawnParams);
		}
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
				AnimInstance->Montage_Play(FireAnimation);
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(AFaceTheSunCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}
	// Attach the weapon to the First Person Character
	/*
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
	*/
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}
	/*
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
	*/
}