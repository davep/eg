Summary: A text based Norton Guide database reader.
Name: eg
Version: 1.00
Release: 1
Copyright: GPL
Group: Applications/Publishing
Source: eg-1.00.tar.gz
Vendor: davep.org
Packager: Dave Pearson <davep@davep.org>
URL: http://www.hagbard.demon.co.uk/
%description
Expert Guide is a text based Norton Guide reader.
%prep 
%setup
%build
make
%install
make install
%files
%doc README TODO BUGS Changes COPYING eg.1

/usr/bin/eg
/usr/man/man1/eg.1
/usr/share/norton-guides/eg.ng
