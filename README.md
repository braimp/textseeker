What is TextSeeker
==================

This is a simple application to search for and find content in a directory of text files written in C++ using [Qt](https://www.qt.io).  It includes a simple text viewer that lets you scroll thru content that is found and search it for keywords with the F3 key.  The nearest equivalent of this application is probably the original gnome search tool. This application is not going to change the world, but it did help me clarify how I wish to generally develop desktop applications with Qt and what standard practices I want to introduce in that process going forward.

History
=======

TextSeeker first appeared as "coastal search" in coastal-qt in 2011.  Back then I was experimenting with cross-platform Qt desktop applications and cmake.  This is a stand-alone derived version of coastal search, to make it easier to separately maintain, and to experiment with what I feel are good practices for using Qt and QtCreator for cross-platform development.  TextSeeker is licensed using the GNU GPL, Version 3 or later, as was it's immediate predecessor in coastal-qt, TextSearch.

Install
=======

On Mac I presume TextSeeker can be delivered as a fully stand-alone bundled application, complete with required Qt runtime libraries.  This may then be bundled together and distributed with other desktop apps in a common .dmg installer.  This project can produce a stand-alone application that can be used and redistributed directly and separately if so desired.  This package can be setup to build thru macports or homebrew as well.

On Windows I produce a deploy archive directory which includes Qt runtime dll libraries.  The deploy archive directory from this and related applications are then gathered together, removing duplication of runtime dll's, and will normally be delivered in a common .exe installer using inno setup.  The archive directory from this project could also be used to create a stand-alone windows binary distribution of just this application.

For generic Unix (GNU/Linux, BSD systems, etc), the publish target provides a clean source tarball.  **qmake** can be directly ran to configure the build from a source tarball, and **make** to produce binaries, without needing qtcreator.  A generic **make install** can then be used to install the result.  I also may produce a generated .spec file from qmake which can then be used to produce a .rpm package which can then be loaded on a build system like obs.  For BSD systems I recommend creating a "ports" file.

Support
=======

At the moment I do not have infrastructure to offer support.  I have published this, and perhaps as of this writing, other packages, mostly to experiment with and facilitate best practices for future work.  I do maintain an email address for public contact for all similarly published Tycho Softworks projects as [tychosoft@gmail.com](mailto://tychosoft@gmail.com).  Merge requests may be accepted when I happen to have a chance and connectivity to do so.  I also will be using the gitlab [textseeker](https://gitlab.com/tychosoft/textseeker) issue tracker for bug reporting and project management.
