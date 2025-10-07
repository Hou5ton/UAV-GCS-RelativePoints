#!/usr/bin/env /bin/bash
cd ../../gcs
if [[ -d "/opt/Qt5.14.2/5.14.2/gcc_64" && -d "/opt/Qt5.14.2/Tools/QtCreator/bin" ]]; then
    echo "make deploy"
    QTDIR=/opt/Qt5.14.2/5.14.2/gcc_64 QBSDIR=/opt/Qt5.14.2/Tools/QtCreator/bin make deploy
else
    if [[ -d "/home/gcu/Qt5.14.2/5.14.2/gcc_64" && -d "/home/gcu/Qt5.14.2/Tools/QtCreator/bin" ]]; then
        echo "make deploy"
        QTDIR=/home/gcu/Qt5.14.2/5.14.2/gcc_64 QBSDIR=/home/gcu/Qt5.14.2/Tools/QtCreator/bin make deploy
    else
        echo "QTDIR && QBSDIR do not exist."
    fi
fi

