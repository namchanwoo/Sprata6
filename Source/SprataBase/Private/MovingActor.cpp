// Fill out your copyright notice in the Description page of Project Settings.


#include "SprataBase/Public/MovingActor.h"

#include "UW_Display.h"
#include "Components/WidgetComponent.h"


// Sets default values
AMovingActor::AMovingActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(DefaultScene);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(DefaultScene);

	// 위젯 컴포넌트 생성 및 설정
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(MeshComp);
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen); // 화면 공간에 표시
	WidgetComp->SetDrawSize(FVector2D(200.f, 50.f)); // 위젯 크기 설정
	WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f)); // 메쉬 위에 배치

	MoveSpeed = 300.f;
	MaxRange = 400.f;
	MovementMode = EPlatFormerMovementMode::RandomLinear;

	MoveDirection = FVector::ZeroVector;
	CircleAngle = 0.f;
	ElapsedTime = 0.f;
}

// Called when the game starts or when spawned
void AMovingActor::BeginPlay()
{
	Super::BeginPlay();

	if (UUW_Display* Widget = Cast<UUW_Display>(WidgetComp->GetUserWidgetObject()))
	{
		// Mode 텍스트 갱신
		FString ModeText;
		switch (MovementMode)
		{
		case EPlatFormerMovementMode::RandomLinear:
			ModeText = TEXT("Random Linear");
			break;
		case EPlatFormerMovementMode::Circular:
			ModeText = TEXT("Circular");
			break;
		case EPlatFormerMovementMode::ZigZag:
			ModeText = TEXT("Zig Zag");
			break;
		case EPlatFormerMovementMode::ForwardBackward:
			ModeText = TEXT("Forward-Backward");
			break;
		default:
			ModeText = TEXT("Unknown");
			break;
		}

		if (Widget->Mode)
		{
			Widget->SetModeText(FText::FromString(ModeText));
		}

		// Value 텍스트 갱신 (현재 요청에 따라 비워둠)
		if (Widget->Value)
		{
			Widget->SetModeValue(FText::GetEmpty());
		}
	}

	StartLocation = GetActorLocation();

	switch (MovementMode)
	{
	case EPlatFormerMovementMode::RandomLinear:
		{
			// 임의 각도 생성
			const float RandomAngle = FMath::RandRange(0.f, 360.f);
			const float Radians = FMath::DegreesToRadians(RandomAngle);

			// XY 평면 임의 방향
			const float DirX = FMath::Cos(Radians);
			const float DirY = FMath::Sin(Radians);
			MoveDirection = FVector(DirX, DirY, 0.f);
			break;
		}
	case EPlatFormerMovementMode::Circular:
		{
			// 초기 각도 0 (필요하다면 RandomAngle로 시작해도 됨)
			CircleAngle = 0.f;
			break;
		}
	case EPlatFormerMovementMode::ZigZag:
		{
			ElapsedTime = 0.f;
			break;
		}
	case EPlatFormerMovementMode::ForwardBackward:
		{
			// +X 방향으로 시작
			MoveDirection = FVector(1.f, 0.f, 0.f);
			break;
		}
	default: break;
	}
}

// Called every frame
void AMovingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (MovementMode)
	{
	case EPlatFormerMovementMode::RandomLinear:
		HandleRandomLinearMovement(DeltaTime);
		break;

	case EPlatFormerMovementMode::Circular:
		HandleCircularMovement(DeltaTime);
		break;

	case EPlatFormerMovementMode::ZigZag:
		HandleZigZagMovement(DeltaTime);
		break;

	case EPlatFormerMovementMode::ForwardBackward:
		HandleForwardBackwardMovement(DeltaTime);
		break;

	default: break;
	}
}


void AMovingActor::HandleRandomLinearMovement(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector DeltaMove = MoveDirection * MoveSpeed * DeltaTime;
	CurrentLocation += DeltaMove;

	SetActorLocation(CurrentLocation);

	float DistanceFromStart = FVector::Dist2D(StartLocation, CurrentLocation);
	if (DistanceFromStart >= MaxRange)
	{
		float Overshoot = DistanceFromStart - MaxRange;

		// 반전
		MoveDirection *= -1.f;

		// 오버슈트만큼 보정
		FVector Correction = MoveDirection.GetSafeNormal() * Overshoot;
		CurrentLocation += Correction;

		SetActorLocation(CurrentLocation);
	}
}

void AMovingActor::HandleCircularMovement(float DeltaTime)
{
	if (MaxRange <= 1.f)
	{
		// 반지름이 너무 작으면 그냥 리턴
		return;
	}

	// 각속도(라디안/초) = (이동속도 MoveSpeed) / (반지름 MaxRange)
	float AngularVelocity = MoveSpeed / MaxRange;

	// 각도 업데이트
	CircleAngle += AngularVelocity * DeltaTime;

	// XY 평면에서 원운동
	float X = FMath::Cos(CircleAngle) * MaxRange;
	float Y = FMath::Sin(CircleAngle) * MaxRange;

	FVector NewLocation = StartLocation + FVector(X, Y, 0.f);
	SetActorLocation(NewLocation);
}

void AMovingActor::HandleZigZagMovement(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	// X축으로 이동량: MoveSpeed * t
	float X = StartLocation.X + MoveSpeed * ElapsedTime;

	// Y축으로 사인 진동 (폭: MaxRange, 주기: 임의로 2.f 곱)
	float Y = StartLocation.Y + MaxRange * FMath::Sin(ElapsedTime * 2.f);

	// Z축은 고정
	FVector NewLocation(X, Y, StartLocation.Z);
	SetActorLocation(NewLocation);
}

void AMovingActor::HandleForwardBackwardMovement(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector DeltaMove = MoveDirection * MoveSpeed * DeltaTime;
	CurrentLocation += DeltaMove;

	SetActorLocation(CurrentLocation);

	float DistanceFromStart = FVector::Dist2D(StartLocation, CurrentLocation);
	if (DistanceFromStart >= MaxRange)
	{
		float Overshoot = DistanceFromStart - MaxRange;

		// 방향 반전
		MoveDirection *= -1.f;

		// 초과분 보정
		FVector Correction = MoveDirection.GetSafeNormal() * Overshoot;
		CurrentLocation += Correction;

		SetActorLocation(CurrentLocation);
	}
}