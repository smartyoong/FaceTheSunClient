// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KrakenBeamSkillActor.generated.h"

UCLASS()
class FACETHESUN_API AKrakenBeamSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKrakenBeamSkillActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BeamComp;
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* Effect;
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* Sound;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	TArray<class AFaceTheSunCharacter*> DamageCharacters;
	FTimerHandle SkillHandle;
	UPROPERTY(EditDefaultsOnly)
	float SkillTime = 5.f;
	virtual void Tick(float DeltaTime) override;
};
