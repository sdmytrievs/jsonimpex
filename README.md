## JSONIMPEX17

Jsonimpex17 is a library and API providing a generic interface for exchanging the structured data between JSON, YAML, or XML files and ArangoDB multi-model database back-ends and for importing/exporting data from/to arbitrary foreign formats. This is extension Jsonio17 library for using YAML and XML input/output file formats and for export/import  internal json schemas defined data using thrift defined converters.

* Jsonimpex17 is written in C/C++ using open-source libraries jsonArango, Jsonio17, YAML-CPP, Pugixml, Thrift and Lua.
* Version: currently 0.1.
* Will be distributed as is (no liability) under the terms of Lesser GPL v.3 license. 

### How to get Jsonimpex17 source code

* In your home directory, make a folder named e.g. ~/jsonimpex17.
* cd ~/jsonimpex17 and clone this repository from https://bitbucket.org/gems4/jsonimpex17.git  using a preinstalled free git client SourceTree or SmartGit (the best way on Windows). 
* Alternatively on Mac OS X or linux, open a terminal and type in the command line (do not forget a period):

```sh
git clone https://bitbucket.org/gems4/jsonimpex17.git . 

```
## How to install the Jsonimpex17 library ##

 Building Jsonimpex17 requires g++, [CMake](http://www.cmake.org/). 

* Make sure you have g++, cmake and git installed. If not, install them. 

  On Ubuntu linux terminal, this can be done using the following commands:

```sh
#!bash
sudo apt-get install g++ cmake git libssl-dev libtool byacc flex
```

  For Mac OSX, make sure you have Homebrew installed (see [Homebrew web site](http://brew.sh) and [Homebrew on Mac OSX El Capitan](http://digitizor.com/install-homebrew-osx-el-capitan/) ).


* Install Dependencies ( Velocypack, [jsonArango](https://bitbucket.org/gems4/jsonarango/src/master/), [Jsonio17](https://bitbucket.org/gems4/jsonio17/src/master/), YAML-CPP, Pugixml, Thrift and Lua)

In order to build the Jsonimpex17 library on (k)ubuntu linux 18.04/20.04 or MacOS, first execute the following: 

```sh
#!bash
cd ~/jsonimpex17
sudo ./install-dependencies.sh
```

* Install the Jsonimpex17 library

Then navigate to the directory where this README.md file is located and type in terminal:

```sh
cd ~/jsonimpex17
sudo ./install.sh
```

* After that, headers, library  and the third-party libraries can be found in /usr/local/{include,lib}. 

* Install current version of ArangoDB server locally ( see [jsonArango](https://bitbucket.org/gems4/jsonarango/src/master/) ).

### How to use Jsonimpex17 (use cases) 

* XML format example 

```c++
{
    auto json_arr = json::loads( "{\"about\":{\"version\":1},\"values\":[[1,2],[3,4]]}" );
    auto xml_txt = XML::dump(json_arr);

    auto xml_arr = JsonFree::array();
    XML::loads( xml_txt, xml_arr  );

}

```

* Yaml format example 

```c++
{
    auto json_arr = json::loads( "{\"about\":{\"version\":1},\"values\":[[1,2],[3,4]]}" );
    auto yaml_txt = yaml::dump(json_arr);

    auto yaml_arr = JsonFree::array();
    yaml::loads( yaml_txt, yaml_arr  );

}

```
For more information on APIs and more examples, take a look into the /examples directory.

