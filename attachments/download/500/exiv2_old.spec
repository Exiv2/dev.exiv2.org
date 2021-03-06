Summary: Exif and Iptc metadata manipulation library
Name: exiv2
Version: 0.22
Release: 6%{?dist}

License: GPLv2+
Group: Applications/Multimedia
URL:  http://www.exiv2.org/
Source0: http://www.exiv2.org/exiv2-%{version}%{?pre:-%{pre}}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

## upstream patches

BuildRequires: expat-devel
BuildRequires: gettext
BuildRequires: pkgconfig
BuildRequires: zlib-devel
# docs
BuildRequires: doxygen graphviz libxslt

Requires: %{name}-libs%{?_isa} = %{version}-%{release}


%description
A command line utility to access image metadata, allowing one to:
* print the Exif metadata of Jpeg images as summary info, interpreted values,
  or the plain data for each tag
* print the Iptc metadata of Jpeg images
* print the Jpeg comment of Jpeg images
* set, add and delete Exif and Iptc metadata of Jpeg images
* adjust the Exif timestamp (that's how it all started...)
* rename Exif image files according to the Exif timestamp
* extract, insert and delete Exif metadata (including thumbnails),
  Iptc metadata and Jpeg comments

%package devel
Summary: Header files, libraries and development documentation for %{name}
Group: Development/Libraries
Requires: %{name}-libs%{?_isa} = %{version}-%{release}
Requires: pkgconfig
%description devel
%{summary}.

%package libs
Summary: Exif and Iptc metadata manipulation library
Group: System Environment/Libraries
%description libs
A C++ library to access image metadata, supporting full read and write access
to the Exif and Iptc metadata, Exif MakerNote support, extract and delete 
methods for Exif thumbnails, classes to access Ifd and so on.


%prep
%setup -q -n %{name}-%{version}%{?pre:-%{pre}}


%build
%configure   --disable-rpath   --disable-static 

sed -i 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=|g' libtool
sed -i 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' libtool

make %{?_smp_mflags} 

make doc


%install
rm -rf %{buildroot} 

make install DESTDIR=%{buildroot}

%find_lang exiv2

## Unpackaged files
rm -fv %{buildroot}%{_libdir}/libexiv2.la

## fix perms on installed lib
ls -l     %{buildroot}%{_libdir}/libexiv2.so.*
chmod 755 %{buildroot}%{_libdir}/libexiv2.so.*


%check
export PKG_CONFIG_PATH=%{buildroot}%{_datadir}/pkgconfig:%{buildroot}%{_libdir}/pkgconfig
test 0.22 = %{version}
test -x %{buildroot}%{_libdir}/libexiv2.so


%clean
rm -rf %{buildroot} 


%files 
%defattr(-,root,root,-)
%doc COPYING README
%{_bindir}/exiv2
%{_mandir}/man1/*

%post libs -p /sbin/ldconfig
%postun libs -p /sbin/ldconfig

%files libs -f exiv2.lang
%defattr(-,root,root,-)
%{_libdir}/libexiv2.so.11*

%files devel
%defattr(-,root,root,-)
%doc doc/html
%{_includedir}/exiv2/
%{_libdir}/libexiv2.so
%{_libdir}/pkgconfig/exiv2.pc


%changelog
* Tue Aug 14 2012 Rex Dieter <rdieter@fedoraproject.org> 0.22-6
- empty html doc dir (#848025)

* Tue Feb 28 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.22-5
- Rebuilt for c++ ABI breakage

* Mon Jan 16 2012 Rex Dieter <rdieter@fedoraproject.org> 0.22-4
- better rpath handling
- revert locale change, move back to -libs

* Mon Jan 16 2012 Rex Dieter <rdieter@fedoraproject.org> 0.22-3
- move locale files to main pkg (from -libs)

* Fri Jan 13 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.22-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_17_Mass_Rebuild

* Fri Oct 14 2011 Rex Dieter <rdieter@fedoraproject.org> 0.22-1
- exiv2-0.22

* Tue Sep 27 2011 Rex Dieter <rdieter@fedoraproject.org> 0.21.1-3
- New Tamron 70-300 mm lens improperly recognized (#708403)

* Mon Sep 26 2011 Rex Dieter <rdieter@fedoraproject.org> 0.21.1-2
- gthumb crashes because of bug in exiv2 0.21.1 (#741429)

* Sat Feb 26 2011 Rex Dieter <rdieter@fedoraproject.org> 0.21.1-1
- exiv2-0.21.1

* Tue Feb 08 2011 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.21-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_15_Mass_Rebuild

* Wed Jan 26 2011 Rex Dieter <rdieter@fedoraproject.org> 0.21-2
- Move ldconfig scriptlet calls to -libs (#672361)

* Wed Dec 01 2010 Rex Dieter <rdieter@fedoraproject.org> - 0.2


* Sun May 30 2
- exiv2-0.20

* Wed Dec 30 2009
- exiv2-0.19 (#552275)

* Su
- -libs unconditional
- tighten deps using %%?_isa

* Fri Aug 07 200
- (again) drop -fvisibility-inlines-hidden (#496050)

* Fri Jul 24 2009 R

- drop visibility p

* Fri Apr 17 2009 Rex Diet
- exiv2-0.18.1
- drop 

* Tue Feb 24 2009 Fedor
- Rebuilt for https://fed

* Thu
- exiv2-0.18

* Fri Dec 12 2008 Rex Die


* Mon Jun 23 2008 Rex Dieter <rdieter@fedoraproject.o


* Mon Feb 11
- respin (gcc43)
- gcc43 patch

* Sun Jan 13 2008 Rex Dieter <rdi
- eviv2-0.16

* Mon Dec 17 2007 Rex Dieter <rdieter[AT]fedoraproject.org> 0.1
- CVE-2007-6353 (#425924)

* Mon
- -libs subpkg toggle (f8+)

* Tue Nov 1
- exiv2-0.16-pre1

* Tue Sep 18 2007 Re
- -libs: -Requires: %%name

* Tue Aug 21 2007 Rex Dieter <r
- -libs subpkg to be multilib-friendlier (f8+)

* Sat Aug 
- License: GPLv2+

* Thu Jul 12 2007 Rex Dieter <rdieter[
- exiv2-0.15

* Mon Apr 02 2007 Rex Dieter <rdieter[AT]fedoraproje
- exiv2-0.14

* Tue Nov 28 2006 R


* Wed Oct 04 
- respin

* Tue Sep 19 2006 Rex Dieter <rexdieter[AT]users.sf.net> 0.11-2


* Tue Sep 19 2006 Rex Dieter
- exiv2-0.11

* Tue Aug 29 2006 Rex Dieter <rexdieter[
- fc6 respin

* Sat
- 0.10

* Wed May 
- cleanup %

- no_rpath pa
- deps patch (items get (re)compiled on *every* call

* Wed May 17 2006 Rex Dieter <rexd
- %%post/%%postun

* Tue May 16 2006 Rex Dieter <rexdieter[AT]users.sf.net> 0.9.1-1
- first try

