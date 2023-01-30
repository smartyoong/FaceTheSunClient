// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinAnimInstance.h"
#include "FaceTheSunCharacter.h"
#include "CommonEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CommonAiContoroller.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void UGoblinAnimInstance::AnimNotify_GoblinAttack()
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
					if (Character->IsDead)
					{
						auto EC = Cast<ACommonAiContoroller>(Enemy->GetController());
						if (EC)
						{
							EC->GetBlackboard()->SetValueAsBool(TEXT("CanAttack"), false);
						}
					}
					if (Hit.Distance < Enemy->GetAttackRange())
					{
						UGameplayStatics::ApplyRadialDamage(GetWorld(), Enemy->AttackDamage, Enemy->GetActorLocation(), Enemy->GetAttackRange(),nullptr ,TArray<AActor*>() , Enemy,Enemy->GetController());
                        Enemy->Destroy();
					}
				}
				else
					UE_LOG(LogTemp, Log, TEXT("Fail To Cast Attack AI To Character"));
			}
		}
	}
}

void UGoblinAnimInstance::AnimNotify_GoblinDie()
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
