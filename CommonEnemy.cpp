// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonEnemy.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CommonEnemyAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "CommonAiContoroller.h"

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
	if (IsDead)
	{
		//죽으면 밑으로 쭉 내려가게해서 자동으로 ZDeath로 알아서 죽음
		FVector NowLoc = GetActorLocation();
		FVector VeloTime = FVector::DownVector * 50.f * DeltaTime;
		SetActorLocation(NowLoc + VeloTime);
	}
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
	{
		auto PE =Cast<UCommonEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (PE)
		{
			PE->StopAllMontages(0.f);
		}
		MultiDeath();
		return Damage;
	}
	else
		MultiHit();
	return Damage;
}

void ACommonEnemy::MultiDeath_Implementation()
{
	auto EC = Cast<ABossKarakenAiContoroller>(GetController());
	if (EC)
	{
		EC->StopBTTree();
	}
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (DeathAnimation)
	{
		auto PE =Cast<UCommonEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (PE)
		{
			PE->StopAllMontages(0.f);
			PE->Montage_Play(DeathAnimation);
		}
		else
			UE_LOG(LogTemp, Log, TEXT("Fail to cast Enemy Anim Instance"));
	}
}

void ACommonEnemy::MultiAttack_Implementation()
{
	if (AttackAnimation)
	{
		int32 index = FMath::RandRange(1, 2);
		FString SectionIndex = FString::Printf(TEXT("Attack%d"), index);
		auto PE = Cast<UCommonEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (PE)
		{
			PE->Montage_Play(AttackAnimation);
			PE->Montage_JumpToSection(FName(*SectionIndex), AttackAnimation);
		}
		else
			UE_LOG(LogTemp, Log, TEXT("Fail to cast Enemy Anim Instance"));
	}
}

void ACommonEnemy::MultiSetFocus_Implementation()
{
	if (BeforeAttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BeforeAttackSound, GetActorLocation(), GetActorRotation(), 0.2f);
	}
}

void ACommonEnemy::MultiHit_Implementation()
{
	if (HitAnimation)
	{
		auto PE = Cast<UCommonEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (PE)
			PE->Montage_Play(HitAnimation);
		else
			UE_LOG(LogTemp, Log, TEXT("Fail to cast Enemy Anim Instance"));
	}
}

void ACommonEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACommonEnemy, HP);
}