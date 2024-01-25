#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/examples/ss/dev/68/src/java/burst/makefile#1 $
#

PACKAGEDIR=com/smartsockets/burst
LIBPATH=.
JARFILE=$(LIBPATH)/ss_burst.jar
JAVAC=javac -g
CP=cp
RM=rm -f
JAVADOCCLASSPATH=-sourcepath .
JAVADOC_DIR=./doc
JAVADOC=javadoc
JAVADOC_WINDOWTITLE='SmartSockets Example TipcBurst Java API 6.5'
JAVADOC_HEADER='<b>SmartSockets Example TipcBurst Java API</b><br><font size="-1">6.5</font>'
JAVADOC_BOTTOM ='<TD ALIGN=LEFT VALIGN=TOP WIDTH="100%"><B><FONT SIZE=-2>Copyright &copy; 2000-2004 TIBCO Software Inc.<BR>3305 Hillview Avenue, Palo Alto, CA 94034</FONT></B></TD>'
JAVADOC_PACKAGES=com.smartsockets.burst
JAVADOC_SOURCEFILES=TipcBurstFilterFuncInterface.java \
                    TipcBurstMsgTraverseInterface.java \
                    TipcBurst.java \

CLASSES = \
	TipcBurstFilterFuncInterface.class \
        TipcBurstMsgTraverseInterface.class \
	TipcBurst.class

all: $(CLASSES) jar

TipcBurstFilterFuncInterface.class: TipcBurstFilterFuncInterface.java
	javac -g TipcBurstFilterFuncInterface.java

TipcBurstMsgTraverseInterface.class: TipcBurstMsgTraverseInterface.java
	javac -g TipcBurstMsgTraverseInterface.java

TipcBurst.class: TipcBurst.java TipcBurstFilterFuncInterface.class TipcBurstMsgTraverseInterface.class
	if [ ! -d $(PACKAGEDIR) ]; then mkdir -p $(PACKAGEDIR); fi
	javac -g *.java

jar: $(JARFILE)
	jar cvf $(JARFILE) $(PACKAGEDIR)/*.class

doc :: jar
	if [ ! -d $(JAVADOC_DIR) ]; then mkdir $(JAVADOC_DIR); fi
	cp -f $(JAVADOC_SOURCEFILES) $(PACKAGEDIR)
	$(JAVADOC) -d $(JAVADOC_DIR) $(JAVADOCCLASSPATH) -use -windowtitle $(JAVADOC_WINDOWTITLE) -header $(JAVADOC_HEADER) -bottom $(JAVADOC_BOTTOM) -public -verbose -version $(JAVADOC_PACKAGES)

$(JARFILE): $(CLASSES)
	$(CP) *.class $(PACKAGEDIR)

clean:
	rm -f $(JARFILE) *.class
	rm -rf com

