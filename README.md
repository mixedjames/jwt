James' Windows Toolkit
======================

JWT is a C++ library that aims to make using the Windows API simpler and less verbose.

Legal stuff
-----------
JWT is licensed under the LGPL 2.1.
Copyright &copy; 2017 James Heggie

Building JWT
------------
I assume you are using Visual Studio 2015. I have not idea if it builds on earlier versions.

%JWT_DIR% is a pseudo-environment variable pointing to wherever you unpacked the JWT library to.

1. Define an environment variable BOOST_PATH to point to wherever you have installed Boost. I build against 1.63.0 currently. I suspect it will work with most reasonably up-to-date versions.
2. Navigate to %JWT_DIR%\vc2015\JWT
3. Open JWT.sln
4. Build the configurations that you require (x86 & x64 debug & release versions selectable)
5. Configure your new project as follows:
  - Link to JWT.x86.lib (or whichever version you wanted)
  - Add %JWT_DIR% to your include path

Basic Usage
-----------

TBA.