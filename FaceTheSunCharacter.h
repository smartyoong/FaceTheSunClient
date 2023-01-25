// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimeLineComponent.h"
#include "GenericTeamAgentInterface.h"
#include "FaceTheSunCharacter.generated.h"
#define ROLE_TO_STRING(Value) FindObject<UEnum>(ANY_PACKAGE,TEXT("ENetRole"),true)->GetNameStringByIndex((int32)Value)
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AFaceTheSunCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	/** �ɱ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;
	/** �޸��� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;
	/*�߻�*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/*����*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/*������*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightAction;

	/*�׾����� �ٸ� ��� ȭ�� ����*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* NextCameraAction;
	
public:
	AFaceTheSunCharacter();

	void ToggleLight();
	void SpectateNextPlayer();
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsDead = false;
protected:
	virtual void BeginPlay();
	int32 PlayerCameraIndex = 0;
public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	void SetReloadingNow(bool Reload) { bIsReloadingNow = Reload; }
	UFUNCTION(NetMulticast, Reliable)
	void CallWinFunc();
	UFUNCTION(NetMulticast, Reliable)
	void CallDefeatFunc();
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRun();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStopRun();
	UFUNCTION(Server, Unreliable)
	void ServerRun();
	UFUNCTION(Server, Unreliable)
	void ServerStopRun();

	void StartCrouch();
	void Reloading();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastReloading();
	UFUNCTION(Server, Reliable)
	void ServerReloading();
	UFUNCTION(Client, Reliable)
	void ClientReloading();
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsReloadingNow = false;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsLightOn = true;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun") 
	class UTP_WeaponComponent* Gun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun") 
	class UTP_WeaponComponent* Gun1P;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Light")
	class USpotLightComponent* FlashLight;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "HP")
	float HP = 100.f;

	virtual void Tick(float DeltaTime) override;
	void Fire();
	void StopFire();
	void StartFire();
	//RPC
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire();
	UFUNCTION(Server, Reliable)
	void ServerFire();
	UFUNCTION(Client, Reliable)
	void ClientFire();

	//�ɱ�
	FOnTimelineFloat SmoothCrouchInterpFunction;
	UFUNCTION()
	void SmoothCrouchInterpReturn(float Value);
	UPROPERTY()
	UTimelineComponent* SmoothCrouchingCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* SmoothCrouchingCurveFloat;
	void SmoothCrouchTimelineSetting();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCrouch();
	UFUNCTION(Server, Reliable)
	void ServerCrouch();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopCrouch();
	UFUNCTION(Server, Reliable)
	void ServerStopCrouch();

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsShot = false;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLight();
	UFUNCTION(Server, Unreliable)
	void ServerLight();
	 //�� ������
	int32 ID = 1;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	bool bCIsCrouch = false;
	// ������
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(Server, Reliable)
	void ServerOnDeath();
	UFUNCTION(NetMulticast, Reliable)
	void MultiOnDeath();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death)
	UAnimMontage* HitAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death)
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death)
	USoundBase* HitSound;
	UFUNCTION(Server, Reliable)
	void ServerOnHit();
	UFUNCTION(NetMulticast, Reliable)
	void MultiOnHit();
	UFUNCTION(NetMultiCast, Unreliable)
	void MultiHeal();
	UFUNCTION(Server, Unreliable)
	void ServerHeal();
	UFUNCTION(NetMultiCast, Unreliable)
	void MultiAmmo();
	UFUNCTION(Server, Unreliable)
	void ServerAmmo();
	UPROPERTY(EditDefaultsOnly)
	USoundBase* RechargeAmmoSound;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Particle) class UParticleSystem* AmmoParticleEffect;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* HealSound;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Particle) class UParticleSystem* HealParticleEffect;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
private:
	// ���縦 �����ϱ� ���� �뵵
	FTimerHandle CharacterTimer;
	bool bIsRun = false;
	FGenericTeamId TeamId;
	class UPlayerSciFiAnimation* PlayerSciAnim;
	class AFaceTheSunGameState* MyGameState;
	class UParticleSystemComponent* pe;
};

