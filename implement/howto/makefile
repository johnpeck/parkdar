NAME= 		howto
TEXSRCS=	$(wildcard *.tex) \
			doctools/latex/notes_template.tex \
			doctools/latex/regcommands.tex \
			doctools/latex/mycommands.tex
XDVI_FLAGS= -paper letter -interpreter /usr/bin/X11/gs
DVIPS_FLAGS= -t letter
XFIGSRCS=	$(wildcard figs/*.fig)
include /usr/share/latex-mk/latex.gmk

