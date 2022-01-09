#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

A12COIND=${A12COIND:-$SRCDIR/a12coind}
A12COINCLI=${A12COINCLI:-$SRCDIR/a12coin-cli}
A12COINTX=${A12COINTX:-$SRCDIR/a12coin-tx}
A12COINQT=${A12COINQT:-$SRCDIR/qt/a12coin-qt}

[ ! -x $A12COIND ] && echo "$A12COIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
VER=($($A12COINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for bitcoind if --version-string is not set,
# but has different outcomes for bitcoin-qt and bitcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$A12COIND --version | sed -n '1!p' >> footer.h2m

for cmd in $A12COIND $A12COINCLI $A12COINTX $A12COINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${VER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${VER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
