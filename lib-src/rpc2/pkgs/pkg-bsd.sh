#!/bin/sh

if [ $# != 2 ]
then
  echo "Usage $0 <version> [NetBSD|FreeBSD]"
  exit
fi

VERSION=$1
BSD=$2

function MakePortsTree () {
    ver=$1
    bsd=$2

    # make ports stuff
    mkdir devel
    MakeSubTree ${ver} ${bsd}

#    chown -R root.root devel
}

function MakeSubTree () {
    ver=$1
    bsd=$2

    mkdir devel/rpc2
    MakeMakefile ${ver} devel/rpc2/Makefile ${bsd}

    mkdir devel/rpc2/files

    if [ x${bsd} = xNetBSD ]
    then
	cat > /tmp/mf << EOF
\$NetBSD\$

EOF
    else
	cp /dev/null /tmp/mf
    fi

    for dir in . .. ../.. ; do
      if [ -f ${dir}/rpc2-${ver}.tar.gz ] ; then
	( cd ${dir} ; md5sum rpc2-${ver}.tar.gz | awk '{printf("MD5 (%s) = %s\n",$2,$1)}' >> /tmp/mf )
	mv /tmp/mf devel/rpc2/files/md5
	break;
      fi
    done
    rm -f /tmp/mf

    mkdir devel/rpc2/pkg

    cat > devel/rpc2/pkg/COMMENT << EOF
RPC2 library
EOF
    cat > devel/rpc2/pkg/DESCR << EOF
The RPC2 library. The RPC2 library provides interprocess communication for the
Coda distributed filesystem.
EOF

    MakePLIST devel/rpc2/pkg/PLIST ${bsd}
}

function MakeMakefile () {
  version=$1
  dest=$2
  if [ x$3 = xNetBSD ]
  then
    REMOVE=FreeBSD
    KEEP=NetBSD
  else
    REMOVE=NetBSD
    KEEP=FreeBSD
  fi

  cat > /tmp/mf << EOF
@NetBSD # \$NetBSD\$
@NetBSD #
@FreeBSD # New ports collection makefile for:	rpc2
@FreeBSD # Version required:			@VERSION@
@FreeBSD # Date created:				@DATE@
@FreeBSD # Whom:					@USER@
@FreeBSD # \$FreeBSD\$
@FreeBSD #

DISTNAME=	rpc2-@VERSION@
PKGNAME=	rpc2-@VERSION@
CATEGORIES=	devel
MASTER_SITES=	ftp://ftp.coda.cs.cmu.edu/pub/rpc2/src/
EXTRACT_SUFX=	.tar.gz

MAINTAINER=	coda@cs.cmu.edu
@NetBSD HOMEPAGE=	http://www.coda.cs.cmu.edu/
@FreeBSD LIB_DEPENDS=	lwp.1:\${PORTSDIR}/devel/lwp

@NetBSD ONLY_FOR_ARCHS=	arm32 i386 ns32k
@NetBSD 
@NetBSD LICENSE=	LGPL
@NetBSD 
ALL_TARGET=	all
INSTALL_TARGET=	install

GNU_CONFIGURE=	yes
USE_GMAKE=	yes
USE_LIBTOOL=	yes

@NetBSD .include "../../mk/bsd.pkg.mk"
@FreeBSD .include <bsd.port.mk>
EOF

    cat /tmp/mf | sed -e "s/@VERSION@/${version}/" | \
		  sed -e "s/@DATE@/`date`/" | \
		  sed -e "s/@USER@/${USER}/" | \
		  sed -e "/^@${REMOVE} .*$/d" | \
		  sed -e "s/^@${KEEP} \(.*\)$/\1/" > ${dest}
    rm /tmp/mf
}

function MakePLIST () {
    dst=$1
    bsd=$2

    if [ x${bsd} = xNetBSD ]
    then
	cat > ${dst} << EOF
@comment \$NetBSD\$
lib/libfail.so.1.0
lib/librpc2.so.1.0
lib/libse.so.1.0
EOF
    else
	cat > ${dst} << EOF
lib/libfail.so
lib/libfail.so.1
lib/librpc2.so
lib/librpc2.so.1
lib/libse.so
lib/libse.so.1
EOF
    fi

    cat >> ${dst} << EOF
bin/filcon
bin/rp2gen
lib/libfail.a
lib/libfail.la
lib/librpc2.a
lib/librpc2.la
lib/libse.a
lib/libse.la
include/rpc2/errors.h
include/rpc2/errorsdefs.h
include/rpc2/fail.h
include/rpc2/fcon.h
include/rpc2/multi.h
include/rpc2/rpc2.h
include/rpc2/se.h
include/rpc2/sftp.h
EOF
}

MakePortsTree $VERSION $BSD
tar -czf pkg-rpc2-$VERSION-$BSD.tgz devel
rm -rf devel

