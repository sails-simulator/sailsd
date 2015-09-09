# - Try to find GIO 
# Find GIO headers, libraries and the answer to all questions.
#
#  GIO_FOUND               True if GIO got found
#  GIO_INCLUDE_DIRS        Location of GIO headers 
#  GIO_LIBRARIES           List of libraries to use GIO 
#
# Copyright (c) 2008 Bjoern Ricks <bjoern.ricks@googlemail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

INCLUDE( FindPkgConfig )

IF ( GIO_FIND_REQUIRED )
    SET( _pkgconfig_REQUIRED "REQUIRED" )
ELSE( GIO_FIND_REQUIRED )
    SET( _pkgconfig_REQUIRED "" )   
ENDIF ( GIO_FIND_REQUIRED )

IF ( GIO_MIN_VERSION )
    PKG_SEARCH_MODULE( GIO ${_pkgconfig_REQUIRED} gio-2.0>=${GIO_MIN_VERSION} )
ELSE ( GIO_MIN_VERSION )
    PKG_SEARCH_MODULE( GIO ${_pkgconfig_REQUIRED} gio-2.0 )
ENDIF ( GIO_MIN_VERSION )


IF( NOT GIO_FOUND AND NOT PKG_CONFIG_FOUND )
    FIND_PATH( GIO_INCLUDE_DIRS gio/gio.h PATH_SUFFIXES glib-2.0)
    FIND_LIBRARY( GIO_LIBRARIES gio-2.0 )

    # Report results
    IF ( GIO_LIBRARIES AND GIO_INCLUDE_DIRS )   
        SET( GIO_FOUND 1 )
        IF ( NOT GIO_FIND_QUIETLY )
            MESSAGE( STATUS "Found GIO: ${GIO_LIBRARIES}" )
        ENDIF ( NOT GIO_FIND_QUIETLY )
    ELSE ( GIO_LIBRARIES AND GIO_INCLUDE_DIRS ) 
        IF ( GIO_FIND_REQUIRED )
            MESSAGE( SEND_ERROR "Could NOT find GIO" )
        ELSE ( GIO_FIND_REQUIRED )
            IF ( NOT GIO_FIND_QUIETLY )
                MESSAGE( STATUS "Could NOT find GIO" )  
            ENDIF ( NOT GIO_FIND_QUIETLY )
        ENDIF ( GIO_FIND_REQUIRED )
    ENDIF ( GIO_LIBRARIES AND GIO_INCLUDE_DIRS )
ENDIF( NOT GIO_FOUND AND NOT PKG_CONFIG_FOUND )

MARK_AS_ADVANCED( GIO_LIBRARIES GIO_INCLUDE_DIRS )
