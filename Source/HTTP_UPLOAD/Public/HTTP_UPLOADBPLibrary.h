// Copyright ECG. All Rights Reserved.

#pragma once

#include "Http.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HTTP_UPLOADBPLibrary.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHttpRequestR, const bool, bWasSuccessful,const FString, Message,const TArray<uint8>&, bytes,const FString ,ID);


UCLASS()
class UHTTP_UPLOADBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "HTTP_UPLOAD sample test testing"), Category = "HTTP_UPLOADTesting")
	static float HTTP_UPLOADSampleFunction(float Param);

	UFUNCTION(BlueprintCallable, Category = "HTTP_UPLOADTesting")
	static URequerstObj* UPloadFile(FString URL, FString FileName, FString FilePath, TMap<FString, FString> Keys);

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestHTTP, FString, IsOver);
	UFUNCTION(BlueprintCallable, Category = "HTTP_UPLOADTesting")
	static TArray<uint8> FstringToBytes(FString ChangeString);

	UFUNCTION(BlueprintCallable, Category = "HTTP_UPLOADTesting")
	static FString BytesToFstring(TArray<uint8> Content);

	UFUNCTION(BlueprintCallable, Category = "HTTP_UPLOADTesting")
	static URequerstObj* PostJson(FString URL, FString JsonString, FString ID);

	UFUNCTION(BlueprintCallable, Category = "HTTP_UPLOADTesting")
	static URequerstObj* PostParameter(FString URL, TMap<FString, FString> Parameters, FString ID);

	UFUNCTION(BlueprintCallable, Category = "HTTP_UPLOADTesting")
	static URequerstObj* Get(FString URL, FString ID);

	UFUNCTION(BlueprintCallable, Category = "HTTP_UPLOADTesting")
	static URequerstObj* GetParameter(FString URL, TMap<FString, FString> Parameters, FString ID);

};

UCLASS(Blueprintable, BlueprintType)
class URequerstObj :public UObject
{
	GENERATED_BODY()

public:

	void GetRQ(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);

	UPROPERTY(BlueprintAssignable, Category = "HTTP_UPLOAD")
	FHttpRequestR OnHttpCompletedReturn;

	FString ID;

};