// Copyright Epic Games, Inc. All Rights Reserved.

#include "CreateRedirectors.h"

#include "ContentBrowserModule.h"
#include "CreateRedirectorAction.h"

#define LOCTEXT_NAMESPACE "FCreateRedirectorsModule"

void FCreateRedirectorsModule::StartupModule()
{
	//Create a UCreateRedirectorAction object
	NewObject<UCreateRedirectorAction>();
}

void FCreateRedirectorsModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCreateRedirectorsModule, CreateRedirectors)