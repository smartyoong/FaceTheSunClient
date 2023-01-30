// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "CommonEnemy.generated.h"

UCLASS()
class FACETHESUN_API ACommonEnemy : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACommonEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int32 AttackDamage = 10;
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int32 HP = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 ID = 0;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	UFUNCTION(NetMulticast, Blueprintcallable, Reliable)
	void MultiDeath();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	UAnimMontage* DeathAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	double AttackRange = 90;
	double GetAttackRange() { return AttackRange; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	UAnimMontage* HitAnimation;
	UFUNCTION(NetMulticast,Blueprintcallable, Reliable)
	void MultiAttack();
	UPROPERTY(BlueprintReadWrite)
	bool IsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	USoundBase* BeforeAttackSound;
	UFUNCTION(NetMulticast, Blueprintcallable, Unreliable)
	void MultiSetFocus();
	UFUNCTION(NetMulticast, Blueprintcallable, Unreliable)
	void MultiHit();
protected:
	FGenericTeamId TeamId;

};
