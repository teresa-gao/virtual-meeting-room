// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFunctionLibrary.h"

//File picker includes
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

void UBPFunctionLibrary::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames)
{
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
				DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
			}
		}
	}
}


void UBPFunctionLibrary::PrintData(const FString& File)
{
	//Parse the data into a string array
	TArray<FString> LoadedText;
	FFileHelper::LoadFileToStringArray(LoadedText, *File);
	//Print the contents
	for (int32 i = 0; i < LoadedText.Num(); i++)
	{
		GLog->Log(LoadedText[i]);
	}
}

void UBPFunctionLibrary::CopySelectedFileToPath()
{
	const FString& TargetPath = "C:/Database/";
	TArray<FString> SelectedFiles;
	OpenFileDialog(TEXT("Select a File"), TEXT(""), TEXT("Any File (*.*)|*.*"), SelectedFiles);

	if (SelectedFiles.Num() > 0)
	{
		FString SourcePath = SelectedFiles[0]; // Assuming only one file is selected for simplicity
		FString DestinationPath = FPaths::Combine(TargetPath, FPaths::GetCleanFilename(SourcePath));

		if (FPlatformFileManager::Get().GetPlatformFile().CopyFile(*DestinationPath, *SourcePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("File copied successfully to %s"), *DestinationPath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to copy file to %s"), *DestinationPath);
		}
	}
}
