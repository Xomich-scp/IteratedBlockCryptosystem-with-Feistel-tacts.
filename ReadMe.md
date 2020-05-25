Visual Studio 2019 project.

Build

E.g. run

Crypt in ECB mode
01.exe open.txt key.txt encrypted.txt

Decrypt in ECB mode
02.exe encrypted.txt key.txt decrypted.txt

Additional tool to count mistake's spreading time 
NonessentialVar.exe

Modified bruteforce. Uses simple simple correlation g2Fg1(x) = F(x)
BruteForce.exe open.txt key.txt encrypted.txt

Crypt/decrypt in OFB mode

lab_04_OFB.exe -enc open.txt key.txt iv.txt OFB_encrypted.txt
lab_04_OFB.exe -dec OFB_encrypted.txt key.txt iv.txt OFB_decrypted.txt

Group Analys.
lab_05_GroupAnalysis.exe

Additional tool, used to generate IV
Synch_gen.exe

Generate table of linear and differential substution. Example of input file - {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
06_and_07.exe input.txt
