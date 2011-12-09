This program is the client-server complex for measuring data from gpib
devices and (possibly some other measurement devices in the future). It
uses Qt framework to make development process faster and easier.

Server side code is somewhat hardware-specific and it should be
compiled on win32 platform only.

Client side aims to be platform independent. It uses Qwt library for plotting the data.

Client and server communicates via network (tcp and udp) by using custom http-like protocol (language).

The developed program is distributed under terms of GNU GPLv3 License.
Refer to the gnu.org site for the copy of the license agreement
http://www.gnu.org/licenses/gpl.txt

(c) 2011 Sergey Popov (aka azure)
http://www.azure.org.ua/
