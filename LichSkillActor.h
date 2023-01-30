// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LichSkillActor.generated.h"

UCLASS()
class FACETHESUN_API ALichSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALichSkillActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* LichComp;
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* Effect;
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* Sound;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
