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

// fire서버측에서 총알을 생성하도록한다
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
			PE->SetOwnerNoSee(true);
		}
		if (FireAnimation != nullptr)
		{
			Character->GetMesh()->GetAnimInstance()->Montage_Play(FireAnimation);
		}
		// 서버에서만 총알이 생성되도록 한다, 그래서 총알이 2번생성되지 않도록한다.
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
					Character->GetMesh1P()->GetAnimInstance()->Montage_Play(FireAnimation);
			}
			AmmoCount--;
		}
}

void UTP_WeaponComponent::Reloading()
{
	if (TotalAmmo <= 0)
	{
		Character->SetReloadingNow(false);
		return;
	}
	AmmoHouse = NewObject<UStaticMeshComponent>(Character,TEXT("ReloadHouse"));
	AmmoHouse->SetStaticMesh(AmmoHouseMesh);
	AmmoHouse->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ReloadPoint"));
	AmmoHouse->RegisterComponent();
	AmmoHouse->SetOwnerNoSee(true);
	AmmoHouse->SetIsReplicated(true);
	GetWorld()->GetTimerManager().SetTimer(WeaponTimer, this, &UTP_WeaponComponent::SetRelativeLocationAmmo, 0.1, false);
	if (ReloadAnimation)
	{
		if (Character->GetMesh()->GetAnimInstance())
			Character->GetMesh()->GetAnimInstance()->Montage_Play(ReloadAnimation);
		else if (Character->GetMesh()->GetAnimInstance() == nullptr)
			UE_LOG(LogTemp, Warning, TEXT("No Anim Instance"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No Reload Animation"));
}

void UTP_WeaponComponent::Reloading1P()
{
	if (TotalAmmo <= 0)
	{
		Character->SetReloadingNow(false);
		return;
	}
	AmmoHouse = NewObject<UStaticMeshComponent>(Character, TEXT("ReloadHouse"));
	AmmoHouse->SetStaticMesh(AmmoHouseMesh);
	AmmoHouse->AttachToComponent(Character->GetMesh1P(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ReloadPoint"));
	AmmoHouse->RegisterComponent();
	AmmoHouse->SetOnlyOwnerSee(true);
	AmmoHouse->SetIsReplicated(true);
	GetWorld()->GetTimerManager().SetTimer(WeaponTimer, this, &UTP_WeaponComponent::SetRelativeLocationAmmo, 0.1, false);
	if (ReloadAnimation)
	{
		if (Character->GetMesh1P()->GetAnimInstance())
			Character->GetMesh1P()->GetAnimInstance()->Montage_Play(ReloadAnimation);
		else if (Character->GetMesh1P()->GetAnimInstance() == nullptr)
			UE_LOG(LogTemp, Warning, TEXT("No Anim Instance"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No Reload Animation"));
}

void UTP_WeaponComponent::AttachWeapon(AFaceTheSunCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}
	Character->SetHasRifle(true);

}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}
}

void UTP_WeaponComponent::EndReload()
{
	int32 AmmoOffset = 30 - AmmoCount;
	int32 temp = TotalAmmo - AmmoOffset;
	if (temp < 0)
	{
		temp = AmmoOffset - TotalAmmo;
		AmmoCount += (30-temp);
		TotalAmmo = 0;
	}
	else
	{
		TotalAmmo -= AmmoOffset;
		AmmoCount = 30;
	}
	AmmoHouse->UnregisterComponent();
	AmmoHouse->DestroyComponent();
	Character->SetReloadingNow(false);
	Character->GetWorldTimerManager().ClearTimer(WeaponTimer);
}

void UTP_WeaponComponent::EndReload1P()
{
	int32 AmmoOffset = 30 - AmmoCount;
	int32 temp = TotalAmmo - AmmoOffset;
	if (temp < 0)
	{
		temp = AmmoOffset - TotalAmmo;
		AmmoCount += (30-temp);
		TotalAmmo = 0;
	}
	else
	{
		TotalAmmo -= AmmoOffset;
		AmmoCount = 30;
	}
	AmmoHouse->UnregisterComponent();
	AmmoHouse->DestroyComponent();
	Character->SetReloadingNow(false);
	Character->GetWorldTimerManager().ClearTimer(WeaponTimer);
}

void UTP_WeaponComponent::SetRelativeLocationAmmo()
{
	AmmoHouse->SetRelativeLocation(Character->GetMesh()->GetSocketLocation(TEXT("ReloadPoint")));
}
