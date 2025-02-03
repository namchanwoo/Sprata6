
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingActor.generated.h"

class UWidgetComponent;
/** 이동 모드를 결정하는 열거형 */
UENUM(BlueprintType)
enum class EPlatFormerMovementMode : uint8
{
	RandomLinear      UMETA(DisplayName="Random Linear"),    
	Circular          UMETA(DisplayName="Circular"),         
	ZigZag            UMETA(DisplayName="Zig Zag"),   
	ForwardBackward   UMETA(DisplayName="Forward-Backward")  
};


UCLASS()
class SPRATABASE_API AMovingActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void HandleRandomLinearMovement(float DeltaTime);
	
	void HandleCircularMovement(float DeltaTime);
	
	void HandleZigZagMovement(float DeltaTime);
	
	void HandleForwardBackwardMovement(float DeltaTime);


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platformer|Components",meta=(AllowPrivateAccess=true))
	USceneComponent* DefaultScene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platformer|Components", meta=(AllowPrivateAccess=true))
	UWidgetComponent* WidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platformer|Components",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|Settings",meta=(AllowPrivateAccess=true))
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|Settings",meta=(AllowPrivateAccess=true))
	float MaxRange;

	/** 이동 모드 선택(열거형) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|Settings", meta=(AllowPrivateAccess=true))
	EPlatFormerMovementMode MovementMode;
	
	FVector StartLocation;
	
	FVector MoveDirection;       // RandomLinear / ForwardBackward 모드에서 사용
	float CircleAngle;           // 원 이동(Circular) 모드에서 사용하는 각도
	float ElapsedTime;           // Tick 누적 시간 (ZigZag 등에서 사용)
};
