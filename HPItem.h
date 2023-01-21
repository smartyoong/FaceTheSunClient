// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HPItem.generated.h"

UCLASS()
class FACETHESUN_API AHPItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHPItem();
	UPROPERTY(EditDefaultsOnly)
	class UTP_PickUpComponent* PickComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly)
	class URotatingMovementComponent* RotatingComp;
	UPROPERTY(EditDefaultsOnly)
	int32 HealHP = 50;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void HealUp(class AFaceTheSunCharacter* ch);
};
