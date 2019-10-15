#!/bin/bash

IMAGE=debian:oldstable
BUILDDEPS=(cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev)
PKGNAME=ultimatetapankaikki
PKGVERSION=3.21+sp2
PKGDEPS=(libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-net-2.0-0)

function join_by {
    local IFS="$1"
    shift
    echo "$*"
}

cat >build-in-docker.bash <<EOF
set -euo pipefail

echo "Installing fpm..."
apt-get update
apt-get -y install \
    ruby \
    ruby-dev \
    rubygems \
    build-essential
gem install --no-ri --no-rdoc fpm

echo "Installing build deps..."
apt-get -y install ${BUILDDEPS[*]}

echo "Building..."
mkdir /build
cp -ar /mnt/* /build
cd /build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .
cmake --build .
mkdir /dest
make DESTDIR=/dest install

echo "Packaging..."
cd /dest
fpm -s dir \
    -t deb \
    -n $PKGNAME \
    -v $PKGVERSION \
    $(for dep in ${PKGDEPS[*]}; do echo -n "-d $dep "; done) \
    .
chown \$ORIGUID:\$ORIGGID *.deb
cp -a *.deb /mnt

echo "Done!"
EOF

docker run \
    --rm \
    -e ORIGUID=$(id -u) \
    -e ORIGGID=$(id -g) \
    -v $(pwd):/mnt \
    debian:oldstable \
    bash /mnt/build-in-docker.bash

rm build-in-docker.bash
