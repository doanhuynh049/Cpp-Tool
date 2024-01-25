======================================================================
Product Name:    TIBCO SmartSockets(R)
Release Version: 6.8.2
Release Date:    April 2009

======================================================================
Introduction

  TIBCO SmartSockets is a message-oriented middleware product that
  enables programs to communicate quickly, reliably, and securely
  across:

     * local area networks (LANs)
     * wide area networks (WANs)
     * the Internet

  TIBCO SmartSockets takes care of network interfaces, guarantees
  delivery of messages, handles communications protocols, and directs
  recovery after system or network problems. This enables you to focus
  on higher-level requirements rather than the underlying complexities
  of the network.

======================================================================
Critical Security Upgrade

  Release 6.8.2 addresses a critical security vulnerability. This
  vulnerability affects the following components of SmartSockets:

     * TIBCO SmartSockets RTserver
     * All applications using the TIBCO SmartSockets API to send or
       receive messages

  Impact of this vulnerability could include remote execution of
  arbitrary code, information disclosure, and denial of service.

  We strongly recommend that all customers install this upgrade
  release on all hardware. Additionally, customers using TIBCO
  Enterprise Message Service with the SmartSockets bridge should
  upgrade to software release 5.1.2, which dynamically loads the
  SmartSockets libraries.

======================================================================
Supported Platforms

  Sun SPARC Platforms
     Any Sun Microsystems SPARC family computer, including all
     SPARCstations, SPARCservers, and Sun Ultras, is supported if it
     is running Sun Solaris Version 2.7, 2.8, 2.9 or 2.10.
     Hardware must support 64-bit to run the 64-bit version of
     SmartSockets.

  Hewlett-Packard Platforms
     Any Hewlett-Packard 9000/7xx and binary-compatible 9000/8xx
     computer running HP-UX Version 11.11 (or later) is supported.
     Hardware must support 64-bit to run the 64-bit version of
     SmartSockets.

  x86 Platforms
     Any IBM PC compatible computer equipped with an x86 processor is
     supported if it is running one of the following operating
     systems:
        * Microsoft Windows XP
        * Windows 2000 with Service Pack 1, the Professional or Server
          editions
        * Windows Server 2003
        * Sun Solaris 10
        * Linux with these components:
           * kernel 2.4.21 and up, or 2.6.x
           * gcc 3.2 and later
           * glibc 2.3.2 and up
     Hardware and operating system software must support 64-bit to run
     the 64-bit version of SmartSockets.

   Intel Itanium (IA64) Platforms
     Any IA64 platform with either of these operating environments:
        * HP-UX 11.23 (or later).
        * Linux with these components:
           * kernel 2.4.21 and up
           * gcc 3.2 and later
           * glibc 2.3.2 and up
     Linux Itanium is deprecated. If you are a current user of this
     platform and require future support, please file a service
     request with TIBCO Product Support asking TIBCO SmartSockets
     Product Management to follow up with you to discuss your
     requirements.

   Tru64 Platforms
     Any Hewlett-Packard AlphaServer running Tru64 UNIX 4.0G or
     higher, or 5.0.
     This platform is deprecated. If you are a current user of this
     platform and require future support, please file a service
     request with TIBCO Product Support asking TIBCO SmartSockets
     Product Management to follow up with you to discuss your
     requirements.

   OpenVMS
     Any Hewlett-Packard AlphaServer running OpenVMS 7.3-2 or higher.

   AIX Platforms
     Any IBM RS/6000 system running AIX 5.1 and up.
     Hardware must support 64-bit to run the 64-bit version of
     SmartSockets.

   IRIX Platforms
     Any SGI IRIS running IRIX 6.5.
     The 64-bit version of SmartSockets is not available on IRIX.
     This platform is deprecated. If you are a current user of this
     platform and require future support, please file a service
     request with TIBCO Product Support asking TIBCO SmartSockets
     Product Management to follow up with you to discuss your
     requirements.

   HP Nonstop Server
     Tandem OSS G06.24 or higher.
     This platform is deprecated. If you are a current user of this
     platform and require future support, please file a service
     request with TIBCO Product Support asking TIBCO SmartSockets
     Product Management to follow up with you to discuss your
     requirements.

   Other Platforms
     This list of supported platforms is accurate as of the date of
     the 6.8.2 SmartSockets release. Contact TIBCO Product Support
     for an up-to-date list of supported platforms, or check the TIBCO
     website at http://www.tibco.com.

======================================================================
Software Requirements

  Java Requirements
      To use the SmartSockets Java libraries and APIs, you must have
      the Java2 Software Developer Kit (J2SDK) version 1.4.2 or later
      installed on your system. You can download the J2SDK from the
      Sun Microsystems web site, http://java.sun.com/j2se.

======================================================================
Platform-specific Notes

The UNIX and Windows chapters of the TIBCO SmartSockets Installation
Guide contain notes for specific UNIX and Windows platforms.

Windows
  The RTHOME, RTARCH and PATH environment variables are updated after
  TIBCO SmartSockets is installed. However, during uninstallation,
  TIBCO SmartSockets specific components are not removed from the
  PATH variable. RTHOME and RTARCH are removed.

Solaris 8 and SmartSockets Multicast
  There is a known bug in Sun Solaris 2.8 that prevents running
  multiple multicast receivers simultaneously in the same multicast
  group on the same host. If this is attempted, multicast data will
  only be received by the first of the clients that successfully
  connects. Sun has a patch available that addresses this issue.
  Patch:    109742-04
  Keywords: multicast IPv4 IPv6 icmp multipathing mipagent AAA
            reverse tunneling
  Synopsis: SunOS 5.8: /kernel/drv/icmp patch

HP/UX 11.x
  The following C++ run-time library patches are required to avoid
  runtime incompatibility errors:
  PHSS_22543 and PHSS_22868
  PHSS_22543 fixes an issue with libstd_v2 to establish compatibility
  with the definition of mbstate_t (multibyte support) in the HP/UX
  11.11 runtime (libc). Every site that installs this patch must also
  install the header file patch PHSS_22868.

IRIS
  The SmartSockets libraries and executables are compiled for the
  MIPS4 processor. Currently the MIPS3 processor is not supported by
  the SmartSockets product.

HP Nonstop Server
  The SmartSockets Basic Security feature and the SmartSockets Message
  Compression feature are currently not supported in this release. If
  these features are of critical interest please contact TIBCO
  support.

OpenVMS
  SmartSockets on OpenVMS does not include support for message
  compression or the basic security manager. The RTserver is single
  threaded. The Local and Decnet network protocols are no longer
  supported.

======================================================================
TIBCO Product Support

  Check the TIBCO Product Support web site at
  https://support.tibco.com for product information that was not
  available at release time. Entry to this site requires a username
  and password. If you do not have one, you can request one. You must
  have a valid maintenance or support contract to use this site.

======================================================================
Documentation

  Access to the product documentation set in HTML and PDF

    Before installation, on the product CDROM in the doc folder at
    root level, or from the product download site.

    After installation, from the Start menu on Microsoft Windows
    platforms, or from the product's installation doc directory on
    disk (if documentation has been installed with the product).

  Release Notes and Installation Guide

    See the TIBCO SmartSockets Release Notes for information about new
    features, migrating from an earlier release to this release, and
    closed and known issues.

    See the TIBCO SmartSockets Installation Guide for detailed
    installation instructions for each supported platform.

======================================================================
Copyright

Copyright (C) 1991-2009 TIBCO Software Inc. ALL RIGHTS RESERVED.
TIBCO Software Inc. Confidential Information
