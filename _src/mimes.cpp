#include <string>
#include <map>

// http://www.w3.org/TR/xhtml-media-types/

#include "mimes.h"

const std::map< std::string, std::string >&
getMimeTypes()
{
    static
    std::map< std::string, std::string > _ = 

        { { ""        , "application/octet-stream" } /* application/octet-stream    bin dms lha lrf lzh so iso dmg dist distz pkg bpk dump elc deploy */
    
          /* common types, always enabled */
          /* text types */
        , { ".htm"    , "text/html"  }
        , { ".html"   , "text/html"  }
        , { ".css"    , "text/css"   }
        , { ".xml"    , "text/xml"   }
        , { ".xsl"    , "text/xml"   }
        , { ".xhtml"  , "text/xml"   }
        , { ".xht"    , "text/xml"   }
    
        , { ".js"     , "application/javascript" }
        , { ".json"   , "application/json"       }
    
        , { ".txt"    , "text/plain" }
        , { ".inf"    , "text/plain" }
        , { ".ini"    , "text/plain" }

        , { ".text"   , "text/plain" }
        , { ".conf"   , "text/plain" }
        , { ".def"    , "text/plain" }
        , { ".log"    , "text/plain" }
        , { ".csv"    , "text/csv"   }
        , { ".uu"     , "text/x-uuencode"  }
        , { ".vcs"    , "text/x-vcalendar" }
        , { ".vcf"    , "text/x-vcard"  }
        , { ".uri"    , "text/uri-list" }
        , { ".uris"   , "text/uri-list" }
        , { ".urls"   , "text/uri-list" }
    
        , { ".rtf"    , "text/rtf"   }
    
        , { ".s"      , "text/x-asm" }
        , { ".asm"    , "text/x-asm" }

        , { ".c"      , "text/x-c" }
        , { ".cc"     , "text/x-c" }
        , { ".cxx"    , "text/x-c" }
        , { ".c++"    , "text/x-c" }
        , { ".cpp"    , "text/x-c" }
        , { ".h"      , "text/x-c" }
        , { ".hh"     , "text/x-c" }
        , { ".hpp"    , "text/x-c" }

        , { ".dic"    , "text/x-c" }
    
        , { ".f"      , "text/x-fortran" }
        , { ".for"    , "text/x-fortran" }
        , { ".f77"    , "text/x-fortran" }
        , { ".f90"    , "text/x-fortran" }

        , { ".p"      , "text/x-pascal"  }
        , { ".pas"    , "text/x-pascal"  }
    
        , { ".java"   , "text/x-java-source"  }
    
        , { ".vbs"   , "text/vbscript"  }
    
        , { ".ics"    , "text/calendar"           }
        , { ".ifb"    , "text/calendar"           }
        , { ".rtx"    , "text/richtext"           }
        , { ".sgm"    , "text/plain"              }
        , { ".sgml"   , "text/plain"              }
        , { ".list"   , "text/plain"              }
        , { ".in"     , "text/plain"              }
        , { ".etx"    , "text/x-setext"           }
        , { ".n3"     , "text/n3"                 }
        , { ".dsc"    , "text/prs.lines.tag"      }
        , { ".tsv"    , "text/tab-separated-values" }
        , { ".t"      , "text/troff"              }
        , { ".tr"     , "text/troff"              }
        , { ".roff"   , "text/troff"              }
        , { ".man"    , "text/troff"              }
        , { ".me"     , "text/troff"              }
        , { ".ms"     , "text/troff"              }
        , { ".ttl"    , "text/turtle"             }
        , { ".curl"   , "text/vnd.curl"           }
        , { ".dcurl"  , "text/vnd.curl.dcurl"     }
        , { ".scurl"  , "text/vnd.curl.scurl"     }
        , { ".mcurl"  , "text/vnd.curl.mcurl"     }
        , { ".fly"    , "text/vnd.fly"            }
        , { ".flx"    , "text/vnd.fmi.flexstor"   }
        , { ".gv"     , "text/vnd.graphviz"       }
        , { ".3dml"   , "text/vnd.in3d.3dml"      }
        , { ".spot"   , "text/vnd.in3d.spot"      }
        , { ".jad"    , "text/vnd.sun.j2me.app-descriptor" }
        , { ".wml"    , "text/vnd.wap.wml"        }
        , { ".wmlscript","text/vnd.wap.wmlscript" }
        /*, { "."    , "" }*/
    
        /* xml types */
        , { ".xhtml"  , "application/xhtml+xml"  }
        , { ".xht"    , "application/xhtml+xml"  }
        , { ".xml"    , "application/xml"    }
        , { ".xsl"    , "application/xml"    }

        , { ".dtd"    , "application/xml-dtd"    }
        , { ".xslt"   , "application/xslt+xml"   }
    
    
        /* common image types */
        /* category same as common mime-type */
        , { ".gif"    , "image/gif"     }
        , { ".png"    , "image/png"     }
        , { ".jpg"    , "image/jpeg"    }
        , { ".ico"    , "image/x-icon"  }
        , { ".svg"    , "image/svg+xml" }

        , { ".jpeg"   , "image/jpeg"    }
        , { ".jpe"    , "image/jpeg"    }
        , { ".bmp"    , "image/bmp"     }
        , { ".tiff"   , "image/tiff"    }
        , { ".tif"    , "image/tiff"    }
        , { ".pcx"    , "image/x-pcx"   }
        , { ".djvu"   , "image/vnd.djvu"}
        , { ".djv"    , "image/vnd.djvu"}
        , { ".wbmp"   , "image/vnd.wap.wbmp"}
        , { ".xbm"    , "image/x-xbitmap"}
        , { ".xpm"    , "image/x-xpixmap"}


        , { ".ai"    , "application/postscript" }
        , { ".ps"    , "application/postscript" }
        , { ".eps"   , "application/postscript" }

        , { ".dwg"    , "image/vnd.dwg" }
        , { ".dxf"    , "image/vnd.dxf" }

        , { ".psd"    , "image/vnd.adobe.photoshop" }

        , { ".cdr"    , "image/x-cdr" }  /* corel draw http://filext.com/file-extension/CDR */

        , { ".cgm"    , "image/cgm"   }
        , { ".g3"     , "image/g3fax" }
        , { ".ief"    , "image/ief"   }
        , { ".ktx"    , "image/ktx"   }
        , { ".btif"   , "image/prs.btif" }
        , { ".uvi"    , "image/vnd.dece.graphic" }
        , { ".uvvi"   , "image/vnd.dece.graphic" }
        , { ".uvg"    , "image/vnd.dece.graphic" }
        , { ".uvvg"   , "image/vnd.dece.graphic" }
        , { ".sub"    , "image/vnd.dvb.subtitle" }
        , { ".fbs"    , "image/vnd.fastbidsheet" }
        , { ".fpx"    , "image/vnd.fpx" }
        , { ".fst"    , "image/vnd.fst" }
        , { ".mmr"    , "image/vnd.fujixerox.edmics-mmr" }
        , { ".rlc"    , "image/vnd.fujixerox.edmics-rlc" }
        , { ".mdi"    , "image/vnd.ms-modi" }
        , { ".npx"    , "image/vnd.net-fpx" }
        , { ".xif"    , "image/vnd.xiff" }
        , { ".webp"   , "image/webp" }
        , { ".ras"    , "image/x-cmu-raster" }
        , { ".cmx"    , "image/x-cmx" }
        , { ".fh"     , "image/x-freehand" }
        , { ".fhc"    , "image/x-freehand" }
        , { ".fh4"    , "image/x-freehand" }
        , { ".fh5"    , "image/x-freehand" }
        , { ".fh7"    , "image/x-freehand" }
        , { ".pic"    , "image/x-pict" }
        , { ".pct"    , "image/x-pict" }
        , { ".pnm"    , "image/x-portable-anymap" }
        , { ".pbm"    , "image/x-portable-bitmap" }
        , { ".pgm"    , "image/x-portable-graymap" }
        , { ".ppm"    , "image/x-portable-pixmap" }
        , { ".rgb"    , "image/x-rgb" }
        , { ".xwd"    , "image/x-xwindowdump" }
    
        , { ".rar"    , "application/x-rar-compressed" }
        , { ".7z"     , "application/x-7z-compressed"  }
        , { ".zip"    , "application/zip"              }
        , { ".cab"    , "application/vnd.ms-cab-compressed" }
        , { ".gz"     , "application/x-gzip" }
        , { ".tgz"    , "application/x-gzip" }

        , { ".bz"     , "application/x-bzip"  }
        , { ".bz2"    , "application/x-bzip2" }
        , { ".boz"    , "application/x-bzip2" }
        , { ".gtar"   , "application/x-gtar"  }
        , { ".z"      , "application/x-compress" }
        , { ".arj"    , "application/x-arj" }
        , { ".lzh"    , "application/x-lzh" }
        , { ".lha"    , "application/x-lha" }
        /*, { ".", "" }*/

        , { ".tar"     , "application/x-tar" }
        , { ".iso"     , "application/octet-stream" }
        /*, { "."     , "" }*/
    
        /* application/octet-stream    dms lrf dmg dist distz pkg bpk dump elc deploy */
    
        , { ".com"    , "application/x-msdownload" }
        , { ".exe"    , "application/x-msdownload" }

        , { ".dll"    , "application/x-msdownload" }
    
        /* Microsoft */
        , { ".lib"    , "application/octet-stream" }
        , { ".obj"    , "application/octet-stream" }
        , { ".ocx"    , "application/octet-stream" }
        , { ".tlb"    , "application/octet-stream" }
        /* *nix */
        , { ".so"     , "application/octet-stream" }
        , { ".o"      , "application/octet-stream" }
        , { ".a"      , "application/octet-stream" }
    
        , { ".drv"    , "application/octet-stream" }
        , { ".sys"    , "application/octet-stream" }

        /* Microsoft */
        , { ".cpl"    , "application/octet-stream" }
        , { ".ime"    , "application/octet-stream" }
        , { ".mui"    , "application/octet-stream" }
        /* other */
        , { ".bin"    , "application/octet-stream" }
        /*, { "."    , "application/octet-stream" }*/
    
        , { ".bat"    , "application/x-msdownload" }
        , { ".cmd"    , "application/x-msdownload" }

        , { ".csh"    , "application/x-csh" }
        , { ".sh"     , "application/x-sh"  }
    
        , { ".chm"    , "application/vnd.ms-htmlhelp" }
        , { ".hlp"    , "application/winhlp"       }
        , { ".wmf"    , "application/x-msmetafile" }
        , { ".emf"    , "application/x-emf"        }
        , { ".msi"    , "application/x-msdownload" }
        /*, { ".", "application/x-msdownload" } */
    
        , { ".doc"    , "application/msword" }
        , { ".dot"    , "application/msword" }
        , { ".xls"    , "application/vnd.ms-excel" }
        , { ".xlm"    , "application/vnd.ms-excel" }
        , { ".xla"    , "application/vnd.ms-excel" }
        , { ".xlc"    , "application/vnd.ms-excel" }
        , { ".xlt"    , "application/vnd.ms-excel" }
        , { ".xlw"    , "application/vnd.ms-excel" }
        , { ".ppt"    , "application/vnd.ms-powerpoint" }
        , { ".pps"    , "application/vnd.ms-powerpoint" }
        , { ".pot"    , "application/vnd.ms-powerpoint" }
        , { ".vsd"    , "application/vnd.visio" }
        , { ".vst"    , "application/vnd.visio" }
        , { ".vss"    , "application/vnd.visio" }
        , { ".vsw"    , "application/vnd.visio" }
        , { ".mdb"    , "application/x-msaccess"}
        /*, { ".", "" }*/
    
        , { ".xlam"   , "application/vnd.ms-excel.addin.macroenabled.12"            }
        , { ".xlsb"   , "application/vnd.ms-excel.sheet.binary.macroenabled.12"     }
        , { ".xlsm"   , "application/vnd.ms-excel.sheet.macroenabled.12"            }
        , { ".xltm"   , "application/vnd.ms-excel.template.macroenabled.12"         }
        , { ".ppam"   , "application/vnd.ms-powerpoint.addin.macroenabled.12"       }
        , { ".pptm"   , "application/vnd.ms-powerpoint.presentation.macroenabled.12"}
        , { ".sldm"   , "application/vnd.ms-powerpoint.slide.macroenabled.12"       }
        , { ".ppsm"   , "application/vnd.ms-powerpoint.slideshow.macroenabled.12"   }
        , { ".potm"   , "application/vnd.ms-powerpoint.template.macroenabled.12"    }
        , { ".docm"   , "application/vnd.ms-word.document.macroenabled.12"          }
        , { ".dotm"   , "application/vnd.ms-word.template.macroenabled.12"          }
        , { ".thmx"   , "application/vnd.ms-officetheme"}
        /*, { ".", "" }*/
    
        , { ".cil"    , "application/vnd.ms-artgalry"   }
        , { ".eot"    , "application/vnd.ms-fontobject" }
        , { ".ims"    , "application/vnd.ms-ims"        }
        , { ".lrm"    , "application/vnd.ms-lrm"        }
        , { ".cat"    , "application/vnd.ms-pki.seccat" }
        , { ".stl"    , "application/vnd.ms-pki.stl"    }
        , { ".mpp"    , "application/vnd.ms-project"    }
        , { ".mpt"    , "application/vnd.ms-project"    }
        , { ".obd"    , "application/x-msbinder"        }
        , { ".crd"    , "application/x-mscardfile"      }
        , { ".clp"    , "application/x-msclip"          }
        , { ".mny"    , "application/x-msmoney"         }
        , { ".pub"    , "application/x-mspublisher"     }
        , { ".scd"    , "application/x-msschedule"      }
        , { ".trm"    , "application/x-msterminal"      }
        , { ".mvb"    , "application/x-msmediaview"     }
        , { ".m13"    , "application/x-msmediaview"     }
        , { ".m14"    , "application/x-msmediaview"     }
        , { ".wpl"    , "application/vnd.ms-wpl"        }
        , { ".xps"    , "application/vnd.ms-xpsdocument"}
        /*, { ".application", "application/x-ms-application" }*/
        , { ".wmd"    , "application/x-ms-wmd"  }
        , { ".wmz"    , "application/x-ms-wmz"  }
        , { ".xbap"   , "application/x-ms-xbap" }
        /*, { ".", "" }*/
    
        , { ".odc"    , "application/vnd.oasis.opendocument.chart"                 }
        , { ".otc"    , "application/vnd.oasis.opendocument.chart-template"        }
        , { ".odb"    , "application/vnd.oasis.opendocument.database"              }
        , { ".odf"    , "application/vnd.oasis.opendocument.formula"               }
        , { ".odft"   , "application/vnd.oasis.opendocument.formula-template"      }
        , { ".odg"    , "application/vnd.oasis.opendocument.graphics"              }
        , { ".otg"    , "application/vnd.oasis.opendocument.graphics-template"     }
        , { ".odi"    , "application/vnd.oasis.opendocument.image"                 }
        , { ".oti"    , "application/vnd.oasis.opendocument.image-template"        }
        , { ".odp"    , "application/vnd.oasis.opendocument.presentation"          }
        , { ".otp"    , "application/vnd.oasis.opendocument.presentation-template" }
        , { ".ods"    , "application/vnd.oasis.opendocument.spreadsheet"           }
        , { ".ots"    , "application/vnd.oasis.opendocument.spreadsheet-template"  }
        , { ".odt"    , "application/vnd.oasis.opendocument.text"                  }
        , { ".odm"    , "application/vnd.oasis.opendocument.text-master"           }
        , { ".ott"    , "application/vnd.oasis.opendocument.text-template"         }
        , { ".oth"    , "application/vnd.oasis.opendocument.text-web"              }
        , { ".oxt"    , "application/vnd.openofficeorg.extension" }
        , { ".pptx"   , "application/vnd.openxmlformats-officedocument.presentationml.presentation" }
        , { ".sldx"   , "application/vnd.openxmlformats-officedocument.presentationml.slide"        }
        , { ".ppsx"   , "application/vnd.openxmlformats-officedocument.presentationml.slideshow"    }
        , { ".potx"   , "application/vnd.openxmlformats-officedocument.presentationml.template"     }
        , { ".xlsx"   , "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"         }
        , { ".xltx"   , "application/vnd.openxmlformats-officedocument.spreadsheetml.template"      }
        , { ".docx"   , "application/vnd.openxmlformats-officedocument.wordprocessingml.document"   }
        , { ".dotx"   , "application/vnd.openxmlformats-officedocument.wordprocessingml.template"   }
      
        , { ".tex"    , "application/x-tex"      }
        , { ".tfm"    , "application/x-tex-tfm"  }
        , { ".texinfo", "application/x-texinfo"  }
        , { ".texi"   , "application/x-texinfo"  }
        , { ".latex"  , "application/x-latex"    }
    
        , { ".wri"    , "application/x-mswrite"     }
        , { ".wps"    , "application/vnd.ms-works"  }
        , { ".wks"    , "application/vnd.ms-works"  }
        , { ".wcm"    , "application/vnd.ms-works"  }
        , { ".wdb"    , "application/vnd.ms-works"  }
        , { ".pdf"    , "application/pdf"           }
        , { ".wpd"    , "application/vnd.wordperfect" }
        /*, { ".", "" }*/
    
        , { ".qxd"    , "application/vnd.quark.quarkxpress" }
        , { ".qxt"    , "application/vnd.quark.quarkxpress" }
        , { ".qwd"    , "application/vnd.quark.quarkxpress" }
        , { ".qwt"    , "application/vnd.quark.quarkxpress" }
        , { ".qxl"    , "application/vnd.quark.quarkxpress" }
        , { ".qxb"    , "application/vnd.quark.quarkxpress" }
        /*, { ".", "" }*/
    
        , { ".der"    , "application/x-x509-ca-cert" }
        , { ".crt"    , "application/x-x509-ca-cert" }
        , { ".pgp"    , "application/pgp-encrypted"  }
        , { ".asc"    , "application/pgp-signature"  }
        , { ".sig"    , "application/pgp-signature"  }
    
        , { ".jar"    , "application/java-archive" }
        , { ".ser"    , "application/java-serialized-object" }
        , { ".class"  , "application/java-vm" }
        , { ".jnlp"   , "application/x-java-jnlp-file" }
        /*, { ".", "", 0 }*/
    
        , { ".mpkg"   , "application/vnd.apple.installer+xml" }
        , { ".m3u8"   , "application/vnd.apple.mpegurl" }
    
        , { ".air"    , "application/vnd.adobe.air-application-installer-package+zip" }
        , { ".fxp"    , "application/vnd.adobe.fxp" }
        , { ".fxpl"   , "application/vnd.adobe.fxp" }
        , { ".xdp"    , "application/vnd.adobe.xdp+xml" }
        , { ".xfdf"   , "application/vnd.adobe.xfdf" }
    
        , { ".oas"    , "application/vnd.fujitsu.oasys"  }
        , { ".oa2"    , "application/vnd.fujitsu.oasys2" }
        , { ".oa3"    , "application/vnd.fujitsu.oasys3" }
        , { ".fg5"    , "application/vnd.fujitsu.oasysgp" }
        , { ".bh2"    , "application/vnd.fujitsu.oasysprs" }
        , { ".ddd"    , "application/vnd.fujixerox.ddd" }
        , { ".xdw"    , "application/vnd.fujixerox.docuworks" }
        , { ".xbd"    , "application/vnd.fujixerox.docuworks.binder" }
    
        , { ".kml"    , "application/vnd.google-earth.kml+xml" }
        , { ".kmz"    , "application/vnd.google-earth.kmz" }
        , { ".apk"    , "application/vnd.android.package-archive" }
    
        , { ".karbon" , "application/vnd.kde.karbon" }
        , { ".chrt"   , "application/vnd.kde.kchart" }
        , { ".kfo"    , "application/vnd.kde.kformula" }
        , { ".flw"    , "application/vnd.kde.kivio" }
        , { ".kon"    , "application/vnd.kde.kontour" }
        , { ".kpr"    , "application/vnd.kde.kpresenter" }
        , { ".kpt"    , "application/vnd.kde.kpresenter" }
        , { ".ksp"    , "application/vnd.kde.kspread" }
        , { ".kwd"    , "application/vnd.kde.kword" }
        , { ".kwt"    , "application/vnd.kde.kword" }
        /*, { ".", "", 0 }*/
    
        , { ".123"    , "application/vnd.lotus-1-2-3" }
        , { ".apr"    , "application/vnd.lotus-approach"  }
        , { ".pre"    , "application/vnd.lotus-freelance" }
        , { ".nsf"    , "application/vnd.lotus-notes" }
        , { ".org"    , "application/vnd.lotus-organizer" }
        , { ".scm"    , "application/vnd.lotus-screencam" }
        , { ".lwp"    , "application/vnd.lotus-wordpro" }
        /*, { ".", "", 0 }*/
    
        , { ".sxc"    , "application/vnd.sun.xml.calc" }
        , { ".stc"    , "application/vnd.sun.xml.calc.template" }
        , { ".sxd"    , "application/vnd.sun.xml.draw" }
        , { ".std"    , "application/vnd.sun.xml.draw.template" }
        , { ".sxi"    , "application/vnd.sun.xml.impress" }
        , { ".sti"    , "application/vnd.sun.xml.impress.template" }
        , { ".sxm"    , "application/vnd.sun.xml.math" }
        , { ".sxw"    , "application/vnd.sun.xml.writer" }
        , { ".sxg"    , "application/vnd.sun.xml.writer.global"   }
        , { ".stw"    , "application/vnd.sun.xml.writer.template" }
        /*, { ".", "", 0 }*/
        
        , { ".hvd"    , "application/vnd.yamaha.hv-dic"    }
        , { ".hvs"    , "application/vnd.yamaha.hv-script" }
        , { ".hvp"    , "application/vnd.yamaha.hv-voice"  }
        , { ".osf"    , "application/vnd.yamaha.openscoreformat" }
        , { ".osfpvg" , "application/vnd.yamaha.openscoreformat.osfpvg+xml" }
        , { ".saf"    , "application/vnd.yamaha.smaf-audio"  }
        , { ".spf"    , "application/vnd.yamaha.smaf-phrase" }
    
        , { ".mid"    , "audio/midi" }
        , { ".midi"   , "audio/midi" }
        , { ".kar"    , "audio/midi" }
        , { ".rmi"    , "audio/midi" }
        , { ".mpga"   , "audio/mpeg" }
        , { ".mp2"    , "audio/mpeg" }
        , { ".mp2a"   , "audio/mpeg" }
        , { ".mp3"    , "audio/mpeg" }
        , { ".m2a"    , "audio/mpeg" }
        , { ".m3a"    , "audio/mpeg" }
        , { ".mp4a"   , "audio/mp4"  }
        , { ".oga"    , "audio/ogg"  }
        , { ".ogg"    , "audio/ogg"  }
        , { ".spx"    , "audio/ogg"  }
        , { ".wav"    , "audio/x-wav"}
    
        , { ".adp"    , "audio/adpcm" }
        , { ".au"     , "audio/basic" }
        , { ".snd"    , "audio/basic" }
        , { ".uva"    , "audio/vnd.dece.audio" }
        , { ".uvva"   , "audio/vnd.dece.audio" }
        , { ".eol"    , "audio/vnd.digital-winds" }
        , { ".dra"    , "audio/vnd.dra" }
        , { ".dts"    , "audio/vnd.dts" }
        , { ".dtshd"  , "audio/vnd.dts.hd" }
        , { ".lvp"    , "audio/vnd.lucent.voice" }
        , { ".pya"    , "audio/vnd.ms-playready.media.pya" }
        , { ".ecelp4800", "audio/vnd.nuera.ecelp4800" }
        , { ".ecelp7470", "audio/vnd.nuera.ecelp7470" }
        , { ".ecelp9600", "audio/vnd.nuera.ecelp9600" }
        , { ".rip"    , "audio/vnd.rip" }
        , { ".weba"   , "audio/webm"  }
        , { ".aac"    , "audio/x-aac" }
        , { ".aif"    , "audio/x-aiff" }
        , { ".aiff"   , "audio/x-aiff" }
        , { ".aifc"   , "audio/x-aiff" }
        , { ".m3u"    , "audio/x-mpegurl" }
        , { ".wax"    , "audio/x-ms-wax"  }
        , { ".wma"    , "audio/x-ms-wma"  }
        , { ".ram"    , "audio/x-pn-realaudio" }
        , { ".ra"     , "audio/x-pn-realaudio" }
        , { ".rmp"    , "audio/x-pn-realaudio-plugin" }
        /*, { ".", "", 0 }*/
    
        , { ".mp4"    , "video/mp4"   }
        , { ".mp4v"   , "video/mp4"   }
        , { ".mpg4"   , "video/mp4"   }
        , { ".mpeg"   , "video/mpeg"  }
        , { ".mpg"    , "video/mpeg"  }
        , { ".mpe"    , "video/mpeg"  }
        , { ".m1v"    , "video/mpeg"  }
        , { ".m2v"    , "video/mpeg"  }
        , { ".ogv"    , "video/ogg"   }
        , { ".flv"    , "video/x-flv" }
        , { ".m4v"    , "video/x-m4v" }
        , { ".avi"    , "video/x-msvideo"}
        , { ".asf"    , "video/x-ms-asf" }
        , { ".asx"    , "video/x-ms-asf" }
        , { ".wmv"    , "video/x-ms-wmv" }
        /*, { ".", "", 0 }*/
                
        , { ".3gp"    , "video/3gpp"  }
        , { ".3g2"    , "video/3gpp2" }
        , { ".h261"   , "video/h261"  }
        , { ".h263"   , "video/h263"  }
        , { ".h264"   , "video/h264"  }
        , { ".jpgv"   , "video/jpeg"  }
        , { ".jpm"    , "video/jpm"   }
        , { ".jpgm"   , "video/jpm"   }
        , { ".mj2"    , "video/mj2"   }
        , { ".mjp2"   , "video/mj2"   }
        , { ".qt"     , "video/quicktime"   }
        , { ".mov"    , "video/quicktime"   }
        , { ".uvh"    , "video/vnd.dece.hd" }
        , { ".uvvh"   , "video/vnd.dece.hd" }
        , { ".uvm"    , "video/vnd.dece.mobile" }
        , { ".uvvm"   , "video/vnd.dece.mobile" }
        , { ".uvp"    , "video/vnd.dece.pd" }
        , { ".uvvp"   , "video/vnd.dece.pd" }
        , { ".uvs"    , "video/vnd.dece.sd" }
        , { ".uvvs"   , "video/vnd.dece.sd" }
        , { ".uvv"    , "video/vnd.dece.video" }
        , { ".uvvv"   , "video/vnd.dece.video" }
        , { ".fvt"    , "video/vnd.fvt"     }
        , { ".mxu"    , "video/vnd.mpegurl" }
        , { ".m4u"    , "video/vnd.mpegurl" }
        , { ".uvu"    , "video/vnd.uvvu.mp4" }
        , { ".uvvu"   , "video/vnd.uvvu.mp4" }
        , { ".viv"    , "video/vnd.vivo" }
        , { ".webm"   , "video/webm"  }
        , { ".f4v"    , "video/x-f4v" }
        , { ".fli"    , "video/x-fli" }
        , { ".wm"     , "video/x-ms-wm"  }
        , { ".wmx"    , "video/x-ms-wmx" }
        , { ".wvx"    , "video/x-ms-wvx" }
        , { ".movie"  , "video/x-sgi-movie" }
        /*, { ".", "", 0 }*/
    
        /*
        , { ".ez", "application/andrew-inset", 0 }
        , { ".aw", "application/applixware", 0 }
        , { ".atom", "application/atom+xml", 0 }
        , { ".atomcat", "application/atomcat+xml", 0 }
        , { ".atomsvc", "application/atomsvc+xml", 0 }
        , { ".ccxml", "application/ccxml+xml", 0 }
        , { ".cdmia", "application/cdmi-capability", 0 }
        , { ".cdmic", "application/cdmi-container", 0 }
        , { ".cdmid", "application/cdmi-domain", 0 }
        , { ".cdmio", "application/cdmi-object", 0 }
        , { ".cdmiq", "application/cdmi-queue", 0 }
        , { "", "", 0 }
        */
        };

    return _;
}

/*

application/cu-seeme                cu
application/davmount+xml            davmount
application/dssc+der                dssc
application/dssc+xml                xdssc
application/ecmascript              ecma
application/emma+xml                emma
application/epub+zip                epub
application/exi                 exi
application/font-tdpfr              pfr
application/hyperstudio             stk
application/ipfix               ipfix
application/lost+xml                lostxml
application/mac-binhex40            hqx
application/mac-compactpro          cpt
application/mads+xml                mads
application/marc                mrc
application/marcxml+xml             mrcx
application/mathematica             ma nb mb
application/mathml+xml              mathml
application/mbox                mbox
application/mediaservercontrol+xml      mscml
application/metalink4+xml           meta4
application/mets+xml                mets
application/mods+xml                mods
application/mp21                m21 mp21
application/mp4                 mp4s
application/mxf                 mxf
application/oda                 oda
application/oebps-package+xml           opf
application/ogg                 ogx
application/onenote             onetoc onetoc2 onetmp onepkg
application/patch-ops-error+xml         xer
application/pics-rules              prf
application/pkcs10              p10
application/pkcs7-mime              p7m p7c
application/pkcs7-signature         p7s
application/pkcs8               p8
application/pkix-attr-cert          ac
application/pkix-cert               cer
application/pkix-crl                crl
application/pkix-pkipath            pkipath
application/pkixcmp             pki
application/pls+xml             pls
application/prs.cww             cww
application/pskc+xml                pskcxml
application/rdf+xml             rdf
application/reginfo+xml             rif
application/relax-ng-compact-syntax     rnc
application/resource-lists+xml          rl
application/resource-lists-diff+xml     rld
application/rls-services+xml            rs
application/rsd+xml             rsd
application/rss+xml             rss
application/rtf                 rtf
application/sbml+xml                sbml
application/scvp-cv-request         scq
application/scvp-cv-response            scs
application/scvp-vp-request         spq
application/scvp-vp-response            spp
application/sdp                 sdp
application/set-payment-initiation      setpay
application/set-registration-initiation     setreg
application/shf+xml             shf
application/smil+xml                smi smil
application/sparql-query            rq
application/sparql-results+xml          srx
application/srgs                gram
application/srgs+xml                grxml
application/sru+xml             sru
application/ssml+xml                ssml
application/tei+xml             tei teicorpus
application/thraud+xml              tfi
application/timestamped-data            tsd
application/vnd.3gpp.pic-bw-large       plb
application/vnd.3gpp.pic-bw-small       psb
application/vnd.3gpp.pic-bw-var         pvb
application/vnd.3gpp2.tcap          tcap
application/vnd.3m.post-it-notes        pwn
application/vnd.accpac.simply.aso       aso
application/vnd.accpac.simply.imp       imp
application/vnd.acucobol            acu
application/vnd.acucorp             atc acutc
application/vnd.ahead.space         ahead
application/vnd.airzip.filesecure.azf       azf
application/vnd.airzip.filesecure.azs       azs
application/vnd.amazon.ebook            azw
application/vnd.americandynamics.acc        acc
application/vnd.amiga.ami           ami
application/vnd.anser-web-certificate-issue-initiation  cii
application/vnd.anser-web-funds-transfer-initiation fti
application/vnd.antix.game-component        atx
application/vnd.aristanetworks.swi      swi
application/vnd.audiograph          aep
application/vnd.blueice.multipass       mpm
application/vnd.bmi             bmi
application/vnd.businessobjects         rep
application/vnd.chemdraw+xml            cdxml
application/vnd.chipnuts.karaoke-mmd        mmd
application/vnd.cinderella          cdy
application/vnd.claymore            cla
application/vnd.cloanto.rp9         rp9
application/vnd.clonk.c4group           c4g c4d c4f c4p c4u
application/vnd.cluetrust.cartomobile-config        c11amc
application/vnd.cluetrust.cartomobile-config-pkg    c11amz
application/vnd.commonspace         csp
application/vnd.contact.cmsg            cdbcmsg
application/vnd.cosmocaller         cmc
application/vnd.crick.clicker           clkx
application/vnd.crick.clicker.keyboard      clkk
application/vnd.crick.clicker.palette       clkp
application/vnd.crick.clicker.template      clkt
application/vnd.crick.clicker.wordbank      clkw
application/vnd.criticaltools.wbs+xml       wbs
application/vnd.ctc-posml           pml
application/vnd.cups-ppd            ppd
application/vnd.curl.car            car
application/vnd.curl.pcurl          pcurl
application/vnd.data-vision.rdz         rdz
application/vnd.dece.data           uvf uvvf uvd uvvd
application/vnd.dece.ttml+xml           uvt uvvt
application/vnd.dece.unspecified        uvx uvvx
application/vnd.denovo.fcselayout-link      fe_launch
application/vnd.dna             dna
application/vnd.dolby.mlp           mlp
application/vnd.dpgraph             dpg
application/vnd.dreamfactory            dfac
application/vnd.dvb.ait             ait
application/vnd.dvb.service         svc
application/vnd.dynageo             geo
application/vnd.ecowin.chart            mag
application/vnd.enliven             nml
application/vnd.epson.esf           esf
application/vnd.epson.msf           msf
application/vnd.epson.quickanime        qam
application/vnd.epson.salt          slt
application/vnd.epson.ssf           ssf
application/vnd.eszigno3+xml            es3 et3
application/vnd.ezpix-album         ez2
application/vnd.ezpix-package           ez3
application/vnd.fdf             fdf
application/vnd.fdsn.mseed          mseed
application/vnd.fdsn.seed           seed dataless
application/vnd.flographit          gph
application/vnd.fluxtime.clip           ftc
application/vnd.framemaker          fm frame maker book
application/vnd.frogans.fnc         fnc
application/vnd.frogans.ltf         ltf
application/vnd.fsc.weblaunch           fsc
application/vnd.fuzzysheet          fzs
application/vnd.genomatix.tuxedo        txd
application/vnd.geogebra.file           ggb
application/vnd.geogebra.tool           ggt
application/vnd.geometry-explorer       gex gre
application/vnd.geonext             gxt
application/vnd.geoplan             g2w
application/vnd.geospace            g3w
application/vnd.gmx             gmx
application/vnd.grafeq              gqf gqs
application/vnd.groove-account          gac
application/vnd.groove-help         ghf
application/vnd.groove-identity-message     gim
application/vnd.groove-injector         grv
application/vnd.groove-tool-message     gtm
application/vnd.groove-tool-template        tpl
application/vnd.groove-vcard            vcg
application/vnd.hal+xml             hal
application/vnd.handheld-entertainment+xml  zmm
application/vnd.hbci                hbci
application/vnd.hhe.lesson-player       les
application/vnd.hp-hpgl             hpgl
application/vnd.hp-hpid             hpid
application/vnd.hp-hps              hps
application/vnd.hp-jlyt             jlt
application/vnd.hp-pcl              pcl
application/vnd.hp-pclxl            pclxl
application/vnd.hydrostatix.sof-data        sfd-hdstx
application/vnd.hzn-3d-crossword        x3d
application/vnd.ibm.minipay         mpy
application/vnd.ibm.modcap          afp listafp list3820
application/vnd.ibm.rights-management       irm
application/vnd.ibm.secure-container        sc
application/vnd.iccprofile          icc icm
application/vnd.igloader            igl
application/vnd.immervision-ivp         ivp
application/vnd.immervision-ivu         ivu
application/vnd.insors.igm          igm
application/vnd.intercon.formnet        xpw xpx
application/vnd.intergeo            i2g
application/vnd.intu.qbo            qbo
application/vnd.intu.qfx            qfx
application/vnd.ipunplugged.rcprofile       rcprofile
application/vnd.irepository.package+xml     irp
application/vnd.is-xpr              xpr
application/vnd.isac.fcs            fcs
application/vnd.jam             jam
application/vnd.jcp.javame.midlet-rms       rms
application/vnd.jisp                jisp
application/vnd.joost.joda-archive      joda
application/vnd.kahootz             ktz ktr
application/vnd.kenameaapp          htke
application/vnd.kidspiration            kia
application/vnd.kinar               kne knp
application/vnd.koan                skp skd skt skm
application/vnd.kodak-descriptor        sse
application/vnd.las.las+xml         lasxml
application/vnd.llamagraphics.life-balance.desktop  lbd
application/vnd.llamagraphics.life-balance.exchange+xml lbe
application/vnd.macports.portpkg        portpkg
application/vnd.mcd             mcd
application/vnd.medcalcdata         mc1
application/vnd.mediastation.cdkey      cdkey
application/vnd.mfer                mwf
application/vnd.mfmp                mfm
application/vnd.micrografx.flo          flo
application/vnd.micrografx.igx          igx
application/vnd.mif             mif
application/vnd.mobius.daf          daf
application/vnd.mobius.dis          dis
application/vnd.mobius.mbk          mbk
application/vnd.mobius.mqy          mqy
application/vnd.mobius.msl          msl
application/vnd.mobius.plc          plc
application/vnd.mobius.txf          txf
application/vnd.mophun.application      mpn
application/vnd.mophun.certificate      mpc
application/vnd.mozilla.xul+xml         xul
application/vnd.mseq                mseq
application/vnd.musician            mus
application/vnd.muvee.style         msty
application/vnd.neurolanguage.nlu       nlu
application/vnd.noblenet-directory      nnd
application/vnd.noblenet-sealer         nns
application/vnd.noblenet-web            nnw
application/vnd.nokia.n-gage.data       ngdat
application/vnd.nokia.n-gage.symbian.install    n-gage
application/vnd.nokia.radio-preset      rpst
application/vnd.nokia.radio-presets     rpss
application/vnd.novadigm.edm            edm
application/vnd.novadigm.edx            edx
application/vnd.novadigm.ext            ext
application/vnd.olpc-sugar          xo
application/vnd.oma.dd2+xml         dd2
application/vnd.osgeo.mapguide.package      mgp
application/vnd.osgi.dp             dp
application/vnd.palm                pdb pqa oprc
application/vnd.pawaafile           paw
application/vnd.pg.format           str
application/vnd.pg.osasli           ei6
application/vnd.picsel              efif
application/vnd.pmi.widget          wg
application/vnd.pocketlearn         plf
application/vnd.powerbuilder6           pbd
application/vnd.previewsystems.box      box
application/vnd.proteus.magazine        mgz
application/vnd.publishare-delta-tree       qps
application/vnd.pvi.ptid1           ptid
application/vnd.realvnc.bed         bed
application/vnd.recordare.musicxml      mxl
application/vnd.recordare.musicxml+xml      musicxml
application/vnd.rig.cryptonote          cryptonote
application/vnd.rim.cod             cod
application/vnd.rn-realmedia            rm
application/vnd.route66.link66+xml      link66
application/vnd.sailingtracker.track        st
application/vnd.seemail             see
application/vnd.sema                sema
application/vnd.semd                semd
application/vnd.semf                semf
application/vnd.shana.informed.formdata     ifm
application/vnd.shana.informed.formtemplate itp
application/vnd.shana.informed.interchange  iif
application/vnd.shana.informed.package      ipk
application/vnd.simtech-mindmapper      twd twds
application/vnd.smaf                mmf
application/vnd.smart.teacher           teacher
application/vnd.solent.sdkm+xml         sdkm sdkd
application/vnd.spotfire.dxp            dxp
application/vnd.spotfire.sfs            sfs
application/vnd.stardivision.calc       sdc
application/vnd.stardivision.draw       sda
application/vnd.stardivision.impress        sdd
application/vnd.stardivision.math       smf
application/vnd.stardivision.writer     sdw vor
application/vnd.stardivision.writer-global  sgl
application/vnd.stepmania.stepchart     sm
application/vnd.sus-calendar            sus susp
application/vnd.svd             svd
application/vnd.symbian.install         sis sisx
application/vnd.syncml+xml          xsm
application/vnd.syncml.dm+wbxml         bdm
application/vnd.syncml.dm+xml           xdm
application/vnd.tao.intent-module-archive   tao
application/vnd.tmobile-livetv          tmo
application/vnd.trid.tpt            tpt
application/vnd.triscape.mxs            mxs
application/vnd.trueapp             tra
application/vnd.ufdl                ufd ufdl
application/vnd.uiq.theme           utz
application/vnd.umajin              umj
application/vnd.unity               unityweb
application/vnd.uoml+xml            uoml
application/vnd.vcx             vcx
application/vnd.visionary           vis
application/vnd.vsf             vsf
application/vnd.wap.wbxml           wbxml
application/vnd.wap.wmlc            wmlc
application/vnd.wap.wmlscriptc          wmlsc
application/vnd.webturbo            wtb
application/vnd.wolfram.player          nbp
application/vnd.wqd             wqd
application/vnd.wt.stf              stf
application/vnd.xara                xar
application/vnd.xfdl                xfdl
application/vnd.yellowriver-custom-menu     cmp
application/vnd.zul             zir zirz
application/vnd.zzazz.deck+xml          zaz
application/voicexml+xml            vxml
application/widget              wgt
application/wsdl+xml                wsdl
application/wspolicy+xml            wspolicy
application/x-abiword               abw
application/x-ace-compressed            ace
application/x-authorware-bin            aab x32 u32 vox
application/x-authorware-map            aam
application/x-authorware-seg            aas
application/x-bcpio             bcpio
application/x-bittorrent            torrent
application/x-cdlink                vcd
application/x-chat              chat
application/x-chess-pgn             pgn
application/x-cpio              cpio
application/x-debian-package            deb udeb
application/x-director          dir dcr dxr cst cct cxt w3d fgd swa
application/x-doom              wad
application/x-dtbncx+xml            ncx
application/x-dtbook+xml            dtb
application/x-dtbresource+xml           res
application/x-dvi               dvi
application/x-font-bdf              bdf
application/x-font-ghostscript          gsf
application/x-font-linux-psf            psf
application/x-font-otf              otf
application/x-font-pcf              pcf
application/x-font-snf              snf
application/x-font-ttf              ttf ttc
application/x-font-type1            pfa pfb pfm afm
application/x-font-woff             woff
application/x-futuresplash          spl
application/x-gnumeric              gnumeric

application/x-hdf               hdf
application/x-mobipocket-ebook          prc mobi
application/x-netcdf                nc cdf
application/x-pkcs12                p12 pfx
application/x-pkcs7-certificates        p7b spc
application/x-pkcs7-certreqresp         p7r
application/x-shar              shar
application/x-shockwave-flash           swf
application/x-silverlight-app           xap
application/x-stuffit               sit
application/x-stuffitx              sitx
application/x-sv4cpio               sv4cpio
application/x-sv4crc                sv4crc
application/x-tcl               tcl
application/x-ustar             ustar
application/x-wais-source           src
application/x-xfig              fig
application/x-xpinstall             xpi
application/xcap-diff+xml           xdf
application/xenc+xml                xenc
application/xop+xml             xop
application/xspf+xml                xspf
application/xv+xml              mxml xhvml xvml xvm
application/yang                yang
application/yin+xml             yin
chemical/x-cdx                  cdx
chemical/x-cif                  cif
chemical/x-cmdf                 cmdf
chemical/x-cml                  cml
chemical/x-csml                 csml
chemical/x-xyz                  xyz
message/rfc822                  eml mime
model/iges                  igs iges
model/mesh                  msh mesh silo
model/vnd.collada+xml               dae
model/vnd.dwf                   dwf
model/vnd.gdl                   gdl
model/vnd.gtw                   gtw
model/vnd.mts                   mts
model/vnd.vtu                   vtu
model/vrml                  wrl vrml
x-conference/x-cooltalk             ice


AddType font/opentype .svg .svgz
AddEncoding gzip .svgz
http://habrahabr.ru/post/166291/

*/


