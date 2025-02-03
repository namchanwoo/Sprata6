#include "SprataBase/Public/RotatingActor.h"

#include "UW_Display.h"
#include "Components/WidgetComponent.h"


// Sets default values
ARotatingActor::ARotatingActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(DefaultScene);

	// 메쉬 컴포넌트 생성 및 루트로 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(DefaultScene);

	// 위젯 컴포넌트 생성 및 설정
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(MeshComp);
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen); // 화면 공간에 표시
	WidgetComp->SetDrawSize(FVector2D(200.f, 50.f)); // 위젯 크기 설정
	WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f)); // 메쉬 위에 배치


	RotationSpeed = 90.0f;
	BehaviorMode = EActorBehavior::Disappear;

	CountdownTime = 5.0f;
	NewMesh = nullptr;
	NewMaterial = nullptr;
}

void ARotatingActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (UUW_Display* Widget = Cast<UUW_Display>(WidgetComp->GetUserWidgetObject()))
	{
		FString ModeText;
		switch (BehaviorMode)
		{
		case EActorBehavior::Disappear:
			ModeText = TEXT("Disappear");
			break;
		case EActorBehavior::MeshChange:
			ModeText = TEXT("MeshChange");
			break;
		case EActorBehavior::RotationChange:
			ModeText = TEXT("RotationChange");
			break;
		}

		Widget->SetModeText(FText::FromString(ModeText));
		Widget->SetModeValue(FText::AsNumber(CountdownTime));
	}

	if (CountdownTime > 0.f && (BehaviorMode == EActorBehavior::Disappear || BehaviorMode == EActorBehavior::MeshChange))
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle_Countdown,
			this,
			&ARotatingActor::OnCountdownTick,
			1.0f,
			true
		);
	}
}

void ARotatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator RotationDelta(0.0f, RotationSpeed * DeltaTime, 0.0f);
	AddActorLocalRotation(RotationDelta);
}

void ARotatingActor::UpdateRotationSpeed()
{
	RotationSpeed = FMath::RandRange(-180.0f, 180.0f);
}

void ARotatingActor::OnCountdownTick()
{
	CountdownTime -= 1.0f;
	
	// 위젯 갱신
	UpdateWidgetDisplay();

	if (CountdownTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Countdown);

		switch (BehaviorMode)
		{
		case EActorBehavior::Disappear:
			{
				Destroy();
				break;
			}
		case EActorBehavior::MeshChange:
			{
				if (NewMesh)
				{
					MeshComp->SetStaticMesh(NewMesh);
				}
				if (NewMaterial)
				{
					MeshComp->SetMaterial(0, NewMaterial);
				}

				break;
			}
		case EActorBehavior::RotationChange:
			UpdateRotationSpeed();
			break;

		default: break;
		}
	}
}

void ARotatingActor::UpdateWidgetDisplay()
{
	if (UUW_Display* Widget = Cast<UUW_Display>(WidgetComp->GetUserWidgetObject()))
	{
		Widget->SetModeValue(FText::AsNumber(CountdownTime));
	}
}