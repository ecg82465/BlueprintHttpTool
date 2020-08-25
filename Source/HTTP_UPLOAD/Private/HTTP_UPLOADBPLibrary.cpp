// Copyright ECG. All Rights Reserved.

#include "HTTP_UPLOADBPLibrary.h"
#include "HTTP_UPLOAD.h"
#include "Http.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

UHTTP_UPLOADBPLibrary::UHTTP_UPLOADBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

float UHTTP_UPLOADBPLibrary::HTTP_UPLOADSampleFunction(float Param)
{
	return -1;
}


URequerstObj* UHTTP_UPLOADBPLibrary::UPloadFile(FString URL,FString FileName, FString FilePath, TMap<FString, FString> Keys)
{

	// the local image file
	//FString UpFilePath = "/MyPath/MyFile.jpg";

	URequerstObj* Rbaby = NewObject<URequerstObj>();

	// the AU
	TArray<uint8> UpFileRawData;
	if (!FFileHelper::LoadFileToArray(UpFileRawData, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can'tnot Load TheFile Please Check the Path"));
		return Rbaby;
	}
	 
	//FFileHelper::SaveArrayToFile(UpFileRawData,TEXT("E:\\TVMGit\\VideoEditor\\ttt.txt"));
	
	// the request 
	//+ FString::FromInt(FDateTime::Now().GetTicks())
	FString Boundary = "25b65ef91754970cc500eac5" + FString::FromInt(FDateTime::Now().GetTicks());

	TSharedRef<IHttpRequest> FileUploadRequest = (&FHttpModule::Get())->CreateRequest();


	FileUploadRequest->OnProcessRequestComplete().BindUObject(Rbaby, &URequerstObj::GetRQ);
	FileUploadRequest->SetURL(URL);
	FileUploadRequest->SetVerb("POST");
	FileUploadRequest->SetHeader("Content-Type", "multipart/form-data; boundary="+ Boundary);
	//FileUploadRequest->SetHeader("Accept-Encoding", "gzip");
	//FileUploadRequest->SetHeader("Accept", "");


	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FileUploadRequest->GetContentType());

	TArray<uint8> BodyData;
	FString KeyString;


	TArray<FString> KeyNames;
	Keys.GenerateKeyArray(KeyNames);


	//TArray<FString> KeysName = Keys.Num();
	if (Keys.Num())
	{
		for (int i = 0; i < Keys.Num(); i++)
		{

			KeyString.Append("\r\n--" + Boundary+"\r\n");
			KeyString.Append("Content-Disposition: form-data; name=\"" + KeyNames[i] + "\"\r\n\r\n");
			KeyString.Append(*Keys.Find(KeyNames[i]));

		}
	}

	

	FString FileString;
	TArray<uint8> KeysBytes;
	TArray<uint8> EndBytes;
	TArray<uint8> FileBytes;

	FileString.Append("\r\n--" + Boundary + "\r\n");

	FileString.Append("Content-Disposition: form-data; name=\"file\" ; filename=\""+FileName+"\"\r\n");
	FileString.Append("Content-Type: application/octet-stream\r\n\r\n");


	KeysBytes = FstringToBytes(KeyString);
	FileBytes = FstringToBytes(FileString);
	FileBytes.Append(UpFileRawData);

	EndBytes = FstringToBytes("\r\n--" + Boundary + "--");

	BodyData.Append(KeysBytes);
	BodyData.Append(FileBytes);
	BodyData.Append(EndBytes);


	//FString AllString = BytesToString(BodyData.GetData(),BodyData.Num());
	UE_LOG(LogTemp, Warning, TEXT("%s"),*KeyString);


	// content
	//FileUploadRequest->SetContent(UpFileRawData);
	 //useless ? cf doc SetHeader(): "Content-Length is the only header set for you"

	// request processing
	//BodyData = FstringToBytes(KeyString);

	FileUploadRequest->SetContent(BodyData);

	FileUploadRequest->SetHeader("Content-Length", FString::FromInt(BodyData.Num()));


	//TArray<uint8>PBytes = FileUploadRequest->GetContent();
	TArray<FString>heads = FileUploadRequest->GetAllHeaders();

	for (int i=0;i< heads.Num();i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Head :%s"), *heads[i]);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Ceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeb : %s"),*(KeyString+FileString));

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *BytesToString(PBytes.GetData(), PBytes.Num()));

	if (FileUploadRequest->ProcessRequest())
	{
		return Rbaby;
	}
	
	FileUploadRequest->OnProcessRequestComplete().Unbind();


	//UE_LOG(LogTemp, Warning, TEXT("Ceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeb : %s"));


	return Rbaby;
	
}

URequerstObj* UHTTP_UPLOADBPLibrary::UPloadFiles(FString URL, const TMap<FString, FString> Files, TMap<FString, FString> Keys)
{

	URequerstObj* Rbaby = NewObject<URequerstObj>();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL);

	// Request header
	FString Boundary = "---------------------------" + FString::FromInt(FDateTime::Now().GetTicks());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data;boundary =" + Boundary));
	HttpRequest->SetVerb(TEXT("POST"));
	


	//files 
	TArray<FString> FilesNames;
	Files.GenerateKeyArray(FilesNames);

	TArray<uint8> RequestContent;
	for (const FString& FileName : FilesNames)
	{
		TArray<uint8> FileContent;
		FString FilePath = *Files.Find(FileName);

		if (FFileHelper::LoadFileToArray(FileContent, *FilePath))
		{
			FString BeginBoundry = "\r\n--" + Boundary + "\r\n";
			RequestContent.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundry), BeginBoundry.Len());

			FString FileHeader = "Content-Disposition: form-data;";//文件头
			FileHeader.Append("name=\"" + FileName + "\";");
			FileHeader.Append("filename=\"" + FPaths::GetCleanFilename(FilePath) + "\"");
			FileHeader.Append("\r\nContent-Type: \r\n\r\n");
			RequestContent.Append((uint8*)TCHAR_TO_ANSI(*FileHeader), FileHeader.Len());

			RequestContent.Append(FileContent);
		}
	}

	//keys parameter
	FString KeyString;
	TArray<FString> KeyNames;
	Keys.GenerateKeyArray(KeyNames);
	if (Keys.Num())
	{
		for (int i = 0; i < Keys.Num(); i++)
		{
			KeyString.Append("\r\n--" + Boundary + "\r\n");
			KeyString.Append("Content-Disposition: form-data; name=\"" + KeyNames[i] + "\"\r\n\r\n");
			KeyString.Append(*Keys.Find(KeyNames[i])+"\r\n");

		}
	}

	TArray<uint8> KeyBytes = FstringToBytes(KeyString);
	RequestContent.Append(KeyBytes);

	FString EndBoundary = "\r\n--" + Boundary + "--\r\n";
	RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

	HttpRequest->SetContent(RequestContent);
	

	HttpRequest->OnProcessRequestComplete().BindUObject(Rbaby, &URequerstObj::GetRQ);
	HttpRequest->ProcessRequest();


	return Rbaby;

}

TArray<uint8> UHTTP_UPLOADBPLibrary::FstringToBytes(FString JsonStr)
{
	//FString JsonStr;
	TArray<uint8> content;

	FTCHARToUTF8 EchoStrUtf8(*JsonStr);

	int32 DestLen = EchoStrUtf8.Length();


	content.SetNum(DestLen);
	memcpy(content.GetData(), TCHAR_TO_UTF8(*JsonStr), DestLen);



	return content;
}

FString UHTTP_UPLOADBPLibrary::BytesToFstring(TArray<uint8> content)
{

	//const std::string cstr(reinterpret_cast<const char*>(content.GetData()), content.Num());

	FString frameAsFString = "11";
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *frameAsFString);

	return frameAsFString;
}


void URequerstObj:: GetRQ(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	TArray<uint8> byteArray;

	if (response)
	{
		FString content1, content2 = "";
		int code;
		response->GetHeader(content1);
		
		content2 = response->GetContentAsString();
		code = response->GetResponseCode();
		
		request->GetResponse().Get();

		

		//UE_LOG(LogTemp, Warning, TEXT("Ceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeb : %s"), *content2);
		//UE_LOG(LogTemp, Warning, TEXT("ccccccccccccccccccccccccccbbbbbbbbbbbbb : %d"), code);

		content2.Append(content1);
		byteArray = response->GetContent();

		OnHttpCompletedReturn.Broadcast(bWasSuccessful, content2, byteArray,ID);

		return;
	};

	//response->GetAllHeaders();
	if (bWasSuccessful)
	{
		OnHttpCompletedReturn.Broadcast(bWasSuccessful, "OK_LocalSay", byteArray,ID);
	}
	else
	{
		OnHttpCompletedReturn.Broadcast(bWasSuccessful, "Failed", byteArray,ID);
	}

}

 URequerstObj* UHTTP_UPLOADBPLibrary::PostJson(FString URL, FString JsonString, FString ID)
{
	 URequerstObj* Rbaby = NewObject<URequerstObj>();

	 Rbaby->ID = ID;

	 TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	 HttpRequest->OnProcessRequestComplete().BindUObject(Rbaby, &URequerstObj::GetRQ);
	 HttpRequest->SetVerb("POST");
	 HttpRequest->SetHeader("Content-Type", "application/json");
	 //HttpRequest->SetHeader("Content-Type", "charset=utf-8" );
	 


	 HttpRequest->SetURL(URL);

	 //TArray<uint8> Bytes = FstringToBytes(JsonString);

	

	 HttpRequest->SetContentAsString(JsonString);

	 HttpRequest->ProcessRequest();



	 return Rbaby;
}


 URequerstObj* UHTTP_UPLOADBPLibrary::Get(FString URL,FString ID)
 {
	 URequerstObj* Rbaby = NewObject<URequerstObj>();

	 Rbaby->ID = ID;

	 TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	 

	 HttpRequest->OnProcessRequestComplete().BindUObject(Rbaby, &URequerstObj::GetRQ);

	 HttpRequest->SetURL(URL);
	 
	 HttpRequest->SetVerb("GET");
	 
	


	 HttpRequest->ProcessRequest();


	 return Rbaby;

 }



 URequerstObj* UHTTP_UPLOADBPLibrary::GetParameter(FString URL, TMap<FString, FString> Parameters, FString ID)
 {
	 URequerstObj* Rbaby = NewObject<URequerstObj>();

	 Rbaby->ID = ID;

	 TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();



	 HttpRequest->OnProcessRequestComplete().BindUObject(Rbaby, &URequerstObj::GetRQ);


	 FString AllURL;

	 AllURL = URL +"?";


	 

	 HttpRequest->SetVerb("GET");


	 TArray<FString>Keys;

	 Parameters.GetKeys(Keys);


	 for (size_t i = 0; i < Keys.Num(); i++)
	 {
		 if (i == 0)
		 {
			 AllURL += Keys[i] + "=" + *Parameters.Find(Keys[i]);
		 }
		 else
		 {
			 AllURL += "&" + Keys[i] + "=" + *Parameters.Find(Keys[i]);
		 }


	 }
	 
	 HttpRequest->SetURL(AllURL);
	 HttpRequest->ProcessRequest();


	 return Rbaby;

 }


 URequerstObj* UHTTP_UPLOADBPLibrary::PostParameter(FString URL, TMap<FString, FString> Parameters, FString ID)
 {
	 URequerstObj* Rbaby = NewObject<URequerstObj>();

	 Rbaby->ID = ID;

	 TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	 HttpRequest->OnProcessRequestComplete().BindUObject(Rbaby, &URequerstObj::GetRQ);
	 HttpRequest->SetVerb("POST");
	 HttpRequest->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	 //HttpRequest->SetHeader("Content-Type", "charset=utf-8" );


	 HttpRequest->SetURL(URL);

	 //TArray<uint8> Bytes = FstringToBytes(JsonString);

	 //HttpRequest->SetContent();

	 FString ContentString;

	 TArray<FString>Keys;

	 Parameters.GetKeys(Keys);


	 for (size_t i = 0; i < Keys.Num(); i++)
	 {
		 if (i==0)
		 {
			 ContentString += Keys[i] + "=" + *Parameters.Find(Keys[i]);
		 }
		 else
		 {
			 ContentString += "&"+ Keys[i] + "=" + *Parameters.Find(Keys[i]);
		 }
		 

	 }


	 HttpRequest->SetContentAsString(ContentString);

	 HttpRequest->ProcessRequest();



	 return Rbaby;
 }