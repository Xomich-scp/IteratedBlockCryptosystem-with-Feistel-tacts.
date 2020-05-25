Configure solution

Add in solution folowwing projects

File->Create->Project->Visual C++->Classic application Windows->Static Library

    Project->Options->C/C++->Precomplier headers->No use precompilier headers

    You can delete files: stdafx.cpp, stdafx.h, targetver.h

    Add files and code

File->Create->Project->Visual C++->Empty project

    Refernces->Add refernce->Your project->OK

    Project->Options->C/C++->General->Additional include file directories-> $(SolutionDir)Name_your_project_library

    Add files and code

	For debug: Project->Options->Debug->Working directory (default - $(ProjectDir))->..\Debug

Build

E.g. run

01.exe open.txt key.txt encrypted.txt
02.exe encrypted.txt key.txt decrypted.txt
fc /b open.txt decrypted.txt

BruteForce.exe open.txt key.txt encrypted.txt
02.exe encrypted.txt key_founded.txt decrypted.txt
fc /b open.txt decrypted.txt

NonessentialVar.exe

lab_03_BruteForce.exe open.txt key.txt encrypted.txt
lab_02_02_Decrypt.exe encrypted.txt key_founded.txt decrypted.txt
fc /b open.txt decrypted.txt

lab_04_OFB.exe -enc open.txt key.txt iv.txt OFB_encrypted.txt
lab_04_OFB.exe -dec OFB_encrypted.txt key.txt iv.txt OFB_decrypted.txt
fc /b open.txt OFB_decrypted.txt

lab_05_GroupAnalysis.exe
