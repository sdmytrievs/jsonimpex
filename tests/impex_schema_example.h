#pragma once

// File examples into Resource/files

const char* const import_simple_FormatTableFile_data =
"input-0	0	0.1	0\t\n"
"input-1	1	1	100.1\t\n"
"input-2	2	2	200.2\t\n"
"input-3	3	3	300.3\t\n"
"input-4	4	4	400.4\t\n"
"input-5	5	5	500.5\t\n"
"input-6	6	6	600.6\t\n"
"input-7	7	7	700.7\t\n"
"input-8	8	8	800.8\t\n"
"input-9	9	9	900.9\t\n";

const char* const import_simple_FormatTableFile_script = R"({
     "block" :   {
          "defaults" :   {
               "vlist" :   [ 1, 2, 3]
          },
          "pairs" :   {

          },
          "matches" :   {
               "int" :   {
                    "field" :   "vint",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "name" :   {
                    "field" :   "vstring",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "add" :   {
                    "field" :   "vlist.0",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "double" :   {
                    "field" :   "vdouble",
                    "ignore" :   false,
                    "script" :   "field = field / 10;",
                    "convert" :   {

                    }
               }
          },
          "script" :   ""
     },
     "format" :   {
          "Nhcols" :   4,
          "Nhrows" :   0,
          "headers" :   [
               "name",
               "int",
               "add",
               "double"
          ],
          "rowend" :   "\n",
          "rows_one_block" :   1,
          "colends" :   "\t",
          "usequotes" :   false,
          "usemore" :   true,
          "comment_regexp" :   "#.*",
          "row_size" :   0,
          "colsizes" :   [

          ],
          "value_regexp" :   [

          ]
     },
     "renderer" :   "",
     "label" :   "SimpleSchemaTest",
     "separators" :   {
          "v_sep" :   "",
          "l_sep" :   "",
          "r_sep" :   "",
          "c_head" :   "",
          "c_end" :   "",
          "eod" :   "",
          "encoding" :   "",
          "str_delim" :   "",
          "bod" :   ""
     },
     "comment" :   "",
     "fname" :   "",
     "Nblocks" :   0,
     "Nlines" :   0,
     "direction" :   0
}
)";

const char* const export_simple_FormatTableFile_script = R"({
     "block" :   {
          "defaults" :   {
               "add" :   "0.1"
          },
          "pairs" :   {

          },
          "matches" :   {
               "int" :   {
                    "field" :   "vint",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "name" :   {
                    "field" :   "vstring",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "add" :   {
                    "field" :   "vlist.0",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "double" :   {
                    "field" :   "vdouble",
                    "ignore" :   false,
                    "script" :   "field = field * 10;",
                    "convert" :   {

                    }
               }
          },
          "script" :   " if( rintable.vlist ) \n  then rintable.add = rintable.vlist[0] end"
     },
     "format" :   {
          "Nhcols" :   4,
          "Nhrows" :   1,
          "headers" :   [
               "name",
               "int",
               "add",
               "double"
          ],
          "rowend" :   "\n",
          "rows_one_block" :   1,
          "colends" :   "\t",
          "usequotes" :   false,
          "usemore" :   true,
          "comment_regexp" :   "#.*",
          "row_size" :   0,
          "colsizes" :   [

          ],
          "value_regexp" :   [

          ]
     },
     "renderer" :   "",
     "label" :   "SimpleSchemaTest",
     "separators" :   {
          "v_sep" :   "",
          "l_sep" :   "",
          "r_sep" :   "",
          "c_head" :   "",
          "c_end" :   "",
          "eod" :   "",
          "encoding" :   "",
          "str_delim" :   "",
          "bod" :   ""
     },
     "comment" :   "",
     "fname" :   "",
     "Nblocks" :   0,
     "Nlines" :   0,
     "direction" :   1
}
)";

//--------------------------------------------------------------------------------

// simple_export_keyvalue.txt
const char* const import_simple_FormatKeyValueFile_data =
        "\ninput-0\n\t-add\t0.1 \n\t-double\t0 \n\t-int\t0 \n\n"
        "input-1\n\t-add\t0.1 \n\t-double\t10.01 \n\t-int\t1 \n\t-list\t1 2 3 \n\n"
        "input-2\n\t-add\t0.1 \n\t-double\t20.02 \n\t-int\t2 \n\t-list\t2 4 6 \n\n"
        "input-3\n\t-add\t0.1 \n\t-double\t30.03 \n\t-int\t3 \n\t-list\t3 6 9 \n\n"
        "input-4\n\t-add\t0.1 \n\t-double\t40.04 \n\t-int\t4 \n\t-list\t4 8 12 \n\n"
        "input-5\n\t-add\t0.1 \n\t-double\t50.05 \n\t-int\t5 \n\t-list\t5 10 15 \n\n"
        "input-6\n\t-add\t0.1 \n\t-double\t60.06 \n\t-int\t6 \n\t-list\t6 12 18 \n\n"
        "input-7\n\t-add\t0.1 \n\t-double\t70.07 \n\t-int\t7 \n\t-list\t7 14 21 \n\n"
        "input-8\n\t-add\t0.1 \n\t-double\t80.08 \n\t-int\t8 \n\t-list\t8 16 24 \n\n"
        "input-9\n\t-add\t0.1 \n\t-double\t90.09 \n\t-int\t9 \n\t-list\t9 18 27 \n";

const char* const import_simple_FormatKeyValueFile_script = R"({
     "block" :   {
          "defaults" :   {
               "vbool" :   true
          },
          "pairs" :   {
               "list" :   {
                    "organization" :   "list"
               }
          },
          "matches" :   {
               "int" :   {
                    "field" :   "vint",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "head0" :   {
                    "field" :   "vstring",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "double" :   {
                    "field" :   "vdouble",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "list" :   {
                    "field" :   "vlist",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               }
          },
          "script" :   ""
     },
     "format" :   {
          "head_regexp" :   "\\s*([^\\s]+)\\s*\\n",
          "end_regexp" :   "",
          "key_regexp" :   "\\s*-([a-zA-Z]\\w*)\\s*",
          "value_regexp" :   "\\s*([^#\\n]*)\\s*",
          "value_next" :   "",
          "value_token_regexp" :   "\\s+",
          "comment_regexp" :   "#([^\\n]*)\\n*",
          "Ndata" :   0
     },
     "renderer" :   " ",
     "label" :   "SimpleSchemaTest",
     "separators" :   {
          "v_sep" :   "",
          "l_sep" :   "",
          "r_sep" :   "",
          "c_head" :   "",
          "c_end" :   "",
          "eod" :   "",
          "encoding" :   "",
          "str_delim" :   "",
          "bod" :   ""
     },
     "comment" :   "",
     "fname" :   "",
     "Nblocks" :   0,
     "Nlines" :   0,
     "direction" :   0
})";

const char* const export_simple_FormatKeyValueFile_script = R"({
     "block" :   {
          "defaults" :   {
               "add" :   "0.1"
          },
          "pairs" :   {
               "list" :   {
                    "organization" :   "list"
               }
          },
          "matches" :   {
               "int" :   {
                    "field" :   "vint",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "head0" :   {
                    "field" :   "vstring",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "double" :   {
                    "field" :   "vdouble",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "list" :   {
                    "field" :   "vlist",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               }
          },
          "script" :   ""
     },
     "format" :   {
          "head_regexp" :   "\n%head0\n",
          "end_regexp" :   "",
          "key_regexp" :   "\t-%key\t",
          "value_regexp" :   "%value",
          "value_next" :   "\n",
          "value_token_regexp" :   " ",
          "comment_regexp" :   "#",
          "Ndata" :   0
     },
     "renderer" :   " ",
     "label" :   "SimpleSchemaTest",
     "separators" :   {
          "v_sep" :   "",
          "l_sep" :   "",
          "r_sep" :   "",
          "c_head" :   "",
          "c_end" :   "",
          "eod" :   "",
          "encoding" :   "",
          "str_delim" :   "",
          "bod" :   ""
     },
     "comment" :   "",
     "fname" :   "",
     "Nblocks" :   0,
     "Nlines" :   0,
     "direction" :   1
})";


//---------------------------------------------------------------------

// could be yaml, json or xml data type
const char* const import_simple_FormatStructDataFile_data = R"(- yint: 0
  ydouble: 0
  ystring: yaml-0
  ylist:
    - 0
    - 0
    - 0
- yint: 1
  ydouble: 3
  ystring: yaml-1
  ylist:
    - 11
    - 22
    - 33
- yint: 2
  ydouble: 6
  ystring: yaml-2
  ylist:
    - 22
    - 44
    - 66
- yint: 3
  ydouble: 9
  ystring: yaml-3
  ylist:
    - 33
    - 66
    - 99
- yint: 4
  ydouble: 12
  ystring: yaml-4
  ylist:
    - 44
    - 88
    - 132
)";

const char* const import_simple_FormatStructDataFile_script = R"({
     "block" :   {
          "defaults" :   {
               "vbool" :   true
          },
          "pairs" :   {

          },
          "matches" :   {
               "yint" :   {
                    "field" :   "vint",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {
                        }
               },
               "ystring" :   {
                    "field" :   "vstring",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "ylist" :   {
                    "field" :   "vlist",
                    "ignore" :   false,
                    "script" :   "",
                    "convert" :   {

                    }
               },
               "ydouble" :   {
                    "field" :   "vdouble",
                    "ignore" :   false,
                    "script" :   "field = field / 100",
                    "convert" :   {

                    }
               }
          },
          "script" :   ""
     },
     "renderer" :   "yaml",
     "label" :   "SimpleSchemaTest",
     "comment" :   "",
     "fname" :   "",
     "Nblocks" :   0,
     "Nlines" :   0,
     "direction" :   0
}
)";

