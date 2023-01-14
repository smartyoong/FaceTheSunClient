// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"
class AFaceTheSunCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FACETHESUN_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFaceTheSunProjectile> ProjectileClass;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	//추후 리플리케이트 시킬예정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoCount = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 TotalAmmo = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float FireSpeed = .75f;

	float GetFireSpeed() { return FireSpeed; }


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundBase* EmptyAmmoSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundBase* FireSound;

	UStaticMeshComponent* AmmoHouse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	UStaticMesh* AmmoHouseMesh;


	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Particle) class UParticleSystem* ParticleEffect;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AFaceTheSunCharacter* TargetCharacter);

	/*타인 시점*/
	void TPFire();
	//개인화면
	void P1Fire();

	void Reloading();
	void Reloading1P();
	void EndReload();
	void EndReload1P();
	void SetRelativeLocationAmmo();
	FTimerHandle WeaponTimer;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AFaceTheSunCharacter* Character;
};
