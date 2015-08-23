# Linux-SCSI-Driver-MiddleWare-
Implementation of Linux SCSI Driver using SCSI Generic Device (sg)

-------------------------
Listing the SCSI devices
-------------------------

First the command of lsscsi can list the scsi drives that connected to the linux host with their idVendor, idProduct, idProductVersion and the accessing file through /dev/.

You must install the sg library to your linux.

The directory /sys/bus/usb/devices hold all the files that dedicated to your connected usb devices, you can see this via terminal through the command usb-devices.
Also a complete knowledge of the connected device can be found through the path /proc/scsi/usb-storage that contains all information about the device like: connected bus, vendor ID, product ID, product version, Serial number and type of endpoints.

Another way to list the devices using sg library is by using the sg_map command, but take good care to be sudo before using any command or file related to sg library.
To send a command via scsi you must first open the device file from /dev location using the open()  function then write the buffer you are seaking to send via ioctl() function.

here I am using an alternative using of sg SCSI driver used in linux by writing Parameters in strings from the terminal tool.

the interface method with GUIs is through pipeline with the new application.

below si the illustration about the total problem of using SCSI communication via GUIs in Linux Systems.

------------
Introduction
-------------

Every connected device act in linux like a file, so if you read data from device you actually read from file and the same scenario with writing operation. Also, the reading and writing operation is depends on the user permissions to read or write on this file.

when an application is owned by a root user, and has the s flag permission can run easily with double click with the access rights of the application owner.

But, According to GTK+, the thing that responsible fpr making Graphical User Interface didn't accept the s flag permission.

so, every time you use this application, you must take it to terminal , write "sudo" before it then write the password to start executing.

------------
work around
------------

go to the sudoers file, and write this line to accept running without password for this applications.

---------
Solution
---------

divide the application into two application, one with GUI and another without GUI, "normal C/C++ project".

the application without GUI can be owned by root and take the s flag permission and takes arguments from command line.

the GUI application takes no permissions except the execution permission, establish a pipe with the other application, write parameters to it and take the output.
