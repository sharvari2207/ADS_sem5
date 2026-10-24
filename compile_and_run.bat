@echo off
echo =======================================================
echo Compiling Blood Bank ADS Project (C++14)...
echo =======================================================

echo [1/4] Compiling Standalone Feature 1 (Dynamic Max-Heap)...
g++ -std=c++14 -O2 Feature1_UrgencyMaxHeap.cpp -o f1.exe
if %ERRORLEVEL% NEQ 0 ( echo Failed to compile Feature 1! & pause & exit /b %ERRORLEVEL% )

echo [2/4] Compiling Standalone Feature 2 (Edmonds-Karp Max-Flow)...
g++ -std=c++14 -O2 Feature2_MaxLifeSavingEdmondsKarp.cpp -o f2.exe
if %ERRORLEVEL% NEQ 0 ( echo Failed to compile Feature 2! & pause & exit /b %ERRORLEVEL% )

echo [3/4] Compiling Standalone Feature 3 (Trie Directory Autocomplete)...
g++ -std=c++14 -O2 Feature3_DirectoryTrie.cpp -o f3.exe
if %ERRORLEVEL% NEQ 0 ( echo Failed to compile Feature 3! & pause & exit /b %ERRORLEVEL% )

echo [4/4] Compiling Integrated Blood Bank System...
g++ -std=c++14 -O2 main.cpp BloodRequestHeap.cpp MaxLifeSavingEdmondsKarp.cpp DirectoryTrie.cpp BloodBankSystem.cpp -o blood_bank_system.exe
if %ERRORLEVEL% NEQ 0 ( echo Failed to compile Integrated System! & pause & exit /b %ERRORLEVEL% )

echo.
echo =======================================================
echo Compilation Successful!
echo Running Integrated System...
echo =======================================================
.\blood_bank_system.exe
pause

