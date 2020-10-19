#!/bin/bash

# $1 is input raw file.
# $2 is options file to process.

KVAZAAR="../kvazaar"

while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "Text read from file: $line"
    IFS=';' read -ra ADDR <<< "$line"

    echo "Option number: " "${ADDR[0]}"
    echo "Option contents: " "${ADDR[1]}"

    OPNUM="${ADDR[0]}"
    OPKNOBS="${ADDR[1]}"

    #${KVAZAAR} ${OPKNOBS} -i ${1} -o "$1_OP${OPNUM}.265" 2> "${1}_OP${OPNUM}.out"
    ${KVAZAAR} ${OPKNOBS} -i ${1} -o "$1_OP${OPNUM}.265" 2> `basename "$1_OP${OPNUM}.out"`

    #grep fps `basename "$1_OP${OPNUM}.out"` > `basename "$1_OP${OPNUM}_fps.out"`
    grep stats `basename "$1_OP${OPNUM}.out"` > `basename "$1_OP${OPNUM}_fps.out"`

    #octave --no-gui process.m `basename "$1_OP${OPNUM}_fps.out"`  "${OPNUM}"
    octave --no-gui process2.m `basename "$1_OP${OPNUM}_fps.out"`  "${OPNUM}"

done < "$2"
