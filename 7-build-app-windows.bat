@echo off
@setlocal enableextensions
@cd /d "%~dp0"

rem Get the current directory path
for %%A in ("%~dp0.") do (
    set "currentDir=%%~fA"
)

::echo Clean Project
::call 9-clean-project.bat

echo Create the "release" folder and its contents
mkdir publish_win
mkdir release_win
mkdir build_win

echo Create the "docs" folder and its contents
mkdir docs
cd docs
mkdir coverxygenlibwin
mkdir coverxygentestwin
mkdir coveragereportlibwin
mkdir coveragereporttestwin
mkdir doxygenlibwin
mkdir doxygentestwin
mkdir testresultswin
cd ..

echo Create the "site" folder and its contents
mkdir site

echo Folders are Recreated successfully.

echo Generate Documentation

set STRIP_FROM_PATH=%currentDir%

echo Generate HTML/LATEX/RTF/XML Documentation for Library (No Source Code Only Headers)
call doxygen DoxyfileLibWin

echo Generate HTML/LATEX/RTF/XML Documentation for Unit Tests (Test Sources and Test Data Sets)
call doxygen DoxyfileTestWin

echo Not: coverxygen uses doxygen xml output for coverage

echo Run Documentation Coverage Data Collector for Library (No Source Code Only Headers)
call python -m coverxygen --xml-dir ./docs/doxygenlibwin/xml --src-dir ./ --format lcov --output ./docs/coverxygenlibwin/lcov_doxygen_lib_win.info

echo Run Documentation Coverage Data Collector for Unit Tests (Test Sources and Test Data Sets)
call python -m coverxygen --xml-dir ./docs/doxygentestwin/xml --src-dir ./ --format lcov --output ./docs/coverxygentestwin/lcov_doxygen_test_win.info
rem call python -m coverxygen --xml-dir ./docs/doxygen/xml --src-dir ./ --format lcov --output ./docs/coverxygen/lcov.info --prefix %currentDir%\

rem echo Run lcov genhtml
rem call perl C:\ProgramData\chocolatey\lib\lcov\tools\bin\genhtml --legend --title "Documentation Coverage Report" ./docs/coverxygen/lcov.info -o docs/coverxygen

echo Run Documentation Coverage Report Generator for Library 
call reportgenerator "-title:Cost Calculator Library Documentation Coverage Report (Windows)" "-reports:**/lcov_doxygen_lib_win.info" "-targetdir:docs/coverxygenlibwin" "-reporttypes:Html" "-filefilters:-*.md;-*.xml;-*[generated];-*build*" "-historydir:report_doc_lib_hist_win"
call reportgenerator "-reports:**/lcov_doxygen_lib_win.info" "-targetdir:assets/doccoveragelibwin" "-reporttypes:Badges" "-filefilters:-*.md;-*.xml;-*[generated];-*build*"

echo Run Documentation Coverage Report Generator for Unit Tests 
call reportgenerator "-title:Cost Calculator Library Test Documentation Coverage Report (Windows)" "-reports:**/lcov_doxygen_test_win.info" "-targetdir:docs/coverxygentestwin" "-reporttypes:Html" "-filefilters:-*.md;-*.xml;-*[generated];-*build*" "-historydir:report_doc_test_hist_win"
call reportgenerator "-reports:**/lcov_doxygen_test_win.info" "-targetdir:assets/doccoveragetestwin" "-reporttypes:Badges" "-filefilters:-*.md;-*.xml;-*[generated];-*build*"

echo Testing Application with Coverage
echo Configure CMAKE
call cmake -B build_win -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX:PATH=publish_win
echo Build CMAKE Debug/Release
call cmake --build build_win --config Debug -j4
call cmake --build build_win --config Release -j4
rem call cmake --install build_win --strip
start "Install Debug" cmake --install build_win --config Debug --strip
start "Install Release" cmake --install build_win --config Release --strip
echo Test CMAKE
cd build_win
:: Test are already run with OpenCppCoverage...
::call ctest -C Debug -j1 --output-on-failure --output-log test_results_windows.log
call ctest -C Debug -j1 --output-junit testResults_windows.xml --output-log test_results_windows.log
call junit2html testResults_windows.xml testResults_windows.html
call copy testResults_windows.html "..\docs\testresultswin\index.html"
cd ..

echo Generate Test Coverage Data for User Authentication
call OpenCppCoverage.exe --export_type=binary:userauthentication_tests_unit_win.cov --sources src\userauthentication\src --sources src\userauthentication\header --sources src\tests\userauthentication -- build_win\build\Debug\userauthentication_tests.exe

echo Generate Test Coverage Data for Ingredient Management
call OpenCppCoverage.exe --export_type=binary:ingredientmanagement_tests_unit_win.cov --sources src\ingredientmanagement\src --sources src\ingredientmanagement\header --sources src\tests\ingredientmanagement -- build_win\build\Debug\ingredientmanagement_tests.exe

echo Generate Test Coverage Data for Recipe Costing
call OpenCppCoverage.exe --export_type=binary:recipecosting_tests_unit_win.cov --sources src\recipecosting\src --sources src\recipecosting\header --sources src\tests\recipecosting -- build_win\build\Debug\recipecosting_tests.exe

echo Generate Test Coverage Data for Price Adjustment
call OpenCppCoverage.exe --export_type=binary:priceadjustment_tests_unit_win.cov --sources src\priceadjustment\src --sources src\priceadjustment\header --sources src\tests\priceadjustment -- build_win\build\Debug\priceadjustment_tests.exe

echo Generate Test Coverage Data for Budget Planner
call OpenCppCoverage.exe --export_type=binary:budgetplanner_tests_unit_win.cov --sources src\budgetplanner\src --sources src\budgetplanner\header --sources src\tests\budgetplanner -- build_win\build\Debug\budgetplanner_tests.exe

echo Generate Test Coverage Data for userauthentication App and Combine Results
call OpenCppCoverage.exe --input_coverage=userauthentication_tests_unit_win.cov --input_coverage=ingredientmanagement_tests_unit_win.cov --input_coverage=recipecosting_tests_unit_win.cov --input_coverage=priceadjustment_tests_unit_win.cov --input_coverage=budgetplanner_tests_unit_win.cov --export_type=cobertura:costcalculatorapp_unit_win_cobertura.xml --sources src\userauthentication\src --sources src\userauthentication\header --sources src\ingredientmanagement\src --sources src\ingredientmanagement\header --sources src\recipecosting\src --sources src\recipecosting\header --sources src\priceadjustment\src --sources src\priceadjustment\header --sources src\budgetplanner\src --sources src\budgetplanner\header --sources src\costcalculatorapp\src --sources src\costcalculatorapp\header --sources src\tests\userauthentication --sources src\tests\ingredientmanagement --sources src\tests\recipecosting --sources src\tests\priceadjustment --sources src\tests\budgetplanner -- build_win\build\Debug\costcalculatorapp.exe

echo Generate Unit Test Coverage Report
call reportgenerator "-title:Cost Calculator Library Unit Test Coverage Report (Windows)" "-targetdir:docs/coveragereportlibwin" "-reporttypes:Html" "-reports:**/costcalculatorapp_unit_win_cobertura.xml" "-sourcedirs:src/userauthentication/src;src/userauthentication/header;src/ingredientmanagement/src;src/ingredientmanagement/header;src/recipecosting/src;src/recipecosting/header;src/priceadjustment/src;src/priceadjustment/header;src/budgetplanner/src;src/budgetplanner/header;src/costcalculatorapp/src;src/costcalculatorapp/header;src/tests/userauthentication;src/tests/ingredientmanagement;src/tests/recipecosting;src/tests/priceadjustment;src/tests/budgetplanner" "-filefilters:-*minkernel\*;-*gtest*;-*a\_work\*;-*gtest-*;-*gtest.cc;-*gtest.h;-*build*" "-historydir:report_test_hist_win"
call reportgenerator "-targetdir:assets/codecoveragelibwin" "-reporttypes:Badges" "-reports:**/costcalculatorapp_unit_win_cobertura.xml" "-sourcedirs:src/userauthentication/src;src/userauthentication/header;src/ingredientmanagement/src;src/ingredientmanagement/header;src/recipecosting/src;src/recipecosting/header;src/priceadjustment/src;src/priceadjustment/header;src/budgetplanner/src;src/budgetplanner/header;src/costcalculatorapp/src;src/costcalculatorapp/header;src/tests/userauthentication;src/tests/ingredientmanagement;src/tests/recipecosting;src/tests/priceadjustment;src/tests/budgetplanner" "-filefilters:-*minkernel\*;-*gtest*;-*a\_work\*;-*gtest-*;-*gtest.cc;-*gtest.h;-*build*"

echo Copy the "assets" folder and its contents to "docs" recursively
call robocopy assets "docs\assets" /E

echo Copy the "README.md" file to "docs\index.md"
call copy README.md "docs\index.md"

echo Files and folders copied successfully.

:: echo Generate Webpage
:: call mkdocs build

rem echo Publish Linux Binaries
rem call dotnet publish -c Release -r linux-x64 --self-contained true -o publish/linux

rem echo Publish MacOS Binaries
rem call dotnet publish -c Release -r osx-x64 --self-contained true -o publish/macos

rem echo Publish Windows Binaries
rem call dotnet publish -c Release -r win-x64 --self-contained true -o publish/windows

rem echo Package Linux Binaries
rem call tar -czvf release/linux-binaries.tar.gz -C publish/linux .

rem echo Package MacOS Binaries
rem call tar -czvf release/macos-binaries.tar.gz -C publish/macos .

echo Package Publish Windows Binaries
tar -czvf release_win\windows-publish-binaries.tar.gz -C publish_win .

echo Package Publish Windows Binaries
call robocopy src\userauthentication\header "build_win\build\Release" /E
call robocopy src\ingredientmanagement\header "build_win\build\Release" /E
call robocopy src\recipecosting\header "build_win\build\Release" /E
call robocopy src\priceadjustment\header "build_win\build\Release" /E
call robocopy src\budgetplanner\header "build_win\build\Release" /E
call robocopy src\costcalculatorapp\header "build_win\build\Release" /E
tar -czvf release_win\windows-release-binaries.tar.gz -C build_win\build\Release .

echo Package Publish Debug Windows Binaries
call robocopy src\userauthentication\header "build_win\build\Debug" /E
call robocopy src\ingredientmanagement\header "build_win\build\Debug" /E
call robocopy src\recipecosting\header "build_win\build\Debug" /E
call robocopy src\priceadjustment\header "build_win\build\Debug" /E
call robocopy src\budgetplanner\header "build_win\build\Debug" /E
call robocopy src\costcalculatorapp\header "build_win\build\Debug" /E
tar -czvf release_win\windows-debug-binaries.tar.gz -C build_win\build\Debug .

echo Package Publish Test Coverage Report
tar -czvf release_win\windows-test-coverage-report.tar.gz -C docs\coveragereportlibwin .

echo Package Publish Library Doc Coverage Report
tar -czvf release_win\windows-lib-doc-coverage-report.tar.gz -C docs\coverxygenlibwin .

echo Package Publish Unit Test Doc Coverage Report
tar -czvf release_win\windows-test-doc-coverage-report.tar.gz -C docs\coverxygentestwin .

echo Package Publish Library Documentation
tar -czvf release_win\windows-doxygen-lib-documentation.tar.gz -C docs\doxygenlibwin .

echo Package Publish Unit Test Documentation
tar -czvf release_win\windows-doxygen-test-documentation.tar.gz -C docs\doxygentestwin .

echo Package Publish Test Results Report
tar -czvf release_win\windows-test-results-report.tar.gz -C docs\testresultswin .

echo ....................
echo Operation Completed!
echo ....................
pause