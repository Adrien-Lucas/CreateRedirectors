// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Widgets/Input/SDirectoryPicker.h"
#include "CreateRedirectorAction.generated.h"
/**
 * 
 */
UCLASS()
class UCreateRedirectorAction : public UObject
{
	GENERATED_BODY()

protected:
	
	UCreateRedirectorAction();
	
	FReply CreateRedirector() const;
	
	void AddExtension(FMenuBuilder& MenuBuilder);
	void OnObjectChanged(const FAssetData& NewAssetData);
	void CreateRedirectorDialog();
	TSharedRef<FExtender> CreateRedirectorExtender(const TArray<FString>& Path);

	FString ContentBrowserPath;
	FAssetData AssetData;
	SWindow* Window;
};
