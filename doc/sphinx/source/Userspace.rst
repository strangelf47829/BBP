The Userspace
=============

The userspace is where 

.. _creatingUserspaceApplications:

Creating Userspace Applications
-------------------------------

    For documentation on creating userspace applications, please refer to :ref:`stub`

.. _userspaceApplicationMechanisms:

Userspace Applications
----------------------

    BBP can load and execute applications from a persistent storage device such as an SDMMC.
    
    Userspace applications are stored as ELF binaries (specific details on the parsing and format of ELF binaries can be found :ref:`here <stub>`).
    BBP contains various APIs to read, parse, and build ELF applications from memory or disk, without using much heap memory.
    
    Once userspace applications have been loaded into memory, they can be executed through the :ref:`virtual processor <stub>`.
    the virtual processor is an API that can execute :ref:`x86 <stub>` binaries, including multi-threading and signal handling.

    Several services are available to userspace applications through :ref:`x86 <stub>` syscalls such as garbage collection, file handling, IO services, and more.
    For a complete list of syscalls, please refer to :ref:`stub`.

.. _userspaceApplicationMotivations:

Motivations
-----------

    The motivations are ...

.. _userspaceLinkingProcess:

Userspace linking process
-------------------------

    Userspace applications are linked at runtime using standard ELF linking procedures.

.. _userspaceABI:

The userspace ABI
-----------------

    .. note::
        This is a high level overview of the ABI. For a more detailed and complete documentation of the ABI, please refer to :ref:`stub`