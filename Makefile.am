AUTOMAKE_OPTIONS = 1.4

SUBDIRS = src

DIST_SUBDIRS = $(SUBDIRS)

EXTRA_DIST = \
	COPYING \
	INSTALL \
	TODO \
	autogen.sh

$(PACKAGE)-$(VERSION).tar.gz: dist

install-hook:
	-ldconfig

cross-rpm: $(PACKAGE)-$(VERSION).tar.gz 
	cp $(PACKAGE)-$(VERSION).tar.gz /usr/src/redhat/SOURCES
	rpm -ba $(PACKAGE).spec

rpm: $(PACKAGE)-$(VERSION).tar.gz 
	cp $(PACKAGE)-$(VERSION).tar.gz /usr/src/redhat/SOURCES
	rpm -ba $(PACKAGE).spec

rpm_i686: $(PACKAGE)-$(VERSION).tar.gz 
	cp $(PACKAGE)-$(VERSION).tar.gz /usr/src/redhat/SOURCES
	rpm -ba --target=i686-linux $(PACKAGE).spec

rpm_i586: $(PACKAGE)-$(VERSION).tar.gz 
	cp $(PACKAGE)-$(VERSION).tar.gz /usr/src/redhat/SOURCES
	rpm -ba --target=i586-linux $(PACKAGE).spec

rpm_i486: $(PACKAGE)-$(VERSION).tar.gz
	cp $(PACKAGE)-$(VERSION).tar.gz /usr/src/redhat/SOURCES
	rpm -ba --target=i486-linux $(PACKAGE).spec
