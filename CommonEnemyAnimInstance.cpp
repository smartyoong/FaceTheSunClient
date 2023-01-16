// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonEnemyAnimInstance.h"
#include "FaceTheSunCharacter.h"
#include "CommonEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CommonAiContoroller.h"
#include "BehaviorTree/BehaviorTree.h"

void UCommonEnemyAnimInstance::AnimNotify_Attack()
{
	auto Pawn = Cast<ACommonEnemy>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		ACommonEnemy* Enemy = Cast<ACommonEnemy>(Pawn);
		if (Enemy)
		{
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Enemy);
			if (GetWorld()->SweepSingleByChannel(Hit,Enemy->GetActorLocation(),Enemy->GetActorLocation()+Enemy->GetActorForwardVector()*100,FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel1,FCollisionShape::MakeBox(FVector(20.f,20.f,20.f)),Params))
			{
				AFaceTheSunCharacter* Character = Cast<AFaceTheSunCharacter>(Hit.GetActor());
				if (Character)
				{
					// attack range = 45
					if (Hit.Distance < Enemy->GetAttackRange())
					{
						UGameplayStatics::ApplyDamage(Character, Enemy->AttackDamage, Enemy->GetController(), Enemy, nullptr);
					}
				}
				else
					UE_LOG(LogTemp, Log, TEXT("Fail To Cast Attack AI To Character"));
			}
		}
	}
}

void UCommonEnemyAnimInstance::AnimNotify_Die()
{
	auto Pawn = Cast<ACommonEnemy>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		ACommonEnemy* Enemy = Cast<ACommonEnemy>(Pawn);
		if (Enemy)
		{
			Enemy->IsDead = true;
			auto EC = Cast<ACommonAiContoroller>(Enemy->GetController());
			if(EC)
				EC->StopBTTree();
		}
	}
}

void UCommonEnemyAnimInstance::AnimNotify_AttackEnd()
{
	auto Pawn = Cast<ACommonEnemy>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		ACommonEnemy* Enemy = Cast<ACommonEnemy>(Pawn);
		if (Enemy)
		{
			Enemy->GetMesh()->GetAnimInstance()->Montage_Stop(0.25,Enemy->AttackAnimation);
		}
	}
}

void UCommonEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = Cast<ACommonEnemy>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		ACommonEnemy* Enemy = Cast<ACommonEnemy>(Pawn);
		if (Enemy)
		{
			speed = FVector::DotProduct(Enemy->GetVelocity(), Enemy->GetActorForwardVector());
		}
	}
}