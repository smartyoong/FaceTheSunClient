// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonEnemyAnimInstance.h"
#include "FaceTheSunCharacter.h"
#include "CommonEnemy.h"
#include "Kismet/GameplayStatics.h"

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
			if (GetWorld()->LineTraceSingleByChannel(Hit, Enemy->GetActorLocation(), Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * 100, ECC_GameTraceChannel1, Params))
			{
				AFaceTheSunCharacter* Character = Cast<AFaceTheSunCharacter>(Hit.GetActor());
				if (Character)
				{
					// attack range = 45
					if (FVector::Distance(Enemy->GetActorLocation(), Character->GetActorLocation()) < Enemy->GetAttackRange())
					{
						UGameplayStatics::ApplyDamage(Character, Enemy->AttackDamage, Enemy->GetController(), Enemy, nullptr);
					}
				}
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
			Enemy->Destroy();
		}
	}
}