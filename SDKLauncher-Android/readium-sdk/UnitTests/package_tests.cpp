//
//  package_tests.cpp
//  ePub3
//
//  Created by Jim Dovey on 2013-01-02.
//
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//  3. Neither the name of the organization nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//


#include "../ePub3/ePub/container.h"
#include "../ePub3/ePub/package.h"
#include "../ePub3/ePub/content_handler.h"
#include "../ePub3/utilities/error_handler.h"
#include "catch.hpp"
#include <cstdlib>
#include "../ePub3/xml/tree/document.h"

#define EPUB_PATH "TestData/childrens-literature-20120722.epub"
#define BINDINGS_EPUB_PATH "TestData/widget-figure-gallery-20121022.epub"
static const char* kMediaType = "application/x-epub-figure-gallery";

static const char* kInvalidVersion = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kMissingTitle = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kMissingLanguage = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kMissingIdentifier = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kInvalidUniqueIDRef = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="henry">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kMissingModDate = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kInvalidRefinementIRI = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="irkle%pond" property="title-type">main</meta>    <!-- hopefully an invalid IRI -->
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kAbsoluteRefinementIRI = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="http://server.com/index.html#henry" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kInvalidRefinement = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#henry" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kInvalidSpineIDRef = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="henry"/>
  </spine>
</package>
)X";

static const char* kMetadataOutOfPlace = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <placeholder></placeholder>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
</package>
)X";

static const char* kManifestOutOfPlace = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <placeholder></placeholder>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
</package>
)X";

static const char* kSpineOutOfPlace = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <placeholder></placeholder>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kNoContentDocInSpine = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kContentDocInFallback = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image" fallback="cover"/>
    <item href="css/epub.css" id="css" media-type="text/css"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover-img"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

static const char* kCircularFallbackChain = R"X(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="id">http://www.gutenberg.org/ebooks/25545</dc:identifier>
    <meta property="dcterms:modified">2010-02-17T04:39:13Z</meta>
    <dc:title id="t1">Children's Literature</dc:title>
    <meta refines="#t1" property="title-type">main</meta>
    <meta refines="#t1" property="display-seq">1</meta>
    <dc:title id="t2">A Textbook of Sources for Teachers and Teacher-Training Classes</dc:title>
    <meta refines="#t2" property="title-type">subtitle</meta>
    <meta refines="#t2" property="display-seq">2</meta>
    <dc:creator id="curry">Charles Madison Curry</dc:creator>
    <meta property="file-as" refines="#curry">Curry, Charles Madison</meta>
    <dc:creator id="clippinger">Erle Elsworth Clippinger</dc:creator>
    <meta property="file-as" refines="#clippinger">Clippinger, Erle Elsworth</meta>
    <dc:language>en</dc:language>
    <dc:date>2008-05-20</dc:date>
    <dc:subject>Children -- Books and reading</dc:subject>
    <dc:subject>Children's literature -- Study and teaching</dc:subject>
    <dc:source>http://www.gutenberg.org/files/25545/25545-h/25545-h.htm</dc:source>
    <dc:rights>Public domain in the USA.</dc:rights>
  </metadata>
  <manifest>
    <item href="images/cover.png" id="cover-img" media-type="image/png" properties="cover-image" fallback="css"/>
    <item href="css/epub.css" id="css" media-type="text/css" fallback="cover-img"/>
    <item href="cover.xhtml" id="cover" media-type="application/xhtml+xml"/>
    <item href="s04.xhtml" id="s04" media-type="application/xhtml+xml"/>
    <item href="nav.xhtml" id="nav" media-type="application/xhtml+xml" properties="nav"/>
  </manifest>
  <spine>
    <itemref idref="cover"/>
    <itemref idref="nav"/>
    <itemref idref="s04"/>
  </spine>
</package>
)X";

using namespace ePub3;

TEST_CASE("Package should have a Unique ID, Package ID, Type, Version, and a Base Path", "")
{
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = c->DefaultPackage();
    
    REQUIRE(pkg->UniqueID() == "http://www.gutenberg.org/ebooks/25545@2010-02-17T04:39:13Z");
    REQUIRE(pkg->PackageID() == "http://www.gutenberg.org/ebooks/25545");
    REQUIRE(pkg->Type() == "application/oebps-package+xml");
    REQUIRE(pkg->Version() == "3.0");
    REQUIRE(pkg->BasePath() == "EPUB/");
}

TEST_CASE("Packages with no version should raise a spec error", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kInvalidVersion, (int)strlen(kInvalidVersion)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFPackageHasNoVersion));
}

TEST_CASE("Packages with no title metadata should raise a spec error", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kMissingTitle, (int)strlen(kMissingTitle)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFMissingTitleMetadata));
}

TEST_CASE("Packages with no identifier should raise a spec error", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kMissingIdentifier, (int)strlen(kMissingIdentifier)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFMissingIdentifierMetadata));
}

TEST_CASE("Packages with no language metadata should raise a spec error", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kMissingLanguage, (int)strlen(kMissingLanguage)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFMissingLanguageMetadata));
}

TEST_CASE("Packages with no midification date should raise a spec error", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kMissingModDate, (int)strlen(kMissingModDate)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFMissingModificationDateMetadata));
}

TEST_CASE("Packages with an invalid unique-id reference should raise a spec error", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kInvalidUniqueIDRef, (int)strlen(kInvalidUniqueIDRef)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFPackageUniqueIDInvalid));
}

TEST_CASE("'refines' should contain a valid IRI", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kInvalidRefinementIRI, (int)strlen(kInvalidRefinementIRI)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFInvalidRefinementAttribute));
}

TEST_CASE("'refine' should contain a relative IRI", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kAbsoluteRefinementIRI, (int)strlen(kAbsoluteRefinementIRI)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFInvalidRefinementAttribute));
}

TEST_CASE("'refine' IRI should reference an existing item", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kInvalidRefinement, (int)strlen(kInvalidRefinement)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFInvalidRefinementTarget));
}

TEST_CASE("Spine 'idref' should reference an existing manifest item", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kInvalidSpineIDRef, (int)strlen(kInvalidSpineIDRef)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFInvalidSpineIdref));
}

TEST_CASE("Should raise an error if the Metadata is out of place", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kMetadataOutOfPlace, (int)strlen(kMetadataOutOfPlace)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFMetadataOutOfOrder));
}

TEST_CASE("Should raise an error if the Manifest is out of place", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kManifestOutOfPlace, (int)strlen(kManifestOutOfPlace)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFManifestOutOfOrder));
}

TEST_CASE("Should raise an error if the spine is out of place", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kSpineOutOfPlace, (int)strlen(kSpineOutOfPlace)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFSpineOutOfOrder));
}

TEST_CASE("Should raise an error if a spine item doesn't reference a content item", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kNoContentDocInSpine, (int)strlen(kNoContentDocInSpine)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFFallbackChainHasNoContentDocument));
}

TEST_CASE("Should be happy of a spine item only references a content item via the fallback chain", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kContentDocInFallback, (int)strlen(kContentDocInFallback)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::NoError));
}

TEST_CASE("Should raise an error if a fallback chain contains circular references", "")
{
    EPUBError triggeredError = EPUBError::NoError;
    SetErrorHandler([&](const error_details& err){
        if (err.is_spec_error())
            triggeredError = err.epub_error_code();
        return true;
    });
    
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = Package::New(c, "application/oebps-package+xml");
    
    auto doc = ePub3::xml::Wrapped<ePub3::xml::Document>(xmlParseMemory(kCircularFallbackChain, (int)strlen(kCircularFallbackChain)));
    pkg->_OpenForTest(doc, "EPUB/");
    
    SetErrorHandler(DefaultErrorHandler);
    REQUIRE(int(triggeredError) == int(EPUBError::OPFFallbackChainCircularReference));
}

TEST_CASE("Our test package should only have TOC and PageList navigation tables", "")
{
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = c->DefaultPackage();
    
    REQUIRE(pkg->TableOfContents() != nullptr);
    REQUIRE(pkg->ListOfFigures() == nullptr);
    REQUIRE(pkg->ListOfIllustrations() == nullptr);
    REQUIRE(pkg->ListOfTables() == nullptr);
    REQUIRE(pkg->PageList() != nullptr);
}

TEST_CASE("Package should have multiple manifest items, and they should be indexable by identifier string", "")
{
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = c->DefaultPackage();
    auto manifest = pkg->Manifest();
    REQUIRE_FALSE(manifest.empty());
    
    int idx = arc4random() % manifest.size();
    ManifestItemPtr randomItem;
    for ( auto pos = manifest.begin(); idx >= 0; ++pos, --idx )
    {
        randomItem = pos->second;
    }
    
    if ( randomItem == nullptr )
        randomItem = manifest.begin()->second;
    
    auto subscripted = (*pkg)[randomItem->Identifier()];
    REQUIRE(subscripted == randomItem);
    
    auto fetched = pkg->ManifestItemWithID(randomItem->Identifier());
    REQUIRE(fetched == randomItem);
}

TEST_CASE("Package should have multiple spine items", "")
{
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = c->DefaultPackage();
    auto spine = pkg->FirstSpineItem();
    REQUIRE(spine != nullptr);
    
    size_t count = spine->Count();
    REQUIRE(count > 1);
    
    size_t idx = arc4random() % count;
    REQUIRE(pkg->SpineItemAt(idx) == (*pkg)[idx]);
}

TEST_CASE("Package should be able to create and resolve basic CFIs", "")
{
    ContainerPtr c = Container::OpenContainer(EPUB_PATH);
    PackagePtr pkg = c->DefaultPackage();
    
    uint32_t numSpineItems = static_cast<uint32_t>(pkg->FirstSpineItem()->Count());
    size_t spineIdx = arc4random_uniform(numSpineItems);
    auto spineItem = pkg->SpineItemAt(spineIdx);
    
    // create a mutable CFI
    CFI cfi(pkg->CFIForSpineItem(spineItem));
    REQUIRE_FALSE(cfi.Empty());
    
    std::string str(_Str("/", pkg->SpineCFIIndex(), "/", (spineIdx+1)*2, "[", spineItem->Idref(), "]!"));
    REQUIRE(cfi.String() == _Str("epubcfi(", str, ")"));
    REQUIRE(cfi == _Str("epubcfi(", str, ")"));
    
    ManifestItemPtr manifestItem = pkg->ManifestItemForCFI(cfi, nullptr);
    REQUIRE(manifestItem != nullptr);
    REQUIRE(manifestItem == spineItem->ManifestItem());
    
    // subpath returned does not have a leading forward-slash
    REQUIRE(pkg->CFISubpathForManifestItemWithID(manifestItem->Identifier()) == str.substr(1));
    
    CFI fragment("/4/2,1:22,1:28");
    cfi += fragment;
    
    CFI remainder;
    (void) pkg->ManifestItemForCFI(cfi, &remainder);
    REQUIRE(remainder == fragment);
}

TEST_CASE("Package should parse bindings correctly.", "")
{
    ContainerPtr c = Container::OpenContainer(BINDINGS_EPUB_PATH);
    PackagePtr pkg = c->DefaultPackage();
    const Package::StringList handledTypes = pkg->MediaTypesWithDHTMLHandlers();
    
    REQUIRE(handledTypes.size() == 1);
    REQUIRE(handledTypes[0] == kMediaType);
    
    auto handler = pkg->OPFHandlerForMediaType(kMediaType);
    REQUIRE(handler != nullptr);
    REQUIRE(handler->MediaType() == kMediaType);
    
    IRI target = handler->Target("test.xml", ContentHandler::ParameterList());
    REQUIRE(target.URIString() == _Str("epub3://", pkg->PackageID(), "/EPUB/figure-gallery-widget/figure-gallery-impl.xhtml?src=test.xml"));
}
