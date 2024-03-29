.. BBP documentation master file, created by
   sphinx-quickstart on Thu Sep 28 10:41:17 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to BBP's documentation!
===============================

.. role:: cpp(code)
   :language: cpp

*BBP* is an extensible API in the form of libraries and conventions capable of hosting a minimal operating system in a portable manner.
Currently, BBP is built on top of g++'s libc++, however BBP provides portability by hiding functions such as :cpp:`malloc()` behind a garbage collector,
so that the user ought not to call :cpp:`malloc()` directly.

Although BBP is far from POSIX compliant, it does attempt to follow certain standards such as `ECMA-35 <https://www.ecma-international.org/wp-content/uploads/ECMA-35_6th_edition_december_1994.pdf>`_.
All definitions of conventions created for this project can be found throughout this documentation, for which summaries can be found at :ref:`stub`.

.. _gettingStarted:

Getting Started
---------------
   

.. _installation:

Installing the shell
--------------------
   This is how you install the shell

.. _install:

Installing the source
---------------------
   If you are installing from source, here is how


Contents
--------

.. toctree::
   :maxdepth: 2

   Home <self>
   Links <stub>
   Userspace
   ESA
   EABI
