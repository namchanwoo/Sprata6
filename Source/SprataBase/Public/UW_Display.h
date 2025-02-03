
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Display.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SPRATABASE_API UUW_Display : public UUserWidget
{
	GENERATED_BODY()
public:


	void SetModeText(FText text);
	
	void SetModeValue(FText text);
	
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Display",meta=(BindWidget))
	UTextBlock* Mode;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Display",meta=(BindWidget))
	UTextBlock* Value;
};
