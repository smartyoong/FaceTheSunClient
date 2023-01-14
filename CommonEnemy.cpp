// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonEnemy.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CommonEnemyAnimInstance.h"

// Sets default values
ACommonEnemy::ACommonEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMovementComponent()->NavAgentProps.AgentHeight = 90.f;
	GetMovementComponent()->NavAgentProps.AgentRadius = 40.f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

// Called when the game starts or when spawned
void ACommonEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACommonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACommonEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	HP -= Damage;
	if (HP <= 0)
		Death();
	return Damage;
}

void ACommonEnemy::Death()
{
	check(GetCharacterMovement());
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	check(GetCapsuleComponent());
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	check(GetMesh());
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (DeathAnimation)
	{
		auto PE =Cast<UCommonEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (PE)
			PE->Montage_Play(DeathAnimation);
		else
			UE_LOG(LogTemp, Log, TEXT("Fail to cast Enemy Anim Instance"));
	}
}
