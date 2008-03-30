#!/bin/sh

#
# empacota.sh
#
# Group 1
# Creates a valid zip file with our work :)
#

TPNAME="tp2.zip"
TPOUTPUT="tpzip"

if [ -f $TPNAME ]; then
  echo ">> Apagando zip anterior..."
  rm $TPNAME
fi

echo ">> Criando árvore de diretórios..."
svn export -q . $TPOUTPUT

echo ">> Gerando relatório..."
pdflatex report.tex 2>&1>/dev/null

echo ">> Adicionando report.pdf à árvore..."
cp report.pdf $TPOUTPUT

echo ">> Criando zip..."
zip -9 -r -q $TPNAME $TPOUTPUT

echo ">> Removendo pasta temporária de árvore..."
rm -fr $TPOUTPUT

echo ":: Arquivo $TPNAME criado com sucesso!"
