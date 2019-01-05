============================
Release Testing
============================
Last Update: 2012-06-03

This file describes the testing procedures that should be done when releasing 
Rocs. The test procudure starts with a simple checklist (that can be skipped
by beta testers) and then focuses on testing the functional properties
of the application

===============================================================================
Release Checklist
===============================================================================

-------------------------------------------------------------------------------
 Release Information
-------------------------------------------------------------------------------
* Application version is bumped
* About Dialog is updated
* Application information updated at edu.kde.org

-------------------------------------------------------------------------------
 Static Code Checkers
-------------------------------------------------------------------------------
* All (important) Krazy issues are fixed:
  http://www.englishbreakfastnetwork.org/krazy/reports/kde-4.x/kdeedu/rocs/index.html
* CppCheck issues are fixed:
  http://build.kde.org/view/KDEEDU/job/rocs_master/cppcheckResult

-------------------------------------------------------------------------------
 Unit Tests
-------------------------------------------------------------------------------
* Rocs builds with KDE4_BUILD_TESTS 
* All unit test pass:
  http://build.kde.org/view/KDEEDU/job/rocs_master/lastCompletedBuild/testReport/

-------------------------------------------------------------------------------
 Documentation
-------------------------------------------------------------------------------
* Handbook builds: KDE4_ENABLE_HTMLHANDBOOK
* Handbook is updated

-------------------------------------------------------------------------------
 Website and Release Information
-------------------------------------------------------------------------------
* edu.kde.org is updated
* features are correctly reported at KDE Feature List


===============================================================================
Beta Test Instructions
===============================================================================

///////////////////////////////////////////////////////////////////////////////
Name of tester:
Date:
OS Version:
Qt Version:
KDE Version:
///////////////////////////////////////////////////////////////////////////////

Before starting with the tests you should run the unit test to check whether
your installation is fine.

===============================================================================
Tutorial Tests / Documentation
===============================================================================

-------------------------------------------------------------------------------
 T001 - Tutorial
-------------------------------------------------------------------------------
Testprocedure:
1) Proceed steps as described in the Rocs tutorial in the handbook.

-------------------------------------------------------------------------------
 T002 - Compare UI elements from handbook with Rocs UI
-------------------------------------------------------------------------------
Testprocedure:
1) Look up the User Interface Section in the handbook
2) Compare images and names of UI elements


===============================================================================
Projects
===============================================================================

-------------------------------------------------------------------------------
 P001 - Projectfiles
-------------------------------------------------------------------------------
Testprocedure:
1) Create two data elements and connect them (data structure shall not be empty).
2) Write "test" into the script file.
3) Save project.
4) Close Rocs.
5) Open Rocs.
6) Load project.

-------------------------------------------------------------------------------
 P002 - Projectfiles with directory change
-------------------------------------------------------------------------------
Testprocedure:
1) Create two data elements and connect them (data structure shall not be empty).
2) Write "test" into the script file.
3) Save project to folder foo.
4) Close Rocs.
5) Rename folder foo to baa.
6) Open Rocs.
7) Load project.

-------------------------------------------------------------------------------
 P003 - Export/Import (simple)
-------------------------------------------------------------------------------
Testprocedure:
1) Create two data elements and connect them (data structure shall not be empty).
2) Write "test" into the script file.
3) Export project as archive.
4) Close Rocs.
5) Open Rocs.
6) Import archive.

-------------------------------------------------------------------------------
 P004 - Export/Import with directory change
-------------------------------------------------------------------------------
Testprocedure:
1) Create two data elements and connect them (data structure shall not be empty).
2) Write "test" into the script file.
3) Export project as archive.
4) Close Rocs.
5) Move archive file to another folder.
6) Open Rocs.
7) Import project.


===============================================================================
Create and Modify Data Structures
===============================================================================


===============================================================================
Script Engine
===============================================================================


===============================================================================
Script Execution
===============================================================================

