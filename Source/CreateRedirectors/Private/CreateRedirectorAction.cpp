// Fill out your copyright notice in the Description page of Project Settings.
#include "CreateRedirectorAction.h"
#include "ContentBrowserModule.h"
#include "Editor.h"
#include "EditorAssetLibrary.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Internationalization/Regex.h"
#include "Widgets/Input/SFilePathPicker.h"

#define LOCTEXT_NAMESPACE "FCreateRedirectorsModule"

UCreateRedirectorAction::UCreateRedirectorAction()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FContentBrowserMenuExtender_SelectedPaths>& AssetViewExtenderDelegates = ContentBrowserModule.GetAllAssetContextMenuExtenders();
	AssetViewExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedPaths::CreateUObject(this, &UCreateRedirectorAction::CreateRedirectorExtender));
}

TSharedRef<FExtender> UCreateRedirectorAction::CreateRedirectorExtender(const TArray<FString>& Path)
{
	if(!Path.IsEmpty())
		ContentBrowserPath = Path[0];
	
	TSharedPtr<FExtender> CreateRedirectorExtension = MakeShareable(new FExtender());
	CreateRedirectorExtension->AddMenuExtension("NewFolder", EExtensionHook::After, TSharedPtr<FUICommandList>(), FMenuExtensionDelegate::CreateUObject(this, &UCreateRedirectorAction::AddExtension));
	return CreateRedirectorExtension.ToSharedRef();
}

void UCreateRedirectorAction::AddExtension(class FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Create Redirector Here"),
		FText::FromString("Create Redirector Here"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateUObject(this, &UCreateRedirectorAction::CreateRedirectorDialog))
	);
}

void UCreateRedirectorAction::CreateRedirectorDialog()
{
	// Create the dialog
	TSharedRef<SWindow> SharedWindow = SNew(SWindow)
	   .Title(LOCTEXT("CreateRedirectorDialogTitle", "Create Redirector"))
	   .ClientSize(FVector2D(400, 100))
	   .SupportsMaximize(false)
	   .SupportsMinimize(false)
	   .SizingRule(ESizingRule::FixedSize);

	TSharedRef<SWidget> Widget = SNew(SVerticalBox)
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("CreateRedirectorDialogText", "Select the file to redirect to"))
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SObjectPropertyEntryBox)
			.ObjectPath(ContentBrowserPath)
			.AllowedClass(UObject::StaticClass())
			.OnObjectChanged(FOnSetObject::CreateUObject(this, &UCreateRedirectorAction::OnObjectChanged))
			.DisplayThumbnail(true)
			.DisplayBrowse(true)
			.DisplayUseSelected(true)
			.DisplayCompactSize(true)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		.Padding(5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(LOCTEXT("CreateRedirectorDialogButton", "Create Redirector"))
				.OnClicked(FOnClicked::CreateUObject(this,  &UCreateRedirectorAction::CreateRedirector))
				
			]
		];

	Window = &SharedWindow.Get();
		
	// add the widget to the window
	Window->SetContent(Widget);
	
	// Show the dialog
	GEditor->EditorAddModalWindow(SharedWindow);
}

void UCreateRedirectorAction::OnObjectChanged(const FAssetData& NewAssetData)
{
	this->AssetData = NewAssetData;
}

FReply UCreateRedirectorAction::CreateRedirector() const
{
	const EObjectFlags Flags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	
	FString NewPath = AssetData.GetPackage()->GetPathName();
	
	// Get the file name from the path
	FRegexPattern FileNamePattern(TEXT("([^/]+)$"));
	FRegexMatcher FileNameMatcher(FileNamePattern, NewPath);
	FString FileName;
	if (FileNameMatcher.FindNext())
	{
		FileName = FileNameMatcher.GetCaptureGroup(1);
	}

	FString Path = ContentBrowserPath + "/" + FileName;

	// Log the old and new file paths
	UE_LOG(LogTemp, Warning, TEXT("Old File Path: %s"), *Path);
	UE_LOG(LogTemp, Warning, TEXT("Current File Path: %s"), *NewPath);

	FString AssetPath = FPaths::ProjectContentDir() + ContentBrowserPath.Replace(TEXT("/Game/"), TEXT(""));

	// Create redirector
	UObject *NewAsset = UEditorAssetLibrary::LoadAsset(NewPath);
	if (!NewAsset) return FReply::Handled();
	UPackage *Package = CreatePackage(*Path);
	UObjectRedirector *Redirector = NewObject<UObjectRedirector>(Package, NewAsset->GetFName(), Flags);
	Redirector->DestinationObject = NewAsset;
	FAssetRegistryModule::AssetCreated(Redirector);
	Redirector->MarkPackageDirty();

	if (UBlueprint *Blueprint = Cast<UBlueprint>(NewAsset)) {
		// Blueprints need extra redirector for class and CDO, see
		// https://github.com/EpicGames/UnrealEngine/blob/c3caf7b6bf12ae4c8e09b606f10a09776b4d1f38/Engine/Source/Editor/UnrealEd/Private/ObjectTools.cpp#L1450-L1477
		UObjectRedirector *RedirectorClass = NewObject<UObjectRedirector>(Package, Blueprint->GeneratedClass->GetFName(), Flags);
		RedirectorClass->DestinationObject = Blueprint->GeneratedClass;
		FAssetRegistryModule::AssetCreated(RedirectorClass);
		RedirectorClass->MarkPackageDirty();

		UObjectRedirector *RedirectorCDO = NewObject<UObjectRedirector>(Package, Blueprint->GeneratedClass->GetDefaultObject()->GetFName(), Flags);
		RedirectorCDO->DestinationObject = Blueprint->GeneratedClass->GetDefaultObject();
		FAssetRegistryModule::AssetCreated(RedirectorCDO);
		RedirectorCDO->MarkPackageDirty();
	}

	FString FilePath = FString::Printf(TEXT("%s/%s%s"), *AssetPath, *FileName, *FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, Redirector, Flags, *FilePath);
	Package = ReloadPackage(Package, 0);

	Window->RequestDestroyWindow();
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

