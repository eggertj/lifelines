#
# lifelines.spec - RPM configuration file for Lifelines
#
# To generate RPMs, place the tar.gz in /usr/src/packages/SOURCES/ and
# (or /usr/src/redhat/SOURCES as appropriate)
# then "rpm -ba lifelines.spec"
#

%define lifelines_version       3.0.27

Name: lifelines
Summary: lifelines genealogy program
Version: %{lifelines_version}
Release: 1
Copyright: MIT
Group: Utilities/System
Source:         http://download.sourceforge.net/lifelines/lifelines-%{lifelines_version}.tar.gz
URL:            http://lifelines.sourceforge.net/
Packager:       Marc Nozell <marc@nozell.com>
Provides:       lifelines
BuildRoot: %{_tmppath}/%{name}-%{version}-root
%description
This program allows the tracking of genealogical information.  The lifelines
reports are the power of the system.

%prep 
%setup

%build
#make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"
sh -c ./configure 
make 

%install
install -d -m 755 $RPM_BUILD_ROOT/usr/local/bin
install -s -m 755 src/liflines/llines $RPM_BUILD_ROOT/usr/local/bin
install -s -m 755 src/liflines/llexec $RPM_BUILD_ROOT/usr/local/bin
install -s -m 755 src/tools/dbverify $RPM_BUILD_ROOT/usr/local/bin

install -d -m 755 $RPM_BUILD_ROOT/usr/local/share/locale/da/LC_MESSAGES
install -m 644 po/da.gmo $RPM_BUILD_ROOT/usr/local/share/locale/da/LC_MESSAGES/lifelines.mo

install -d -m 755 $RPM_BUILD_ROOT/usr/local/share/locale/de/LC_MESSAGES
install -m 644 po/de.gmo $RPM_BUILD_ROOT/usr/local/share/locale/de/LC_MESSAGES/lifelines.mo

install -d -m 755 $RPM_BUILD_ROOT/usr/local/share/locale/fr/LC_MESSAGES
install -m 644 po/fr.gmo $RPM_BUILD_ROOT/usr/local/share/locale/fr/LC_MESSAGES/lifelines.mo

install -d -m 755 $RPM_BUILD_ROOT/usr/local/share/locale/sv/LC_MESSAGES
install -m 644 po/sv.gmo $RPM_BUILD_ROOT/usr/local/share/locale/sv/LC_MESSAGES/lifelines.mo

install -d -m 755 $RPM_BUILD_ROOT/usr/local/share/lifelines/reports
install -m 644 reports/CREDIT reports/*.ll $RPM_BUILD_ROOT/usr/local/share/lifelines/reports

install -d -m 755 $RPM_BUILD_ROOT/usr/local/man/man1
install -m 644 docs/llines.1 $RPM_BUILD_ROOT/usr/local/man/man1/llines.1


%files
%defattr(-,root,root)
%doc README ChangeLog NEWS AUTHORS LICENSE
%doc docs/ll-reportmanual.xml docs/ll-reportmanual.html docs/ll-reportmanual.pdf docs/ll-reportmanual.txt
%doc docs/ll-userguide.xml docs/l-userguide.html docs/ll-userguide.pdf docs/ll-userguide.txt

/usr/local/bin/llines
/usr/local/bin/llexec
/usr/local/bin/dbverify
/usr/local/share/locale/da/LC_MESSAGES/lifelines.mo
/usr/local/share/locale/de/LC_MESSAGES/lifelines.mo
/usr/local/share/locale/fr/LC_MESSAGES/lifelines.mo
/usr/local/share/locale/sv/LC_MESSAGES/lifelines.mo
/usr/local/share/lifelines/reports
/usr/local/man/man1/llines.1

%changelog
* Mon Nov 11 2002 Perry Rapp
- Add da.po
- (post lifelines-3.0.21)
* Mon Oct 07 2002 Perry Rapp
- Add new binary llexec
* Sun Sep 29 2002 Perry Rapp
- Implement patch by Karl DeBisschop: adds build root, list reports as group instead of individually
- Add dbverify, de.po, fr.po, sv.po
- (lifelines-3.0.19-1)
