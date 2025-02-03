// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Display.h"

#include "Components/TextBlock.h"

void UUW_Display::SetModeText(FText text)
{
	Mode->SetText(text);
}

void UUW_Display::SetModeValue(FText text)
{
	Value->SetText(text);
}
