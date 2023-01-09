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
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

// fire�� ���̷�Ż�� ���� �ٸ��⶧���� 2�� ��ø���� ����Ǵ� ȿ���� �����ؼ� �и�
void UTP_WeaponComponent::TPFire()
{
	if (Character == nullptr)
		return;
	if (AmmoCount == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptyAmmoSound, Character->GetActorLocation(), Character->GetActorRotation(), 0.3f);
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, Character->GetActorLocation(), Character->GetActorRotation(), 0.3f);
		if (ParticleEffect)
		{
			auto PE = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, this, TEXT("Muzzle"));
			PE->SetIsReplicated(true);
			PE->SetOwnerNoSee(true);
		}
		if (FireAnimation != nullptr)
		{
			Character->GetMesh()->GetAnimInstance()->Montage_Play(FireAnimation);
		}
		// ������ ���͸� �����ϵ��� ������ Ŭ�� ���忡�� Simulated���ͷ� �������� ������ ������� Ŭ�������� �ߺ����� ����
		if (Character->GetLocalRole() == ENetRole::ROLE_Authority)
		{
			if (ProjectileClass != nullptr)
			{
				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// Spawn the projectile at the muzzle
				GetWorld()->SpawnActor<AFaceTheSunProjectile>(ProjectileClass, this->GetSocketLocation(TEXT("Muzzle")), Character->GetControlRotation(), ActorSpawnParams);
			}
		}
		AmmoCount--;
	}
}

void UTP_WeaponComponent::P1Fire()
{
		if (Character == nullptr)
			return;
		if (AmmoCount > 0)
		{
			// �߻�ü�� ��Ƽĳ��Ʈ�� ��ȯ
			if (ParticleEffect)
			{
				UGameplayStatics::SpawnEmitterAttached(ParticleEffect, this, TEXT("Muzzle"));
			}
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
					AnimInstance->Montage_Play(FireAnimation);
			}
			AmmoCount--;
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

/*
void UTP_WeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTP_WeaponComponent, AmmoCount);
	DOREPLIFETIME(UTP_WeaponComponent, TotalAmmo);
}
*/

