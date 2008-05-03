#!/bin/bash

#
# empacota.sh
#
# Group 1
# Creates a valid zip file with our work :)
#

TPNAME="tp3.zip"
TPOUTPUT="tp03_grupo01"

if [ -f $TPNAME ]; then
  echo ">> Apagando zip anterior..."
  rm $TPNAME
fi

echo ">> Criando árvore de diretórios..."
svn export -q . $TPOUTPUT

echo ">> Gerando relatório..."
pdflatex report.tex 2>&1>/dev/null
rm report.{aux,log}

echo ">> Adicionando report.pdf à árvore..."
cp report.pdf $TPOUTPUT

echo ">> Gerando documentação..."
if [ ! -d doc/api/ ]; then
  mkdir -p doc/api
fi
doxygen doxygen.conf
mv doc/ $TPOUTPUT

echo ">> Criando zip..."
zip -9 -r -q $TPNAME $TPOUTPUT

echo ">> Removendo pasta temporária de árvore..."
rm -fr $TPOUTPUT

echo ":: Arquivo $TPNAME criado com sucesso!"
