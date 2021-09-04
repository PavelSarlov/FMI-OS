#!/bin/bash

if [ $# -ne 2 ]; then
    echo "incorrect number of args" >&2
    exit 1
fi

repo="$1"
package="$2"

#1 create package archive
tar -cJf package_archive.tar.xz "${package}/tree"

#1.1 get archive checksum
checksum="$( sha256sum package_archive.tar.xz | cut -d ' ' -f 1 )"

#1.2 rename package to a more useful name
mv package_archive.tar.xz "${checksum}.tar.xz"

#2 get package version
version="$( cat "${package}"/version )"

#3 verify whether package exists in repo
if ( egrep -q "^${package}-${version} ${checksum}$" "${repo}"/db ); then
    # package already exists in repo
    exit 0
fi

if ( egrep -q "^${package}-${version}" "${repo}"/db); then
    # different checksum, must be updated
    
    # get old checksum
    old_checksum="$( egrep "^${package}-${version} " "${repo}"/db | cut -d ' ' -f 2)"
    # remove old archive for that version.
    rm "${repo}/packages/${checksum}.tar.xz"
    # add new archive for that version
    mv "${checksum}.tar.xz" "${repo}/packages/${old_checksum}.tar.xz"
    # update db
    sed -i "s|^${package}-${version} ${old_checksum}$|${package}-${version} ${checksum}" "${repo}/db"
else
    # add info for the package in db
    echo "${package}-${version} ${checksum}" >>"${repo}/db"

    # add archive
    mv "${checksum}.tar.xz" "${repo}/packages/${checksum}.tar.xz"

    # make sure repo/db file is still sorted.
    sort "${repo}/db" >file
    mv file "${repo}/db"
fi

exit 0
