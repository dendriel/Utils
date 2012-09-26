#!/usr/bin/python
# -*- coding: utf-8 -*-
#################################################################
# Author: Vitor Rozsa                                           #
# Contact: vitor.rozsa@hotmail.com                              #
# Version: 1.0                                                  #
#                                                               #
# Description:                                                  #
#  Simple script that "unify" all application commands into     #
# only one simple command "open". It's possible to add any      #
# application command following the samples. Add an alias into  #
# ~/.bashrc to make easy and natural to use the "open" script.  #
#################################################################
import sys
import os

alen = len(sys.argv)

def open_file(ext, arq_path):


	if ext == "txt" or arq_path.find(".txt") != -1:
		print "vi %s" % arq_path
		os.system("vi \"%s\"" % arq_path)

	elif ext == "sh" or arq_path.find(".sh") != -1:
		print "vi %s" % arq_path
		os.system("vi \"%s\"" % arq_path)

	elif ext == "log" or arq_path.find(".log") != -1:
		print "vi %s" % arq_path
		os.system("vi %s" % arq_path)

	elif ext == "py" or arq_path.find(".py") != -1:
		print "vi %s" % arq_path
		os.system("vi %s" % arq_path)

	elif ext == "pdf" or arq_path.find(".pdf") != -1:
		print "evince %s" % arq_path
		os.system("evince \"%s\" &" % arq_path)

	elif ext == "odt" or arq_path.find(".odt") != -1:
		print "libreoffice %s" % arq_path
		os.system("libreoffice \"%s\" &" % arq_path)

	elif ext == "doc" or arq_path.find(".doc") != -1:
		print "libreoffice %s" % arq_path
		os.system("libreoffice \"%s\" &" % arq_path)

	elif ext == "docx" or arq_path.find(".docx") != -1:
		print "libreoffice %s" % arq_path
		os.system("libreoffice \"%s\" &" % arq_path)

	elif ext == "xlsx" or arq_path.find(".xlsx") != -1:
		print "libreoffice %s" % arq_path
		os.system("libreoffice \"%s\" &" % arq_path)

	elif ext == "html" or arq_path.find(".html") != -1:
		print "google-chrome %s" % arq_path
		os.system("google-chrome \"%s\" &" % arq_path)

	elif ext == "lua" or arq_path.find(".lua") != -1:
		print "eclipse_lua %s" % arq_path
		os.system("eclipse_lua \"%s\" &" % arq_path)

	# Use "nautilus" instead of thunar in Ubuntu distributions #
	elif len(arq_path) > 0:
		print "thunar %s" % arq_path
		os.system("thunar \"%s\"" % arq_path)

	else:
		print "\nNão existe uma aplicação cadastrada para rodar este tipo de extensão.\n"

if alen < 2 or alen > 3:
	print """\n##-+ Unified Open File Script +-##\n\nUso:\n
		Você deve especificar pelo menos o caminho do arquivo que deseja executar/abrir.
		Pode-se especificar a extensão do arquivo caso ela não esteja escrita no próprio nome.\n
		Exemplo: $open nome_arquivo.txt
			 $open nome_arquivo txt

		No caso do arquivo possuir um nome separado por espaços, utilize " (aspas) para delimitar o nome.
		"""

elif alen == 2:
	open_file("none", sys.argv[1])

elif alen == 3:
	open_file(sys.argv[2], sys.argv[1])
