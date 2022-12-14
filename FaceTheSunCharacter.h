// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimeLineComponent.h"
#include "FaceTheSunCharacter.generated.h"
#define ROLE_TO_STRING(Value) FindObject<UEnum>(ANY_PACKAGE,TEXT("ENetRole"),true)->GetNameStringByIndex((int32)Value)
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AFaceTheSunCharacter : public ACharacter
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
	/** 앉기 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;
	/** 달리기 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;
	/*발사*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/*장전*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/*손전등*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightAction;
	
public:
	AFaceTheSunCharacter();

	void ToggleLight();

protected:
	virtual void BeginPlay();

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
	void StopCrouch();
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	int32 HP = 100;

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

	//앉기
	FOnTimelineFloat SmoothCrouchInterpFunction;
	UFUNCTION()
	void SmoothCrouchInterpReturn(float Value);
	UPROPERTY()
	UTimelineComponent* SmoothCrouchingCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* SmoothCrouchingCurveFloat;
	void SmoothCrouchTimelineSetting();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastCrouch();
	UFUNCTION(Server, Unreliable)
	void ServerCrouch();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStopCrouch();
	UFUNCTION(Server, Unreliable)
	void ServerStopCrouch();

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsShot = false;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLight();
	UFUNCTION(Server, Unreliable)
	void ServerLight();

private:
	// 연사를 구현하기 위한 용도
	FTimerHandle CharacterTimer;
	bool bIsRun = false;
};

