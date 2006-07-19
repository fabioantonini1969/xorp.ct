#!/bin/sh -e

CONFIG="$(dirname $0)/config"
. $CONFIG

cd ${ROOTDIR}

remote_run()
{
    host=$1
    shift
    ssh ${SSH_FLAGS} -n $host "$*"
}

if [ $# -eq 0 -o $# -gt 2 ] ; then
    echo "usage: $0 <host> [<workdir>]" >/dev/stderr
    echo "executed: $0 $*" >/dev/stderr
    exit 1
fi

HOST=$1
if [ $# -eq 1 ] ; then
    DESTDIR="."
else
    DESTDIR=$2
fi

remote_run $HOST rm -rf "${DESTDIR}/scripts"
remote_run $HOST rm -rf "${DESTDIR}/tmp"
remote_run $HOST rm -rf "${DESTDIR}/xorp"
remote_run $HOST rm -rf "${DESTDIR}/data"

# We make tmp as it forces DESTDIR to be created if it doesn't exist and
# doesn't cause an error if it already does since previous command deletes
# tmp.

remote_run $HOST mkdir -p ${DESTDIR}/tmp

for i in xorp scripts data ; do
    tar cfps - ${i} | ssh ${SSH_FLAGS} $1 cd ${DESTDIR} \&\& tar xfps -
    if [ $? -ne 0 ] ; then
	exit 1
    fi
done